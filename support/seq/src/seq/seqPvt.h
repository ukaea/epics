/*************************************************************************\
Copyright (c) 1991-1993 The Regents of the University of California
                        and the University of Chicago.
                        Los Alamos National Laboratory
Copyright (c) 2010-2015 Helmholtz-Zentrum Berlin f. Materialien
                        und Energie GmbH, Germany (HZB)
This file is distributed subject to a Software License Agreement found
in the file LICENSE that is included with this distribution.
\*************************************************************************/
/*	Internal common definitions for the run-time sequencer library
 *
 *      Author:         Andy Kozubal
 *      Date:           
 *
 *      Experimental Physics and Industrial Control System (EPICS)
 *
 *      This software was produced under  U.S. Government contracts:
 *      (W-7405-ENG-36) at the Los Alamos National Laboratory,
 *      and (W-31-109-ENG-38) at Argonne National Laboratory.
 *
 *      Initial development by:
 *              The Controls and Automation Group (AT-8)
 *              Ground Test Accelerator
 *              Accelerator Technology Division
 *              Los Alamos National Laboratory
 *
 *      Co-developed with
 *              The Controls and Computing Group
 *              Accelerator Systems Division
 *              Advanced Photon Source
 *              Argonne National Laboratory
 */
#ifndef	INCLseqPvth
#define	INCLseqPvth

#include "seq_snc.h"
#define boolean seqBool
#define bitMask seqMask

#include "seq_queue.h"

#define valPtr(ch,ss)		((char*)(ss)->var+(ch)->offset)
#define bufPtr(ch)		((char*)(ch)->prog->var+(ch)->offset)

#define ssNum(ss)		((ss)-(ss)->prog->ss)
#define chNum(ch)		((ch)-(ch)->prog->chan)

#define metaPtr(ch,ss) (			\
	(ch)->dbch				\
	?(optTest((ch)->prog,OPT_SAFE)		\
		?(ss)->metaData + chNum(ch)	\
		:&(ch)->dbch->metaData)		\
	:0					\
)

#define optTest(sp,opt)		(((sp)->options & (opt)) != 0)
					/* test if opt is set in program instance sp */

/* Generic iteration on lists */
#define foreach(e,l)		for (e = l; e != 0; e = e->next)

/* Generic min and max */
#ifndef min
#define min(x, y)		(((x) < (y)) ? (x) : (y))
#endif

#ifndef max
#define max(x, y)		(((x) < (y)) ? (y) : (x))
#endif

#define free(p)			{DEBUG("%s:%d:free(%p)\n",__FILE__,__LINE__,p); if(p){free(p); p=0;}}

/* Generic allocation */
#define newArray(type,count)	(DEBUG("%s:%d:calloc(%u,%u)\n",__FILE__,__LINE__,count,sizeof(type)),(type *)calloc(count, sizeof(type)))
#define new(type)		newArray(type,1)

typedef struct db_channel	DBCHAN;
typedef struct channel		CHAN;
typedef seqState		STATE;
typedef struct macro		MACRO;
typedef struct state_set	SSCB;
typedef struct program_instance	PROG;
typedef struct pvreq		PVREQ;
typedef const struct pv_type	PVTYPE;
typedef struct pv_meta_data	PVMETA;

typedef struct seqg_vars        SEQ_VARS;

/* Channel, i.e. an assigned variable */
struct channel
{
	/* static channel data (assigned once on startup) */
	size_t		offset;		/* offset to value (e.g. in prog->var) */
	const char	*varName;	/* variable name */
	unsigned	count;		/* number of elements in array */
	unsigned	eventNum;	/* event number */
	PVTYPE		*type;		/* request type info */
	PROG		*prog;		/* state program that owns this struct*/

	/* dynamic channel data (assigned at runtime) */
	DBCHAN		*dbch;		/* channel assigned to a named db pv */
	EF_ID		syncedTo;	/* event flag id if synced */
	CHAN		*nextSynced;	/* next channel synced to same flag */
	QUEUE		queue;		/* queue if queued */
	boolean		monitored;	/* whether channel is monitored */
	/* buffer access, only used in safe mode */
	epicsMutexId	varLock;	/* mutex for locking access to shared
					   var buffer and meta data */
};

struct pv_type
{
	enum prim_type_tag tag;
	pvType		putType;
	pvType		getType;
	size_t		size;
};

/* Meta data received from pv layer per request/monitor */
struct pv_meta_data
{
	epicsTimeStamp	timeStamp;	/* time stamp */
	pvStat		status;		/* status code */
	pvSevr		severity;	/* severity code */
	const char	*message;	/* error message */
};

/* Channel assigned to a named (database) pv */
struct db_channel
{
	char		*dbName;	/* channel name after macro expansion */
	pvVar		pvid;		/* PV (process variable) id */
	unsigned	dbCount;	/* actual count for db access */
	boolean		connected;	/* whether channel is connected */
	boolean		gotMonitor;	/* whether we got a monitor after connect */
	PVMETA		metaData;	/* meta data (shared buffer) */
};

struct state_set
{
	SEQ_VARS	*var;		/* variable value block */

	/* static state set data (assigned once on startup) */
	const char	*ssName;	/* state set name (for debugging) */
	epicsThreadId	threadId;	/* thread id */
	unsigned	numStates;	/* number of states */
	STATE		*states;	/* ptr to array of state blocks */
	PROG		*prog;		/* ptr back to state program block */

