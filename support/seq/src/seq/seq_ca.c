/*************************************************************************\
Copyright (c) 1991-1994 The Regents of the University of California
                        and the University of Chicago.
                        Los Alamos National Laboratory
Copyright (c) 2010-2015 Helmholtz-Zentrum Berlin f. Materialien
                        und Energie GmbH, Germany (HZB)
This file is distributed subject to a Software License Agreement found
in the file LICENSE that is included with this distribution.
\*************************************************************************/
/*	Process Variable interface for sequencer.
 *
 *	Author:  Andy Kozubal
 *	Date:    July, 1991
 *
 *	Experimental Physics and Industrial Control System (EPICS)
 *
 *	This software was produced under  U.S. Government contracts:
 *	(W-7405-ENG-36) at the Los Alamos National Laboratory,
 *	and (W-31-109-ENG-38) at Argonne National Laboratory.
 *
 *	Initial development by:
 *	  The Controls and Automation Group (AT-8)
 *	  Ground Test Accelerator
 *	  Accelerator Technology Division
 *	  Los Alamos National Laboratory
 *
 *	Co-developed with
 *	  The Controls and Computing Group
 *	  Accelerator Systems Division
 *	  Advanced Photon Source
 *	  Argonne National Laboratory
 */
#include "seq.h"
#include "seq_debug.h"

static void proc_db_events(
	pvValue		*value,	/* ptr to value */
	pvType		type,	/* type of value */
	CHAN		*ch,	/* channel object */
	SSCB		*ss,	/* originator, for put and get, else 0 */
	pvEventType	evtype,	/* put, get, or monitor */
	pvStat		status	/* status from pv layer */
);

/*
 * seq_connect() - Initiate connect & monitor requests to PVs.
 * If wait is TRUE, wait for all connections to be established.
 */
pvStat seq_connect(PROG *sp, boolean wait)
{
	pvStat		status;
	unsigned	nch;
	int		delay = 2;
	boolean		ready = FALSE;

	/*
	 * For each channel: create pv object, then subscribe if monitored.
	 */
	for (nch = 0; nch < sp->numChans; nch++)
	{
		CHAN *ch = sp->chan + nch;
		DBCHAN *dbch = ch->dbch;

		if (dbch == NULL)
			continue; /* skip records without pv names */
		DEBUG("seq_connect: connect %s to %s\n", ch->varName,
			dbch->dbName);
		/* Connect to it */
		status = pvVarCreate(
				sp->pvSys,		/* PV system context */
				ch->dbch->dbName,	/* PV name */
				seq_conn_handler,	/* connection handler routine */
				seq_event_handler,	/* event handler routine */
				ch,			/* private data is CHAN struc */
				&dbch->pvid);		/* ptr to PV id */
		if (status != pvStatOK)
		{
			errlogSevPrintf(errlogFatal, "seq_connect(var '%s', pv '%s'): pvVarCreate() failure: "
				"%s\n", ch->varName, dbch->dbName, pvVarGetMess(dbch->pvid));
			free(ch->dbch->dbName);
			free(ch->dbch);
			continue;
		}
	}
	pvSysFlush(sp->pvSys);

	if (wait)
	{
		boolean firstTime = TRUE;
		double timeStartWait;
		pvTimeGetCurrentDouble(&timeStartWait);

		do {
			unsigned ac, mc, cc, gmc;
			double timeNow = timeStartWait;
			/* Check whether we have been asked to exit */
			if (sp->die)
				return pvStatERROR;

			epicsMutexMustLock(sp->lock);
			ac = sp->assignCount;
			mc = sp->monitorCount;
			cc = sp->connectCount;
			gmc = sp->gotMonitorCount;
			epicsMutexUnlock(sp->lock);

			ready = ac == cc && mc == gmc;
			if (!ready)
			{
				if (!firstTime)
				{
					errlogSevPrintf(errlogMinor,
						"%s[%d](after %d sec): assigned=%d, connected=%d, "
						"monitored=%d, got monitor=%d\n",
						sp->progName, sp->instance,
						(int)(timeNow - timeStartWait),
						ac, cc, mc, gmc);
				}
				firstTime = FALSE;
				if (epicsEventWaitWithTimeout(
					sp->ready, (double)delay) == epicsEventWaitError)
				{
					errlogSevPrintf(errlogFatal, "seq_connect: "
						"epicsEventWaitWithTimeout failure\n");
					return pvStatERROR;
				}
				pvTimeGetCurrentDouble(&timeNow);
				if (delay < 3600)
					delay = (int)(delay*1.71);
				else
					delay = 3600;
			}
		} while (!ready);
		errlogSevPrintf(errlogInfo,
			"%s[%d]: all channels connected & received 1st monitor\n",
			sp->progName, sp->instance);
	}
	return pvStatOK;
}

