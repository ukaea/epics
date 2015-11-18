/*************************************************************************\
Copyright (c) 1991      The Regents of the University of California
                        and the University of Chicago.
                        Los Alamos National Laboratory
Copyright (c) 2010-2015 Helmholtz-Zentrum Berlin f. Materialien
                        und Energie GmbH, Germany (HZB)
This file is distributed subject to a Software License Agreement found
in the file LICENSE that is included with this distribution.
\*************************************************************************/
/*************************************************************************\
                        Program list management
\*************************************************************************/
#include "seq.h"
#include "seq_debug.h"

/*
 * seqFindProg() - find a program in the state program list from thread id.
 */
PROG *seqFindProg(epicsThreadId threadId)
{
    SSCB *ss = seqFindStateSet(threadId);
    return ss ? ss->prog : NULL;
}

struct findStateSetArgs {
    SSCB *ss;
    epicsThreadId threadId;
};

static int findStateSet(PROG *sp, void *param)
{
    struct findStateSetArgs *pargs = (struct findStateSetArgs *)param;
    unsigned nss;

    for (nss = 0; nss < sp->numSS; nss++) {
        SSCB *ss = sp->ss + nss;

        DEBUG("findStateSet trying %s[%d] ss[%d].threadId=%p\n",
            sp->progName, sp->instance, nss, ss->threadId);
        if (ss->threadId == pargs->threadId) {
            pargs->ss = ss;
            return TRUE;
        }
    }
    return FALSE;
}

/*
 * seqFindStateSet() - find a state set in the program list from thread id.
 */
SSCB *seqFindStateSet(epicsThreadId threadId)
{
    struct findStateSetArgs args;

    args.ss = 0;
    args.threadId = threadId;
    seqTraverseProg(findStateSet, &args);
    return args.ss;
}

struct traverseInstancesArgs {
    seqTraversee *func;
    void *param;
};

static int traverseInstances(PROG **ppInstances, seqProgram *pseq, void *param)
{
    struct traverseInstancesArgs *pargs = (struct traverseInstancesArgs *)param;
    PROG *sp;
    if (!ppInstances) return FALSE;
    foreach(sp, *ppInstances) {
        if (pargs->func(sp, pargs->param))
            return TRUE;    /* terminate traversal */
    }
    return FALSE;           /* continue traversal */
}

/*
 * seqTraverseProg() - visit all existing program instances and
 * call the specified routine or function.  Passes one parameter of
 * pointer size.
 */
void seqTraverseProg(seqTraversee * func, void *param)
{
    struct traverseInstancesArgs args;
    args.func = func;
    args.param = param;
    traverseSequencerPrograms(traverseInstances, &args);
}

static int addProg(PROG **ppInstances, seqProgram *pseq, void *param)
{
    PROG *sp = (PROG *)param;

    if (!ppInstances || !pseq) {
        return FALSE;
    }
    /* same program name */
    if (strcmp(sp->progName, pseq->progName) == 0) {
        PROG *curSP, *lastSP = NULL;
        int instance = -1;

        /* determine maximum instance number for this program */
        foreach(curSP, *ppInstances) {
            lastSP = curSP;
            /* check precondition */
            assert(curSP != sp);
            instance = max(curSP->instance, instance);
        }
        sp->instance = instance + 1;
        if (lastSP) {
            lastSP->next = sp;
        } else {
            *ppInstances = sp;
        }
        DEBUG("Added program %p, instance %d to instance list.\n",
            sp, sp->instance);
        return TRUE;        /* terminate traversal */
    }
    return FALSE;           /* continue traversal */
}

/*
 * seqAddProg() - add a program to the program instance list.
 * Precondition: must not be already in the list.
 */
void seqAddProg(PROG *sp)
{
    traverseSequencerPrograms(addProg, sp);
}

static int delProg(PROG **ppInstances, seqProgram *pseq, void *param)
{
    PROG *sp = (PROG *)param;

    if (!ppInstances || !pseq)
        return FALSE;
    if (strcmp(sp->progName, pseq->progName) == 0) {
        PROG *curSP;

        if (*ppInstances == sp) {
            *ppInstances = sp->next;
            DEBUG("Deleted program %p, instance %d from instance list.\n", sp, sp->instance);
            return TRUE;
        }
        foreach(curSP, *ppInstances) {
            if (curSP->next == sp) {
                curSP->next = sp->next;
                DEBUG("Deleted program %p, instance %d from instance list.\n", sp, sp->instance);
                return TRUE;
            }
        }
    }
    return FALSE;
}

/*
 * seqDelProg() - delete a program from the program instance list.
 */
void seqDelProg(PROG *sp)
{
    traverseSequencerPrograms(delProg, sp);
}
