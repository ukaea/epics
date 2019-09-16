/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 * @date 2013.07.24
 */


/* Author: Marty Kraimer */

#include <epicsThread.h>
#include <iocsh.h>
#include <pv/event.h>
#include <pv/pvAccess.h>
#include <pv/serverContext.h>
#include <pv/pvData.h>
#include <pv/pvTimeStamp.h>
#include <pv/rpcService.h>

// The following must be the last include for code pvDatabase uses
#include <epicsExport.h>
#define epicsExportSharedSymbols
#include "pv/pvStructureCopy.h"
#include "pv/pvDatabase.h"
#include "pv/traceRecord.h"

using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvDatabase;
using namespace std;

static const iocshArg testArg0 = { "recordName", iocshArgString };
static const iocshArg *testArgs[] = {
    &testArg0};

static const iocshFuncDef traceRecordFuncDef = {"traceRecordCreate", 1,testArgs};

static void traceRecordCallFunc(const iocshArgBuf *args)
{
    char *recordName = args[0].sval;
    if(!recordName) {
        throw std::runtime_error("traceRecordCreate invalid number of arguments");
    }
    TraceRecordPtr record = TraceRecord::create(recordName);
    bool result = PVDatabase::getMaster()->addRecord(record);
    if(!result) cout << "recordname" << " not added" << endl;
}

static void traceRecordRegister(void)
{
    static int firstTime = 1;
    if (firstTime) {
        firstTime = 0;
        iocshRegister(&traceRecordFuncDef, traceRecordCallFunc);
    }
}

extern "C" {
    epicsExportRegistrar(traceRecordRegister);
}