/*
 * seq_get_handler() - Sequencer callback handler.
 * Called when a "get" completes.
 */
static void seq_get_handler(
	pvType type, unsigned count, pvValue *value, void *arg, pvStat status)
{
	PVREQ	*rq = (PVREQ *)arg;
	CHAN	*ch = rq->ch;
	SSCB	*ss = rq->ss;
	PROG	*sp = ch->prog;

	freeListFree(sp->pvReqPool, arg);
	if (!ch->dbch) return;
	/* ignore callback if not expected, e.g. already timed out */
	if (ss->getReq[chNum(ch)] == rq)
		proc_db_events(value, type, ch, ss, pvEventGet, status);
}

/*
 * seq_put_handler() - Sequencer callback handler.
 * Called when a "put" completes.
 */
static void seq_put_handler(
	pvType type, unsigned count, pvValue *value, void *arg, pvStat status)
{
	PVREQ	*rq = (PVREQ *)arg;
	CHAN	*ch = rq->ch;
	SSCB	*ss = rq->ss;
	PROG	*sp = ch->prog;

	freeListFree(sp->pvReqPool, arg);
	if (!ch->dbch) return;
	/* ignore callback if not expected, e.g. already timed out */
	if (ss->putReq[chNum(ch)] == rq)
		proc_db_events(value, type, ch, ss, pvEventPut, status);
}

/*
 * seq_mon_handler() - PV events (monitors) come here.
 */
static void seq_mon_handler(
	pvType type, unsigned count, pvValue *value, void *arg, pvStat status)
{
	CHAN	*ch = (CHAN *)arg;
	PROG	*sp = ch->prog;
	DBCHAN	*dbch = ch->dbch;

	if (!dbch) return;
	proc_db_events(value, type, ch, 0, pvEventMonitor, status);
	if (!dbch->gotMonitor)
	{
		dbch->gotMonitor = TRUE;
		epicsMutexMustLock(sp->lock);
		sp->gotMonitorCount++;
		if (sp->gotMonitorCount == sp->monitorCount
			&& sp->connectCount == sp->assignCount)
		{
			epicsEventSignal(sp->ready);
		}
		epicsMutexUnlock(sp->lock);
	}
}

/*
 * seq_event_handler() - main CA event handler.
 */
void seq_event_handler(
	pvEventType evt, void *arg, pvType type, unsigned count, pvValue *value, pvStat status)
{
	switch (evt)
	{
	case pvEventGet:
		seq_get_handler(type, count, value, arg, status);
		break;
	case pvEventPut:
		seq_put_handler(type, count, value, arg, status);
		break;
	case pvEventMonitor:
		seq_mon_handler(type, count, value, arg, status);
		break;
	}
}

struct putq_cp_arg {
	CHAN	*ch;
	void	*value;
};

static void *putq_cp(void *dest, const void *src, size_t elemSize)
{
	struct putq_cp_arg *arg = (struct putq_cp_arg *)src;
	CHAN *ch = arg->ch;

	return memcpy(dest, arg->value,
		pv_size_n(ch->type->getType, ch->dbch->dbCount));
}