	/* dynamic state set data (assigned at runtime) */
	int		currentState;	/* current state index, -1 if none */
	int		nextState;	/* next state index, -1 if none */
	int		prevState;	/* previous state index, -1 if none */
	const bitMask	*mask;		/* current event mask */
	double		timeEntered;	/* time that current state was entered */
	double		wakeupTime;	/* next time state set should wake up */
	epicsEventId	syncSem;	/* semaphore for event sync */
	epicsEventId	dead;		/* event to signal state set exit done */
	/* these are arrays, one for each channel */
	PVREQ		**getReq;	/* currently pending get requests */
	PVREQ		**putReq;	/* currently pending put requests */
	PVMETA		*metaData;	/* meta data (safe mode) */
	/* safe mode */
	boolean		*dirty;		/* array of flags, one for each channel */
};

STATIC_ASSERT(offsetof(struct state_set,var)==0);

struct program_instance
{
	SEQ_VARS	*var;		/* user variable area (shared buffer) */

	/* static program data (assigned once on startup) */
	const char	*progName;	/* program name (for messages) */
	int		instance;	/* program instance number */
	unsigned	threadPriority;	/* thread priority (all threads) */
	unsigned	stackSize;	/* stack size (all threads) */
	pvSystem	pvSys;		/* pv system handle */
	CHAN		*chan;		/* table of channels */
	unsigned	numChans;	/* number of channels */
	QUEUE		*queues;	/* array of syncQ queues */
	unsigned	numQueues;	/* number of syncQ queues */
	SSCB		*ss;		/* array of state set control blocks */
	unsigned	numSS;		/* number of state sets */
	size_t		varSize;	/* size of user variable area */
	MACRO		*macros;	/* ptr to macro table */
	char		*params;	/* program parameters */
	unsigned	options;	/* options (bit-encoded) */
	SEQ_PROG_FUNC	*initFunc;	/* init function */
	SEQ_SS_FUNC	*entryFunc;	/* entry function */
	SEQ_SS_FUNC	*exitFunc;	/* exit function */
	unsigned	numEvFlags;	/* number of event flags */

	/* dynamic program data (assigned at runtime) */
	epicsMutexId	lock;	/* mutex for locking dynamic program data */
	/* the following five members must always be protected by lock */
	bitMask		*evFlags;	/* event bits for event flags & channels */
	CHAN		**syncedChans;	/* for each event flag, start of synced list */
	unsigned	assignCount;	/* number of channels assigned to ext. pv */
	unsigned	connectCount;	/* number of channels connected */
	unsigned	monitorCount;	/* number of channels monitored */
	unsigned	gotMonitorCount;/* number of monitored channels that got
					   a monitor event */

	void		*pvReqPool;	/* freeList for pv requests (has own lock) */
	boolean		die;		/* flag set when seqStop is called */
	epicsEventId	ready;		/* all channels connected & got 1st monitor */
	epicsEventId	dead;		/* event to signal exit of main thread done */
	PROG		*next;		/* next element in program list */
};

STATIC_ASSERT(offsetof(struct program_instance,var)==0);

/* Request data for pvPut and pvGet */
struct pvreq
{
	CHAN		*ch;		/* requested variable */
	SSCB		*ss;		/* state set that made the request */
};

/* Thread parameters */
#define THREAD_NAME_SIZE	32
#define THREAD_STACK_SIZE	epicsThreadStackBig
#define THREAD_PRIORITY		epicsThreadPriorityMedium

/* Internal procedures */

/* seq_task.c */
void sequencer(void *arg);
void ss_write_buffer(CHAN *ch, void *val, PVMETA *meta, boolean dirtify);
void ss_read_buffer(SSCB *ss, CHAN *ch, boolean dirty_only);
void ss_read_buffer_selective(PROG *sp, SSCB *ss, EF_ID ev_flag);
void ss_wakeup(PROG *sp, unsigned eventNum);

/* seq_mac.c */
void seqMacParse(PROG *sp, const char *macStr);
char *seqMacValGet(PROG *sp, const char *name);
void seqMacEval(PROG *sp, const char *inStr, char *outStr, size_t maxChar);
void seqMacFree(PROG *sp);

/* seq_ca.c */
pvConnFunc seq_conn_handler;
pvEventFunc seq_event_handler;
pvStat seq_connect(PROG *sp, boolean wait);
void seq_disconnect(PROG *sp);
pvStat seq_camonitor(CHAN *ch, boolean on);

/* seq_prog.c */
typedef int seqTraversee(PROG *prog, void *param);
void seqTraverseProg(seqTraversee *func, void *param);
SSCB *seqFindStateSet(epicsThreadId threadId);
PROG *seqFindProg(epicsThreadId threadId);
void seqDelProg(PROG *sp);
void seqAddProg(PROG *sp);

/* seqCommands.c */
typedef int sequencerProgramTraversee(PROG **prog, seqProgram *pseq, void *param);
int traverseSequencerPrograms(sequencerProgramTraversee *traversee, void *param);
void createOrAttachPvSystem(PROG *sp);

/* seq_main.c */
void seq_free(PROG *sp);

/* debug/query support */
typedef int pr_fun(const char *format,...);
void print_channel_value(pr_fun *, CHAN *ch, void *val);

#endif	/*INCLseqPvth*/
