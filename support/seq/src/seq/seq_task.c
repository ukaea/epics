/*************************************************************************\
Copyright (c) 1990-1994 The Regents of the University of California
                        and the University of Chicago.
                        Los Alamos National Laboratory
Copyright (c) 2010-2015 Helmholtz-Zentrum Berlin f. Materialien
                        und Energie GmbH, Germany (HZB)
This file is distributed subject to a Software License Agreement found
in the file LICENSE that is included with this distribution.
\*************************************************************************/
/*************************************************************************\
            Thread creation and control for sequencer state sets
\*************************************************************************/
#include "seq.h"
#include "seq_debug.h"

static void ss_entry(void *arg);

/*
 * sequencer() - Sequencer main thread entry point.
 */
void sequencer (void *arg)	/* ptr to original (global) state program table */
{
	PROG		*sp = (PROG *)arg;
	unsigned	nss;
	size_t		threadLen;
	char		threadName[THREAD_NAME_SIZE+10];

	/* Get this thread's id */
	sp->ss->threadId = epicsThreadGetIdSelf();

	/* Add the program to the program list */
	seqAddProg(sp);

	createOrAttachPvSystem(sp);

	if (!pvSysIsDefined(sp->pvSys))
	{
		sp->die = TRUE;
		goto exit;
	}

	/* Call sequencer init function to initialize variables. */
	sp->initFunc(sp);

	/* Initialize state set variables. In safe mode, copy variable
	   block to state set buffers. Must do all this before connecting. */
	if (optTest(sp, OPT_SAFE))
	{
		for (nss = 0; nss < sp->numSS; nss++)
		{
			SSCB	*ss = sp->ss + nss;
			memcpy(ss->var, sp->var, sp->varSize);
		}
	}

	/* Attach to PV system */
	pvSysAttach(sp->pvSys);

	/* Initiate connect & monitor requests to database channels, waiting
	   for all connections to be established if the option is set. */
	if (seq_connect(sp, optTest(sp, OPT_CONN) != pvStatOK))
		goto exit;

	/* Emulate the 'first monitor event' for anonymous PVs */
	if (optTest(sp, OPT_SAFE))
	{
		unsigned nch;
		for (nch=0; nch<sp->numChans; nch++)
			if (sp->chan[nch].syncedTo && !sp->chan[nch].dbch)
				seq_efSet(sp->ss, sp->chan[nch].syncedTo);
	}

	/* Call program entry function if defined.
	   Treat as if called from 1st state set. */
	if (sp->entryFunc) sp->entryFunc(sp->ss);

	/* Create each additional state set task (additional state set thread
	   names are derived from the first ss) */
	epicsThreadGetName(sp->ss->threadId, threadName, sizeof(threadName));
	threadLen = strlen(threadName);
	for (nss = 1; nss < sp->numSS; nss++)
	{
		SSCB		*ss = sp->ss + nss;
		epicsThreadId	tid;

		/* Form thread name from program name + state set number */
		sprintf(threadName+threadLen, "_%d", nss);

		/* Spawn the task */
		tid = epicsThreadCreate(
			threadName,			/* thread name */
			sp->threadPriority,		/* priority */
			sp->stackSize,			/* stack size */
			ss_entry,			/* entry point */
			ss);				/* parameter */

		DEBUG("Spawning additional state set thread %p: \"%s\"\n", tid, threadName);
	}

	/* First state set jumps directly to entry point */
	ss_entry(sp->ss);

	DEBUG("   Wait for other state sets to exit\n");
	for (nss = 1; nss < sp->numSS; nss++)
	{
		SSCB *ss = sp->ss + nss;
		epicsEventMustWait(ss->dead);
	}

	/* Call program exit function if defined.
	   Treat as if called from 1st state set. */
	if (sp->exitFunc) sp->exitFunc(sp->ss);

exit:
	DEBUG("   Disconnect all channels\n");
	seq_disconnect(sp);
	DEBUG("   Remove program instance from list\n");
	seqDelProg(sp);

	errlogSevPrintf(errlogInfo,
		"Instance %d of sequencer program \"%s\" terminated\n",
		sp->instance, sp->progName);

	/* Free all allocated memory */
	seq_free(sp);
}

/*
 * ss_read_buffer_static() - static version of ss_read_buffer.
 * This is to enable inlining in the for loop in ss_read_all_buffer.
 */
