// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution

/*exampleLinkRegister.cpp */

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
#include <pv/ntscalarArray.h>
#include <pv/pvDatabase.h>

#include <epicsExport.h>
#include <pv/exampleLinkRecord.h>

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

static const iocshArg testArg0 = { "provider", iocshArgString };
static const iocshArg testArg1 = { "exampleLinkRecordName", iocshArgString };
static const iocshArg testArg2 = { "linkedRecordName", iocshArgString };
static const iocshArg testArg3 = { "generateLinkedRecord", iocshArgString };
static const iocshArg *testArgs[] = {
    &testArg0,&testArg1,&testArg2,&testArg3};

static const iocshFuncDef exampleLinkFuncDef = {
    "exampleLinkCreateRecord", 4, testArgs};
static void exampleLinkCallFunc(const iocshArgBuf *args)
{
    string provider("pva");
    string exampleLinkRecord("exampleLink");
    string linkedRecordName("doubleArray");
    bool generateLinkedRecord(true);
    char *sval = args[0].sval;
    if(sval) provider = string(sval);
    sval = args[1].sval;
    if(sval) exampleLinkRecord = string(sval);
    sval = args[2].sval;
    if(sval) linkedRecordName = string(sval);
    sval = args[3].sval;
    if(sval) {
        string value = string(sval);
        if(value=="false") generateLinkedRecord = false;
    }
    PVDatabasePtr master = PVDatabase::getMaster();
    PVRecordPtr pvRecord;
    bool result(false);
    if(generateLinkedRecord) {
        NTScalarArrayBuilderPtr ntScalarArrayBuilder = NTScalarArray::createBuilder();
        PVStructurePtr pvStructure = ntScalarArrayBuilder->
            value(pvDouble)->
            addAlarm()->
            addTimeStamp()->
            createPVStructure();
        result = master->addRecord(PVRecord::create(linkedRecordName,pvStructure));
        if(!result) cout<< "record " << linkedRecordName << " not added" << endl;
    }
    PvaClientPtr pva= PvaClient::get(provider);
    ExampleLinkRecordPtr record = ExampleLinkRecord::create(pva,exampleLinkRecord,provider,linkedRecordName);
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
    PVDatabasePtr master = PVDatabase::getMaster();