/* Common code for completion and monitor handling */
static void proc_db_events(
	pvValue		*value,
	pvType		type,
	CHAN		*ch,
	SSCB		*ss,
	pvEventType	evtype,
	pvStat		status
)
{
	PROG	*sp = ch->prog;
	DBCHAN	*dbch = ch->dbch;
	static const char *event_type_name[] = {"get","put","mon"};

	assert(dbch != NULL);

	DEBUG("proc_db_events: var=%s, pv=%s, type=%s, status=%d\n", ch->varName,
		dbch->dbName, event_type_name[evtype], status);

	/* monitor on var queued via syncQ */
	if (ch->queue && evtype == pvEventMonitor)
	{
		boolean	full;
		struct putq_cp_arg arg = {ch, value};

		DEBUG("proc_db_events: var=%s, pv=%s, queue=%p, used(max)=%d(%d)\n",
			ch->varName, ch->dbch->dbName,
			ch->queue, seqQueueUsed(ch->queue), seqQueueNumElems(ch->queue));
		/* Copy whole message into queue; no need to lock against other
		   writers, because named and anonymous PVs are disjoint. */
		full = seqQueuePutF(ch->queue, putq_cp, &arg);
		if (full)
		{
			errlogSevPrintf(errlogMinor,
			  "monitor event for variable '%s' (pv '%s'): "
			  "last queue element overwritten (queue is full)\n",
			  ch->varName, ch->dbch->dbName
			);
		}
	}
	else if (value != NULL)
	{
		/* Copy value and meta data into user variable shared buffer */
		void *val = pv_value_ptr(value,type);
		PVMETA meta;

		/* put completion always has NULL value pointer */
		assert(evtype != pvEventPut);
		/* must not use an initializer here, the MS C compiler chokes on it */
		meta.timeStamp = pv_stamp(value,type);
		meta.status = pv_status(value,type);
		meta.severity = pv_severity(value,type);
		meta.message = NULL;

		/* Set error message only when severity indicates error */
		if (meta.severity != pvSevrNONE)
		{
			const char *pmsg = pvVarGetMess(dbch->pvid);
			if (!pmsg) pmsg = "unknown";
			meta.message = pmsg;
		}

		/* Write value and meta data to shared buffers.
		   Set the dirty flag only if this was a monitor event. */
		ss_write_buffer(ch, val, &meta, evtype == pvEventMonitor);
	}

	epicsMutexMustLock(sp->lock);

	/* Signal completion */
	switch (evtype)
	{
	case pvEventPut:
		ss->putReq[chNum(ch)] = NULL;
		epicsEventSignal(ss->syncSem);
		break;
	case pvEventGet:
		ss->getReq[chNum(ch)] = NULL;
		epicsEventSignal(ss->syncSem);
		if (optTest(sp, OPT_SAFE))
			break;
		/* else: fall through */
	case pvEventMonitor:
		/* Wake up each state set that uses this channel in a when condition. */
		/* In safe mode this is only necessary for monitor events, since the
		   effects of get events are local to the state set. */
		ss_wakeup(sp, ch->eventNum);
		break;
	}

	/* If there's an event flag associated with this channel, set it */
	if (ch->syncedTo)
		seq_efSet(sp->ss, ch->syncedTo);

	epicsMutexUnlock(sp->lock);
}

/* Disconnect all database channels */
void seq_disconnect(PROG *sp)
{
	unsigned nch;

	DEBUG("seq_disconnect: sp = %p\n", sp);

	epicsMutexMustLock(sp->lock);
	for (nch = 0; nch < sp->numChans; nch++)
	{
		CHAN	*ch = sp->chan + nch;
		pvStat	status;
		DBCHAN	*dbch = ch->dbch;

		if (!dbch)
			continue;
		DEBUG("seq_disconnect: disconnect %s from %s\n",
			ch->varName, dbch->dbName);
		/* Disconnect this PV */
		epicsMutexUnlock(sp->lock);
		/* Note: must unlock around pvVarDestroy to avoid deadlock
		   with pending callbacks. */
		status = pvVarDestroy(&dbch->pvid);
		epicsMutexMustLock(sp->lock);
		if (status != pvStatOK)
			errlogSevPrintf(errlogFatal, "seq_disconnect(var '%s', pv '%s'): pvVarDestroy() failure: "
				"%s\n", ch->varName, dbch->dbName, pvVarGetMess(dbch->pvid));
	}
	epicsMutexUnlock(sp->lock);

	pvSysFlush(sp->pvSys);
}