static void ss_read_buffer_static(SSCB *ss, CHAN *ch, boolean dirty_only)
{
	char *val = valPtr(ch,ss);
	char *buf = bufPtr(ch);
	ptrdiff_t nch = chNum(ch);
	/* Must take dbCount for db channels, else we overwrite
	   elements we didn't get */
	size_t count = ch->dbch ? ch->dbch->dbCount : ch->count;
	size_t var_size = ch->type->size * count;

	if (!ss->dirty[nch] && dirty_only)
		return;

	epicsMutexMustLock(ch->varLock);

	DEBUG("ss %s: before read %s", ss->ssName, ch->varName);
	print_channel_value(DEBUG, ch, val);

	memcpy(val, buf, var_size);
	if (ch->dbch)
	{
		/* structure copy */
		ss->metaData[nch] = ch->dbch->metaData;
	}

	DEBUG("ss %s: after read %s", ss->ssName, ch->varName);
	print_channel_value(DEBUG, ch, val);

	ss->dirty[nch] = FALSE;

	epicsMutexUnlock(ch->varLock);
}

/*
 * ss_read_buffer() - Copy value and meta data
 * from shared buffer to state set local buffer
 * and reset corresponding dirty flag. Do this
 * only if dirty flag is set or dirty_only is FALSE.
 */
void ss_read_buffer(SSCB *ss, CHAN *ch, boolean dirty_only)
{
	ss_read_buffer_static(ss, ch, dirty_only);
}

/*
 * ss_read_all_buffer() - Call ss_read_buffer_static
 * for all channels.
 */
static void ss_read_all_buffer(PROG *sp, SSCB *ss)
{
	unsigned nch;

	for (nch = 0; nch < sp->numChans; nch++)
	{
		CHAN *ch = sp->chan + nch;
		/* Call static version so it gets inlined */
		ss_read_buffer_static(ss, ch, TRUE);
	}
}

/*
 * ss_read_all_buffer_selective() - Call ss_read_buffer_static
 * for all channels that are sync'ed to the given event flag.
 * NOTE: calling code must take sp->lock, as we traverse
 * the list of channels synced to this event flag.
 */
void ss_read_buffer_selective(PROG *sp, SSCB *ss, EF_ID ev_flag)
{
	CHAN *ch = sp->syncedChans[ev_flag];
	while (ch)
	{
		/* Call static version so it gets inlined */
		ss_read_buffer_static(ss, ch, TRUE);
		ch = ch->nextSynced;
	}
}

/*
 * ss_write_buffer() - Copy given value and meta data
 * to shared buffer. In safe mode, if dirtify is TRUE then
 * set dirty flag for each state set.
 */
void ss_write_buffer(CHAN *ch, void *val, PVMETA *meta, boolean dirtify)
{
	PROG *sp = ch->prog;
	char *buf = bufPtr(ch);		/* shared buffer */
	/* Must use dbCount for db channels, else we overwrite
	   elements we didn't get */
	size_t count = ch->dbch ? ch->dbch->dbCount : ch->count;
	size_t var_size = ch->type->size * count;
	ptrdiff_t nch = chNum(ch);
	unsigned nss;

	epicsMutexMustLock(ch->varLock);

	DEBUG("ss_write_buffer: before write %s", ch->varName);
	print_channel_value(DEBUG, ch, buf);

	memcpy(buf, val, var_size);
	if (ch->dbch && meta)
		/* structure copy */
		ch->dbch->metaData = *meta;

	DEBUG("ss_write_buffer: after write %s", ch->varName);
	print_channel_value(DEBUG, ch, buf);

	if (optTest(sp, OPT_SAFE) && dirtify)
		for (nss = 0; nss < sp->numSS; nss++)
			sp->ss[nss].dirty[nch] = TRUE;

	epicsMutexUnlock(ch->varLock);
}

/*
 * ss_entry() - Thread entry point for all state sets.
 * Provides the main loop for state set processing.
 */
