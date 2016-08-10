/* THIS IS A GENERATED FILE. DO NOT EDIT! */
/* Generated from ../O.Common/streamApp.dbd */

#include <string.h>

#include "epicsStdlib.h"
#include "iocsh.h"
#include "iocshRegisterCommon.h"
#include "registryCommon.h"

extern "C" {

epicsShareExtern rset *pvar_rset_aSubRSET, *pvar_rset_aaiRSET,
    *pvar_rset_aaoRSET, *pvar_rset_aiRSET, *pvar_rset_aoRSET,
    *pvar_rset_biRSET, *pvar_rset_boRSET, *pvar_rset_calcRSET,
    *pvar_rset_calcoutRSET, *pvar_rset_compressRSET,
    *pvar_rset_dfanoutRSET, *pvar_rset_eventRSET, *pvar_rset_fanoutRSET,
    *pvar_rset_histogramRSET, *pvar_rset_longinRSET,
    *pvar_rset_longoutRSET, *pvar_rset_lsiRSET, *pvar_rset_lsoRSET,
    *pvar_rset_mbbiRSET, *pvar_rset_mbbiDirectRSET, *pvar_rset_mbboRSET,
    *pvar_rset_mbboDirectRSET, *pvar_rset_permissiveRSET,
    *pvar_rset_printfRSET, *pvar_rset_selRSET, *pvar_rset_seqRSET,
    *pvar_rset_stateRSET, *pvar_rset_stringinRSET,
    *pvar_rset_stringoutRSET, *pvar_rset_subRSET, *pvar_rset_subArrayRSET,
    *pvar_rset_waveformRSET;

typedef int (*rso_func)(dbRecordType *pdbRecordType);
epicsShareExtern rso_func pvar_func_aSubRecordSizeOffset,
    pvar_func_aaiRecordSizeOffset, pvar_func_aaoRecordSizeOffset,
    pvar_func_aiRecordSizeOffset, pvar_func_aoRecordSizeOffset,
    pvar_func_biRecordSizeOffset, pvar_func_boRecordSizeOffset,
    pvar_func_calcRecordSizeOffset, pvar_func_calcoutRecordSizeOffset,
    pvar_func_compressRecordSizeOffset, pvar_func_dfanoutRecordSizeOffset,
    pvar_func_eventRecordSizeOffset, pvar_func_fanoutRecordSizeOffset,
    pvar_func_histogramRecordSizeOffset, pvar_func_longinRecordSizeOffset,
    pvar_func_longoutRecordSizeOffset, pvar_func_lsiRecordSizeOffset,
    pvar_func_lsoRecordSizeOffset, pvar_func_mbbiRecordSizeOffset,
    pvar_func_mbbiDirectRecordSizeOffset, pvar_func_mbboRecordSizeOffset,
    pvar_func_mbboDirectRecordSizeOffset,
    pvar_func_permissiveRecordSizeOffset,
    pvar_func_printfRecordSizeOffset, pvar_func_selRecordSizeOffset,
    pvar_func_seqRecordSizeOffset, pvar_func_stateRecordSizeOffset,
    pvar_func_stringinRecordSizeOffset,
    pvar_func_stringoutRecordSizeOffset, pvar_func_subRecordSizeOffset,
    pvar_func_subArrayRecordSizeOffset, pvar_func_waveformRecordSizeOffset;

static const char * const recordTypeNames[] = {
    "aSub", "aai", "aao", "ai", "ao", "bi", "bo", "calc", "calcout",
    "compress", "dfanout", "event", "fanout", "histogram", "longin",
    "longout", "lsi", "lso", "mbbi", "mbbiDirect", "mbbo", "mbboDirect",
    "permissive", "printf", "sel", "seq", "state", "stringin",
    "stringout", "sub", "subArray", "waveform"
};

static const recordTypeLocation rtl[] = {
    {pvar_rset_aSubRSET, pvar_func_aSubRecordSizeOffset},
    {pvar_rset_aaiRSET, pvar_func_aaiRecordSizeOffset},
    {pvar_rset_aaoRSET, pvar_func_aaoRecordSizeOffset},
    {pvar_rset_aiRSET, pvar_func_aiRecordSizeOffset},
    {pvar_rset_aoRSET, pvar_func_aoRecordSizeOffset},
    {pvar_rset_biRSET, pvar_func_biRecordSizeOffset},
    {pvar_rset_boRSET, pvar_func_boRecordSizeOffset},
    {pvar_rset_calcRSET, pvar_func_calcRecordSizeOffset},
    {pvar_rset_calcoutRSET, pvar_func_calcoutRecordSizeOffset},
    {pvar_rset_compressRSET, pvar_func_compressRecordSizeOffset},
    {pvar_rset_dfanoutRSET, pvar_func_dfanoutRecordSizeOffset},
    {pvar_rset_eventRSET, pvar_func_eventRecordSizeOffset},
    {pvar_rset_fanoutRSET, pvar_func_fanoutRecordSizeOffset},
    {pvar_rset_histogramRSET, pvar_func_histogramRecordSizeOffset},
    {pvar_rset_longinRSET, pvar_func_longinRecordSizeOffset},
    {pvar_rset_longoutRSET, pvar_func_longoutRecordSizeOffset},
    {pvar_rset_lsiRSET, pvar_func_lsiRecordSizeOffset},
    {pvar_rset_lsoRSET, pvar_func_lsoRecordSizeOffset},
    {pvar_rset_mbbiRSET, pvar_func_mbbiRecordSizeOffset},
    {pvar_rset_mbbiDirectRSET, pvar_func_mbbiDirectRecordSizeOffset},
    {pvar_rset_mbboRSET, pvar_func_mbboRecordSizeOffset},
    {pvar_rset_mbboDirectRSET, pvar_func_mbboDirectRecordSizeOffset},
    {pvar_rset_permissiveRSET, pvar_func_permissiveRecordSizeOffset},
    {pvar_rset_printfRSET, pvar_func_printfRecordSizeOffset},
    {pvar_rset_selRSET, pvar_func_selRecordSizeOffset},
    {pvar_rset_seqRSET, pvar_func_seqRecordSizeOffset},
    {pvar_rset_stateRSET, pvar_func_stateRecordSizeOffset},
    {pvar_rset_stringinRSET, pvar_func_stringinRecordSizeOffset},
    {pvar_rset_stringoutRSET, pvar_func_stringoutRecordSizeOffset},
    {pvar_rset_subRSET, pvar_func_subRecordSizeOffset},
    {pvar_rset_subArrayRSET, pvar_func_subArrayRecordSizeOffset},
    {pvar_rset_waveformRSET, pvar_func_waveformRecordSizeOffset}
};

epicsShareExtern dset *pvar_dset_devAaiSoft, *pvar_dset_devaaiStream,
    *pvar_dset_devAaoSoft, *pvar_dset_devaaoStream, *pvar_dset_devAiSoft,
    *pvar_dset_devAiSoftRaw, *pvar_dset_devAiSoftCallback,
    *pvar_dset_devTimestampAI, *pvar_dset_devAiGeneralTime,
    *pvar_dset_devaiStream, *pvar_dset_devAoSoft, *pvar_dset_devAoSoftRaw,
    *pvar_dset_devAoSoftCallback, *pvar_dset_devaoStream,
    *pvar_dset_devBiSoft, *pvar_dset_devBiSoftRaw,
    *pvar_dset_devBiSoftCallback, *pvar_dset_devBiDbState,
    *pvar_dset_devbiStream, *pvar_dset_devBoSoft, *pvar_dset_devBoSoftRaw,
    *pvar_dset_devBoSoftCallback, *pvar_dset_devBoGeneralTime,
    *pvar_dset_devBoDbState, *pvar_dset_devboStream,
    *pvar_dset_devCalcoutSoft, *pvar_dset_devCalcoutSoftCallback,
    *pvar_dset_devcalcoutStream, *pvar_dset_devEventSoft,
    *pvar_dset_devHistogramSoft, *pvar_dset_devLiSoft,
    *pvar_dset_devLiSoftCallback, *pvar_dset_devLiGeneralTime,
    *pvar_dset_devlonginStream, *pvar_dset_devLoSoft,
    *pvar_dset_devLoSoftCallback, *pvar_dset_devlongoutStream,
    *pvar_dset_devLsiSoft, *pvar_dset_devLsoSoft,
    *pvar_dset_devLsoSoftCallback, *pvar_dset_devLsoStdio,
    *pvar_dset_devMbbiSoft, *pvar_dset_devMbbiSoftRaw,
    *pvar_dset_devMbbiSoftCallback, *pvar_dset_devmbbiStream,
    *pvar_dset_devMbbiDirectSoft, *pvar_dset_devMbbiDirectSoftRaw,
    *pvar_dset_devMbbiDirectSoftCallback, *pvar_dset_devmbbiDirectStream,
    *pvar_dset_devMbboSoft, *pvar_dset_devMbboSoftRaw,
    *pvar_dset_devMbboSoftCallback, *pvar_dset_devmbboStream,
    *pvar_dset_devMbboDirectSoft, *pvar_dset_devMbboDirectSoftRaw,
    *pvar_dset_devMbboDirectSoftCallback, *pvar_dset_devmbboDirectStream,
    *pvar_dset_devPrintfSoft, *pvar_dset_devPrintfSoftCallback,
    *pvar_dset_devPrintfStdio, *pvar_dset_devSiSoft,
    *pvar_dset_devSiSoftCallback, *pvar_dset_devTimestampSI,
    *pvar_dset_devSiGeneralTime, *pvar_dset_devstringinStream,
    *pvar_dset_devSoSoft, *pvar_dset_devSoSoftCallback,
    *pvar_dset_devSoStdio, *pvar_dset_devstringoutStream,
    *pvar_dset_devSASoft, *pvar_dset_devWfSoft,
    *pvar_dset_devwaveformStream;

static const char * const deviceSupportNames[] = {
    "devAaiSoft", "devaaiStream", "devAaoSoft", "devaaoStream",
    "devAiSoft", "devAiSoftRaw", "devAiSoftCallback", "devTimestampAI",
    "devAiGeneralTime", "devaiStream", "devAoSoft", "devAoSoftRaw",
    "devAoSoftCallback", "devaoStream", "devBiSoft", "devBiSoftRaw",
    "devBiSoftCallback", "devBiDbState", "devbiStream", "devBoSoft",
    "devBoSoftRaw", "devBoSoftCallback", "devBoGeneralTime",
    "devBoDbState", "devboStream", "devCalcoutSoft",
    "devCalcoutSoftCallback", "devcalcoutStream", "devEventSoft",
    "devHistogramSoft", "devLiSoft", "devLiSoftCallback",
    "devLiGeneralTime", "devlonginStream", "devLoSoft",
    "devLoSoftCallback", "devlongoutStream", "devLsiSoft", "devLsoSoft",
    "devLsoSoftCallback", "devLsoStdio", "devMbbiSoft", "devMbbiSoftRaw",
    "devMbbiSoftCallback", "devmbbiStream", "devMbbiDirectSoft",
    "devMbbiDirectSoftRaw", "devMbbiDirectSoftCallback",
    "devmbbiDirectStream", "devMbboSoft", "devMbboSoftRaw",
    "devMbboSoftCallback", "devmbboStream", "devMbboDirectSoft",
    "devMbboDirectSoftRaw", "devMbboDirectSoftCallback",
    "devmbboDirectStream", "devPrintfSoft", "devPrintfSoftCallback",
    "devPrintfStdio", "devSiSoft", "devSiSoftCallback", "devTimestampSI",
    "devSiGeneralTime", "devstringinStream", "devSoSoft",
    "devSoSoftCallback", "devSoStdio", "devstringoutStream", "devSASoft",
    "devWfSoft", "devwaveformStream"
};

static const dset * const devsl[] = {
    pvar_dset_devAaiSoft, pvar_dset_devaaiStream, pvar_dset_devAaoSoft,
    pvar_dset_devaaoStream, pvar_dset_devAiSoft, pvar_dset_devAiSoftRaw,
    pvar_dset_devAiSoftCallback, pvar_dset_devTimestampAI,
    pvar_dset_devAiGeneralTime, pvar_dset_devaiStream,
    pvar_dset_devAoSoft, pvar_dset_devAoSoftRaw,
    pvar_dset_devAoSoftCallback, pvar_dset_devaoStream,
    pvar_dset_devBiSoft, pvar_dset_devBiSoftRaw,
    pvar_dset_devBiSoftCallback, pvar_dset_devBiDbState,
    pvar_dset_devbiStream, pvar_dset_devBoSoft, pvar_dset_devBoSoftRaw,
    pvar_dset_devBoSoftCallback, pvar_dset_devBoGeneralTime,
    pvar_dset_devBoDbState, pvar_dset_devboStream,
    pvar_dset_devCalcoutSoft, pvar_dset_devCalcoutSoftCallback,
    pvar_dset_devcalcoutStream, pvar_dset_devEventSoft,
    pvar_dset_devHistogramSoft, pvar_dset_devLiSoft,
    pvar_dset_devLiSoftCallback, pvar_dset_devLiGeneralTime,
    pvar_dset_devlonginStream, pvar_dset_devLoSoft,
    pvar_dset_devLoSoftCallback, pvar_dset_devlongoutStream,
    pvar_dset_devLsiSoft, pvar_dset_devLsoSoft,
    pvar_dset_devLsoSoftCallback, pvar_dset_devLsoStdio,
    pvar_dset_devMbbiSoft, pvar_dset_devMbbiSoftRaw,
    pvar_dset_devMbbiSoftCallback, pvar_dset_devmbbiStream,
    pvar_dset_devMbbiDirectSoft, pvar_dset_devMbbiDirectSoftRaw,
    pvar_dset_devMbbiDirectSoftCallback, pvar_dset_devmbbiDirectStream,
    pvar_dset_devMbboSoft, pvar_dset_devMbboSoftRaw,
    pvar_dset_devMbboSoftCallback, pvar_dset_devmbboStream,
    pvar_dset_devMbboDirectSoft, pvar_dset_devMbboDirectSoftRaw,
    pvar_dset_devMbboDirectSoftCallback, pvar_dset_devmbboDirectStream,
    pvar_dset_devPrintfSoft, pvar_dset_devPrintfSoftCallback,
    pvar_dset_devPrintfStdio, pvar_dset_devSiSoft,
    pvar_dset_devSiSoftCallback, pvar_dset_devTimestampSI,
    pvar_dset_devSiGeneralTime, pvar_dset_devstringinStream,
    pvar_dset_devSoSoft, pvar_dset_devSoSoftCallback,
    pvar_dset_devSoStdio, pvar_dset_devstringoutStream,
    pvar_dset_devSASoft, pvar_dset_devWfSoft, pvar_dset_devwaveformStream
};

epicsShareExtern drvet *pvar_drvet_stream;

static const char *driverSupportNames[] = {
    "stream"};

static struct drvet *drvsl[] = {
    pvar_drvet_stream};

typedef void (*reg_func)(void);
epicsShareExtern reg_func pvar_func_arrInitialize, pvar_func_asSub,
    pvar_func_asynInterposeEosRegister,
    pvar_func_asynInterposeFlushRegister, pvar_func_asynRegister,
    pvar_func_dbndInitialize, pvar_func_drvAsynIPPortRegisterCommands,
    pvar_func_drvAsynIPServerPortRegisterCommands,
    pvar_func_drvAsynSerialPortRegisterCommands,
    pvar_func_streamRegistrar, pvar_func_syncInitialize,
    pvar_func_tsInitialize, pvar_func_vxi11RegisterCommands;

epicsShareExtern int * const pvar_int_asCaDebug;
epicsShareExtern int * const pvar_int_atExitDebug;
epicsShareExtern double * const pvar_double_boHIGHlimit;
epicsShareExtern int * const pvar_int_boHIGHprecision;
epicsShareExtern double * const pvar_double_calcoutODLYlimit;
epicsShareExtern int * const pvar_int_calcoutODLYprecision;
epicsShareExtern int * const pvar_int_callbackParallelThreadsDefault;
epicsShareExtern int * const pvar_int_dbBptNotMonotonic;
epicsShareExtern int * const pvar_int_dbRecordsOnceOnly;
epicsShareExtern int * const pvar_int_dbTemplateMaxVars;
epicsShareExtern int * const pvar_int_histogramSDELprecision;
epicsShareExtern double * const pvar_double_seqDLYlimit;
epicsShareExtern int * const pvar_int_seqDLYprecision;
epicsShareExtern int * const pvar_int_streamDebug;

static struct iocshVarDef vardefs[] = {
    {"asCaDebug", iocshArgInt, pvar_int_asCaDebug},
    {"atExitDebug", iocshArgInt, pvar_int_atExitDebug},
    {"boHIGHlimit", iocshArgDouble, pvar_double_boHIGHlimit},
    {"boHIGHprecision", iocshArgInt, pvar_int_boHIGHprecision},
    {"calcoutODLYlimit", iocshArgDouble, pvar_double_calcoutODLYlimit},
    {"calcoutODLYprecision", iocshArgInt, pvar_int_calcoutODLYprecision},
    {"callbackParallelThreadsDefault", iocshArgInt, pvar_int_callbackParallelThreadsDefault},
    {"dbBptNotMonotonic", iocshArgInt, pvar_int_dbBptNotMonotonic},
    {"dbRecordsOnceOnly", iocshArgInt, pvar_int_dbRecordsOnceOnly},
    {"dbTemplateMaxVars", iocshArgInt, pvar_int_dbTemplateMaxVars},
    {"histogramSDELprecision", iocshArgInt, pvar_int_histogramSDELprecision},
    {"seqDLYlimit", iocshArgDouble, pvar_double_seqDLYlimit},
    {"seqDLYprecision", iocshArgInt, pvar_int_seqDLYprecision},
    {"streamDebug", iocshArgInt, pvar_int_streamDebug},
    {NULL, iocshArgInt, NULL}
};

int streamApp_registerRecordDeviceDriver(DBBASE *pbase)
{
    static int executed = 0;
    const char *bldTop = "D:/epics/support/stream";
    const char *envTop = getenv("TOP");

    if (envTop && strcmp(envTop, bldTop)) {
        printf("Warning: IOC is booting with TOP = \"%s\"\n"
               "          but was built with TOP = \"%s\"\n",
               envTop, bldTop);
    }

    if (!pbase) {
        printf("pdbbase is NULL; you must load a DBD file first.\n");
        return -1;
    }

    if (executed) {
        printf("Warning: Registration already done.\n");
    }
    executed = 1;

    registerRecordTypes(pbase, NELEMENTS(rtl), recordTypeNames, rtl);
    registerDevices(pbase, NELEMENTS(devsl), deviceSupportNames, devsl);
    registerDrivers(pbase, NELEMENTS(drvsl), driverSupportNames, drvsl);
    pvar_func_arrInitialize();
    pvar_func_asSub();
    pvar_func_asynInterposeEosRegister();
    pvar_func_asynInterposeFlushRegister();
    pvar_func_asynRegister();
    pvar_func_dbndInitialize();
    pvar_func_drvAsynIPPortRegisterCommands();
    pvar_func_drvAsynIPServerPortRegisterCommands();
    pvar_func_drvAsynSerialPortRegisterCommands();
    pvar_func_streamRegistrar();
    pvar_func_syncInitialize();
    pvar_func_tsInitialize();
    pvar_func_vxi11RegisterCommands();
    iocshRegisterVariable(vardefs);
    return 0;
}

/* streamApp_registerRecordDeviceDriver */
static const iocshArg rrddArg0 = {"pdbbase", iocshArgPdbbase};
static const iocshArg *rrddArgs[] = {&rrddArg0};
static const iocshFuncDef rrddFuncDef =
    {"streamApp_registerRecordDeviceDriver", 1, rrddArgs};
static void rrddCallFunc(const iocshArgBuf *)
{
    streamApp_registerRecordDeviceDriver(*iocshPpdbbase);
}

} // extern "C"

/*
 * Register commands on application startup
 */
static int Registration() {
    iocshRegisterCommon();
    iocshRegister(&rrddFuncDef, rrddCallFunc);
    return 0;
}

static int done = Registration();
