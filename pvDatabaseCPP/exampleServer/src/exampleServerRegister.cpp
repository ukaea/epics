/*exampleServerRegister.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 * @date 2013.07.24
 */


/* Author: Marty Kraimer */

#include <cstddef>
#include <cstdlib>
#include <cstddef>
#include <string>
#include <cstdio>
#include <memory>
#include <iostream>

#include <cantProceed.h>
#include <epicsStdio.h>
#include <epicsMutex.h>
#include <epicsEvent.h>
#include <epicsThread.h>
#include <iocsh.h>

#include <pv/pvIntrospect.h>
#include <pv/pvData.h>
#include <pv/pvAccess.h>
#include <pv/pvDatabase.h>
#include <pv/recordList.h>

#include <epicsExport.h>
#include <pv/exampleServer.h>

using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvDatabase;
using namespace epics::exampleServer;
using std::cout;
using std::endl;

static const iocshArg testArg0 = { "recordName", iocshArgString };
static const iocshArg *testArgs[] = {
    &testArg0};

static const iocshFuncDef exampleServerFuncDef = {
    "exampleServerCreateRecord", 1, testArgs};
static void exampleServerCallFunc(const iocshArgBuf *args)
{
    PVDatabasePtr master = PVDatabase::getMaster();
    char *recordName = args[0].sval;
    ExampleServerPtr record = ExampleServer::create(recordName);
    bool result = master->addRecord(record);
    if(!result) cout << "recordname" << " not added" << endl;
    PVRecordPtr pvRecord = RecordListRecord::create(
        "laptoprecordListPGRPC");
    if(!pvRecord) {
          cout << "RecordListRecord::create failed" << endl;
    } else {
        result = master->addRecord(pvRecord);
        if(!result) cout<< "record " << recordName << " not added" << endl;
    }
}

static void exampleServerRegister(void)
{
    static int firstTime = 1;
    if (firstTime) {
        firstTime = 0;
        iocshRegister(&exampleServerFuncDef, exampleServerCallFunc);
    }
}

extern "C" {
    epicsExportRegistrar(exampleServerRegister);
}
