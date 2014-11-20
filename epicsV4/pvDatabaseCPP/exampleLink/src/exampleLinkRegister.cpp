/*exampleLink.cpp */
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
#include <pv/standardField.h>
#include <pv/standardPVField.h>
#include <pv/pvAccess.h>
#include <pv/pvDatabase.h>

#include <epicsExport.h>
#include <pv/exampleLink.h>

using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvDatabase;
using std::cout;
using std::endl;
using std::string;

static StandardPVFieldPtr standardPVField = getStandardPVField();

static const iocshArg testArg0 = { "recordName", iocshArgString };
static const iocshArg testArg1 = { "providerName", iocshArgString };
static const iocshArg testArg2 = { "channelName", iocshArgString };
static const iocshArg *testArgs[] = {
    &testArg0,&testArg1,&testArg2};

static const iocshFuncDef exampleLinkFuncDef = {
    "exampleLinkCreateRecord", 3, testArgs};
static void exampleLinkCallFunc(const iocshArgBuf *args)
{
    PVDatabasePtr master = PVDatabase::getMaster();
    PVRecordPtr pvRecord;
    bool result(false);
    string recordName;
    PVStructurePtr pvStructure = standardPVField->scalarArray(
        pvDouble,"alarm,timeStamp");
    pvRecord = PVRecord::create("doubleArray",pvStructure);
    result = master->addRecord(pvRecord);
    if(!result) cout<< "record " << recordName << " not added" << endl;

    recordName = args[0].sval;
    char *providerName = args[1].sval;
    char *channelName = args[2].sval;
    ExampleLinkPtr record = ExampleLink::create(recordName,providerName,channelName);
    if(record) 
        result = master->addRecord(record);
    if(!result) cout << "recordname" << " not added" << endl;
}

static void exampleLinkRegister(void)
{
    static int firstTime = 1;
    if (firstTime) {
        firstTime = 0;
        iocshRegister(&exampleLinkFuncDef, exampleLinkCallFunc);
    }
}

extern "C" {
    epicsExportRegistrar(exampleLinkRegister);
}
