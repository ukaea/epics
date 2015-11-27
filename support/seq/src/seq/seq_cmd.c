/*************************************************************************\
Copyright (c) 2010-2015 Helmholtz-Zentrum Berlin f. Materialien
                        und Energie GmbH, Germany (HZB)
This file is distributed subject to a Software License Agreement found
in the file LICENSE that is included with this distribution.
\*************************************************************************/
/* EPICS sequencer commands
 *
 * Author:  Eric Norum
 *
 * Experimental Physics and Industrial Control System (EPICS)
 *
 * Initial development by:
 *    Canadian Light Source
 *    107 North Road
 *    University of Saskatchewan
 *    Saskatoon, Saskatchewan, CANADA
 *    cls.usask.ca
 */
#include "seq.h"

struct sequencerProgram {
    seqProgram *prog;
    struct program_instance *instances;
    struct sequencerProgram *next;
};

/* These are the only global variables in the whole seq library. */
static struct
{
    epicsMutexId lock;
    struct sequencerProgram *programs;
    pvSystem pvSys;
} globals;

static void seqInitPvt(void *arg)
{
    globals.lock = epicsMutexCreate();
    if (!globals.lock) {
        errlogSevPrintf(errlogFatal, "seqInitPvt: epicsMutexCreate failed\n");
        exit(EXIT_FAILURE);
    }
}

static void seqLazyInit()
{
    static epicsThreadOnceId seqOnceFlag = EPICS_THREAD_ONCE_INIT;
    epicsThreadOnce(&seqOnceFlag, seqInitPvt, NULL);
}

void createOrAttachPvSystem(struct program_instance *sp)
{
    seqLazyInit();
    epicsMutexMustLock(globals.lock);
    if (!pvSysIsDefined(globals.pvSys)) {
        pvStat status = pvSysCreate(&globals.pvSys);
        if (status != pvStatOK) {
            errlogPrintf("getPvSystem: pvSysCreate() failure\n");
        }
    } else {
        pvSysAttach(globals.pvSys);
    }
    sp->pvSys = globals.pvSys;
    epicsMutexUnlock(globals.lock);
}

epicsShareFunc void seqRegisterSequencerProgram(seqProgram *prog)
{
    struct sequencerProgram *sp = NULL;

    seqLazyInit();
    epicsMutexMustLock(globals.lock);
    foreach(sp, globals.programs) {
        if (sp->prog == prog) {
            break;
        }
    }
    if (!sp) {
        sp = (struct sequencerProgram *)malloc(sizeof *sp);
        if (!sp) {
            errlogSevPrintf(errlogFatal, "seqRegisterSequencerProgram: out of memory");
        }
        sp->prog = prog;
        sp->next = globals.programs;
        sp->instances = NULL;
        globals.programs = sp;
    }
    epicsMutexUnlock(globals.lock);
}

int traverseSequencerPrograms(sequencerProgramTraversee *traversee, void *param)
{
    struct sequencerProgram *sp;
    int stop = FALSE;

    seqLazyInit();
    epicsMutexMustLock(globals.lock);
    foreach(sp, globals.programs) {
        stop = traversee(&sp->instances, sp->prog, param);
        if (stop) break;
    }
    /* call one last time to indicate that list was exhausted */
    if (!stop) {
        stop = traversee(NULL, NULL, param);
    }
    epicsMutexUnlock(globals.lock);
    return stop;
}

/*
 * Find a thread by name or ID number
 */
static epicsThreadId findThread(const char *name)
{
    epicsThreadId id;
    char *term;

    id = (epicsThreadId)strtoul(name, &term, 16);
    if ((term != name) && (*term == '\0'))
        return id;
    id = epicsThreadGetId(name);
    if (id)
        return id;
    printf("No such thread.\n");
    return NULL;
}