pvStat seq_camonitor(CHAN *ch, boolean turn_on)
{
	DBCHAN	*dbch;
	PROG	*sp = ch->prog;
	pvStat	status;
	boolean	done;

	assert(ch);

	epicsMutexMustLock(sp->lock);
	dbch = ch->dbch;
	assert(dbch);
	done = turn_on == pvMonIsDefined(dbch->pvid);
	dbch->gotMonitor = FALSE;
	epicsMutexUnlock(sp->lock);

	if (done)
		return pvStatOK;

	DEBUG("calling pvVarMonitor%s(%p)\n", turn_on ? "On" : "Off", ch);
	if (turn_on)
	{
		status = pvVarMonitorOn(
				&dbch->pvid,		/* pvid */
				ch->type->getType,	/* requested type */
				ch->count,		/* element count */
				ch);			/* user arg (channel struct) */
	}
	else
	{
		status = pvVarMonitorOff(&dbch->pvid);
		epicsMutexMustLock(sp->lock);
		sp->gotMonitorCount -= 1;
		epicsMutexUnlock(sp->lock);
	}
	if (status != pvStatOK)
		errlogSevPrintf(errlogFatal, "seq_camonitor: pvVarMonitor%s(var '%s', pv '%s') failure: %s\n",
			turn_on?"On":"Off", ch->varName, dbch->dbName, pvVarGetMess(dbch->pvid));
	return status;
}

/*
 * seq_conn_handler() - Sequencer connection handler.
 * Called each time a connection is established or broken.
 */
void seq_conn_handler(int connected, void *arg)
{
	CHAN	*ch = (CHAN *)arg;
	PROG	*sp = ch->prog;
	DBCHAN	*dbch = ch->dbch;

	epicsMutexMustLock(sp->lock);

	if (!dbch)
	{
		epicsMutexUnlock(sp->lock);
		return;
	}

	if (!connected)
	{
		DEBUG("%s disconnected from %s\n", ch->varName, dbch->dbName);
		if (dbch->connected)
		{
			unsigned nss;

			dbch->connected = FALSE;
			sp->connectCount--;

			if (ch->monitored)
			{
				seq_camonitor(ch, FALSE);
			}
			/* terminate outstanding requests that wait for completion */
			/* TODO: can there be a race condition with pvPut/pvGet? */
			for (nss = 0; nss < sp->numSS; nss++)
			{
				SSCB *ss = sp->ss + nss;

				ss->getReq[chNum(ch)] = NULL;
				ss->putReq[chNum(ch)] = NULL;
				epicsEventSignal(ss->syncSem);
			}
		}
		else
		{
			errlogSevPrintf(errlogMinor,
				"seq_conn_handler(var '%s', pv '%s'): "
				"disconnect event but already disconnected\n",
				ch->varName, dbch->dbName);
		}
	}
	else	/* connected */
	{
		DEBUG("%s connected to %s\n", ch->varName, dbch->dbName);
		if (!dbch->connected)
		{
			unsigned dbCount;
			dbch->connected = TRUE;
			sp->connectCount++;
			if (sp->gotMonitorCount == sp->monitorCount
				&& sp->connectCount == sp->assignCount)
			{
				epicsEventSignal(sp->ready);
			}
			assert(pvVarIsDefined(dbch->pvid));
			dbCount = pvVarGetCount(&dbch->pvid);
			assert(dbCount >= 0);
			dbch->dbCount = min(ch->count, (unsigned)dbCount);

			if (ch->monitored)
			{
				seq_camonitor(ch, TRUE);
			}
		}
		else
		{
			errlogSevPrintf(errlogMinor,
				"seq_conn_handler: var '%s', pv '%s': "
				"connect event but already connected\n",
				ch->varName, dbch->dbName);
		}
	}
	epicsMutexUnlock(sp->lock);

	/* Wake up each state set that is waiting for event processing.
	   Why each one? Because pvConnectCount and pvMonitorCount should
	   act like monitored anonymous channels. Any state set might be
	   using these functions inside a when-condition and it is expected
	   that such conditions get checked whenever these counts change. */
	ss_wakeup(sp, 0);
}