static void ss_entry(void *arg)
{
	SSCB		*ss = (SSCB *)arg;
	PROG		*sp = ss->prog;

	/* Attach to PV system; was already done for the first state set */
	if (ss != sp->ss)
	{
		ss->threadId = epicsThreadGetIdSelf();
		createOrAttachPvSystem(sp);
	}

	/* Register this thread with the EPICS watchdog (no callback func) */
	taskwdInsert(ss->threadId, 0, 0);

	/* In safe mode, update local var buffer with global one before
	   entering the event loop. Must do this using
	   ss_read_all_buffer since CA and other state sets could
	   already post events resp. pvPut. */
	if (optTest(sp, OPT_SAFE))
		ss_read_all_buffer(sp, ss);

	/* Initial state is the first one */
	ss->currentState = 0;
	ss->nextState = -1;
	ss->prevState = -1;

	DEBUG("ss %s: entering main loop\n", ss->ssName);

	/*
	 * ============= Main loop ==============
	 */
	while (TRUE)
	{
		boolean	ev_trig;
		int	transNum = 0;	/* highest prio trans. # triggered */
		STATE	*st = ss->states + ss->currentState;
		double	now;

		/* Set state to current state */
		assert(ss->currentState >= 0);

		/* Set state set event mask to this state's event mask */
		ss->mask = st->eventMask;

		/* If we've changed state, do any entry actions. Also do these
		 * even if it's the same state if option to do so is enabled.
		 */
		if (st->entryFunc && (ss->prevState != ss->currentState
			|| optTest(st, OPT_DOENTRYFROMSELF)))
		{
			st->entryFunc(ss);
		}

		/* Flush any outstanding DB requests */
		pvSysFlush(sp->pvSys);

		/* Setting this semaphore here guarantees that a when() is
		 * always executed at least once when a state is first entered.
		 */
		epicsEventSignal(ss->syncSem);

		pvTimeGetCurrentDouble(&now);

		/* Set time we entered this state if transition from a different
		 * state or else if option not to do so is off for this state.
		 */
		if ((ss->currentState != ss->prevState) ||
			!optTest(st, OPT_NORESETTIMERS))
		{
			ss->timeEntered = now;
		}
		ss->wakeupTime = epicsINF;

		/* Loop until an event is triggered, i.e. when() returns TRUE
		 */
		do {
			/* Wake up on PV event, event flag, or expired delay */
			DEBUG("before epicsEventWaitWithTimeout(ss=%d,timeout=%f)\n",
				ss - sp->ss, ss->wakeupTime - now);
			epicsEventWaitWithTimeout(ss->syncSem, ss->wakeupTime - now);
			DEBUG("after epicsEventWaitWithTimeout()\n");

			/* Check whether we have been asked to exit */
			if (sp->die) goto exit;

			/* Copy dirty variable values from CA buffer
			 * to user (safe mode only).
			 */
			if (optTest(sp, OPT_SAFE))
				ss_read_all_buffer(sp, ss);

			ss->wakeupTime = epicsINF;

			/* Check state change conditions */
			ev_trig = st->eventFunc(ss,
				&transNum, &ss->nextState);

			/* Clear all event flags (old ef mode only) */
			if (ev_trig && !optTest(sp, OPT_NEWEF))
			{
				unsigned i;
				for (i = 0; i < NWORDS(sp->numEvFlags); i++)
				{
					sp->evFlags[i] &= ~ss->mask[i];
				}
			}
			if (!ev_trig)
				pvTimeGetCurrentDouble(&now);
		} while (!ev_trig);

		/* Execute the state change action */
		st->actionFunc(ss, transNum, &ss->nextState);

		/* Check whether we have been asked to exit */
		if (sp->die) goto exit;

		/* If changing state, do exit actions */
		if (st->exitFunc && (ss->currentState != ss->nextState
			|| optTest(st, OPT_DOEXITTOSELF)))
		{
			st->exitFunc(ss);
		}

		/* Change to next state */
		ss->prevState = ss->currentState;
		ss->currentState = ss->nextState;
	}

	/* Thread exit has been requested */
exit:
	taskwdRemove(ss->threadId);
	/* Declare ourselves dead */
	if (ss != sp->ss)
		epicsEventSignal(ss->dead);
}

/*
 * Delete all state set threads and do general clean-up.
 */
epicsShareFunc void epicsShareAPI seqStop(epicsThreadId tid)
{
	PROG	*sp;

	/* Check that this is indeed a state program thread */
	sp = seqFindProg(tid);
	if (sp == NULL)
		return;
	seq_exit(sp->ss);
}

/*
 * ss_wakeup() -- wake up each state set that is waiting on this event
 * based on the current event mask; eventNum = 0 means wake all state sets.
 */
void ss_wakeup(PROG *sp, unsigned eventNum)
{
	unsigned nss;

	/* Check event number against mask for all state sets: */
	for (nss = 0; nss < sp->numSS; nss++)
	{
		SSCB *ss = sp->ss + nss;

		epicsMutexMustLock(sp->lock);
		/* If event bit in mask is set, wake that state set */
		DEBUG("ss_wakeup: eventNum=%d, mask=%u, state set=%d\n", eventNum, 
			ss->mask? *ss->mask : 0, (int)ssNum(ss));
		if (eventNum == 0 || 
			(ss->mask && bitTest(ss->mask, eventNum)))
		{
			DEBUG("ss_wakeup: waking up state set=%d\n", (int)ssNum(ss));
			epicsEventSignal(ss->syncSem); /* wake up ss thread */
		}
		epicsMutexUnlock(sp->lock);
	}
}