/* seq */
static const iocshArg seqArg0 = { "program/threadID",iocshArgString};
static const iocshArg seqArg1 = { "macro definitions",iocshArgString};
static const iocshArg seqArg2 = { "stack size",iocshArgInt};
static const iocshArg * const seqArgs[3] = { &seqArg0,&seqArg1,&seqArg2 };
static const iocshFuncDef seqFuncDef = {"seq",3,seqArgs};
static void seqCallFunc(const iocshArgBuf *args)
{
    char *table = args[0].sval;
    char *macroDef = args[1].sval;
    int stackSize = args[2].ival;
    struct sequencerProgram *sp;

    if (!table) {
        printf("No sequencer specified.\n");
        return;
    }
    if (stackSize < 0) {
        errlogSevPrintf(errlogFatal, "3rd argument of seq must be a positive integer");
        return;
    }
    if (*table == '&')
        table++;
    seqLazyInit();
    epicsMutexMustLock(globals.lock);
    foreach(sp, globals.programs) {
        if (!strcmp(table, sp->prog->progName)) {
            break;
        }
    }
    epicsMutexUnlock(globals.lock);
    if (sp) {
        seq(sp->prog, macroDef, (unsigned)stackSize);
    } else {
        printf("Can't find sequencer `%s'.\n", table);
    }
}

/* seqShow */
static const iocshArg seqShowArg0 = { "program/threadID",iocshArgString};
static const iocshArg * const seqShowArgs[1] = {&seqShowArg0};
static const iocshFuncDef seqShowFuncDef = {"seqShow",1,seqShowArgs};
static void seqShowCallFunc(const iocshArgBuf *args)
{
    epicsThreadId id;
    char *name = args[0].sval;

    if (name == NULL)
        seqShow(NULL);
    else if ((id = findThread(name)) != NULL)
        seqShow(id);
}

/* seqQueueShow */
static const iocshArg seqQueueShowArg0 = { "program/threadID",iocshArgString};
static const iocshArg * const seqQueueShowArgs[1] = {&seqQueueShowArg0};
static const iocshFuncDef seqQueueShowFuncDef = {"seqQueueShow",1,seqQueueShowArgs};
static void seqQueueShowCallFunc(const iocshArgBuf *args)
{
    epicsThreadId id;
    char *name = args[0].sval;

    if ((name != NULL) && ((id = findThread(name)) != NULL))
        seqQueueShow(id);
    else {
        printf("No sequencer task specified.\n");
        seqShow(NULL);
    }
}

/* seqStop */
static const iocshArg seqStopArg0 = { "program/threadID",iocshArgString};
static const iocshArg * const seqStopArgs[1] = {&seqStopArg0};
static const iocshFuncDef seqStopFuncDef = {"seqStop",1,seqStopArgs};
static void seqStopCallFunc(const iocshArgBuf *args)
{
    epicsThreadId id;
    char *name = args[0].sval;

    if ((name != NULL) && ((id = findThread(name)) != NULL))
        seqStop(id);
    else {
        printf("No sequencer task specified.\n");
        seqShow(NULL);
    }
}

/* seqChanShow */
static const iocshArg seqChanShowArg0 = { "program/threadID",iocshArgString};
static const iocshArg seqChanShowArg1 = { "channel",iocshArgString};
static const iocshArg * const seqChanShowArgs[2] = {&seqChanShowArg0,&seqChanShowArg1};
static const iocshFuncDef seqChanShowFuncDef = {"seqChanShow",2,seqChanShowArgs};
static void seqChanShowCallFunc(const iocshArgBuf *args)
{
    epicsThreadId id;
    char *name = args[0].sval;
    char *chan = args[1].sval;

    if ((name != NULL) && ((id = findThread(name)) != NULL))
        seqChanShow(id, chan);
    else {
        printf("No sequencer task specified.\n");
        seqShow(NULL);
    }
}

/* seqcar */
static const iocshArg seqcarArg0 = { "verbosity",iocshArgInt};
static const iocshArg * const seqcarArgs[1] = {&seqcarArg0};
static const iocshFuncDef seqcarFuncDef = {"seqcar",1,seqcarArgs};
static void seqcarCallFunc(const iocshArgBuf *args)
{
    seqcar(args[0].ival);
}

/*
 * This routine is called before multitasking has started, so there's
 * no race condition in the test/set of firstTime.
 */
epicsShareFunc void seqRegisterSequencerCommands(void)
{
    static int firstTime = 1;
    if (firstTime) {
        firstTime = 0;
        iocshRegister(&seqFuncDef,seqCallFunc);
        iocshRegister(&seqShowFuncDef,seqShowCallFunc);
        iocshRegister(&seqQueueShowFuncDef,seqQueueShowCallFunc);
        iocshRegister(&seqStopFuncDef,seqStopCallFunc);
        iocshRegister(&seqChanShowFuncDef,seqChanShowCallFunc);
        iocshRegister(&seqcarFuncDef,seqcarCallFunc);
    }
}
