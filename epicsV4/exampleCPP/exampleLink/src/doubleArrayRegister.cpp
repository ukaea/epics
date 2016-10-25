/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 * @date 2016.06.17
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
#include <pv/ntscalarArray.h>
#include <pv/pvDatabase.h>

#include <epicsExport.h>
#include <pv/doubleArrayRecord.h>

using namespace epics::pvData;
using namespace epics::nt;
using namespace epics::pvAccess;
using namespace epics::pvaClient;
using namespace epics::pvDatabase;
using namespace epics::exampleCPP::exampleLink;
using std::cout;
using std::endl;
using std::string;

static StandardPVFieldPtr standardPVField = getStandardPVField();

static const iocshArg testArg0 = { "recordName", iocshArgString };

static const iocshArg *testArgs[] = {
    &testArg0};

static const iocshFuncDef doubleArrayFuncDef = {
    "doubleArrayCreateRecord", 1, testArgs};
static void doubleArrayCallFunc(const iocshArgBuf *args)
{
    string recordName("doubleArray");
    char *sval = args[0].sval;
    if(sval) recordName = string(sval);
    PVDatabasePtr master = PVDatabase::getMaster();
    NTScalarArrayBuilderPtr ntScalarArrayBuilder = NTScalarArray::createBuilder();
    PVStructurePtr pvStructure = ntScalarArrayBuilder->
        value(pvDouble)->
        addAlarm()->
        addTimeStamp()->
        createPVStructure();
    bool result = master->addRecord(PVRecord::create(recordName,pvStructure));
    if(!result) cout<< "record " << recordName << " not added" << endl;
}

static void doubleArrayRegister(void)
{
    static int firstTime = 1;
    if (firstTime) {
        firstTime = 0;
        iocshRegister(&doubleArrayFuncDef, doubleArrayCallFunc);
    }
}

extern "C" {
    epicsExportRegistrar(doubleArrayRegister);
} 
