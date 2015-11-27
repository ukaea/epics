/*************************************************************************\
Copyright (c) 2010-2015 Helmholtz-Zentrum Berlin f. Materialien
                        und Energie GmbH, Germany (HZB)
This file is distributed subject to a Software License Agreement found
in the file LICENSE that is included with this distribution.
\*************************************************************************/
#include <stdlib.h>

#include "epicsThread.h"
#include "epicsEvent.h"
#include "epicsExit.h"
#include "seqCom.h"

#include "../testSupport.h"

static epicsEventId this_test_done;
static seqProgram *prog;
int seq_test_adapt_priority;

static int doit(void)
{
    seq(prog,0,0);
    return 0;
}

void run_seq_test(seqProgram *seqProg, const char *name, int adapt_priority)
{
    seq_test_adapt_priority = adapt_priority;
    if (!this_test_done) {
        this_test_done = epicsEventMustCreate(epicsEventEmpty);
    }
    prog = seqProg;
    runTestFunc(name, doit);
    epicsEventWait(this_test_done);
    epicsThreadSleep(1.0);
}

void seq_test_init(int num_tests)
{
    testPlan(num_tests);
    if (seq_test_adapt_priority > 0) {
        epicsThreadSetPriority(epicsThreadGetIdSelf(), epicsThreadPriorityHigh);
    } else if (seq_test_adapt_priority < 0) {
        epicsThreadSetPriority(epicsThreadGetIdSelf(), epicsThreadPriorityLow);
    }
}

void seq_test_at_thread_exit(void *dummy)
{
    epicsExit(EXIT_SUCCESS);
}

void seq_test_done(void)
{
    epicsThreadSetPriority(epicsThreadGetIdSelf(), epicsThreadPriorityMedium);
    testDone();
#if defined(vxWorks)
    epicsEventSignal(this_test_done);
#else
    epicsAtThreadExit(seq_test_at_thread_exit, 0);
#endif
}
