/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 * @date 2013.04.02
 */

#include <epicsThread.h>
#include <pv/standardField.h>
#include <sstream>
#define epicsExportSharedSymbols
#include <pv/exampleHelloRPCRecord.h>

using namespace epics::pvData;
using namespace epics::pvDatabase;
using namespace epics::pvAccess;
using std::tr1::static_pointer_cast;
using std::tr1::dynamic_pointer_cast;
using namespace std;

namespace epics { namespace exampleCPP { namespace database {


ExampleHelloRPCRecordPtr  ExampleHelloRPCRecord::create(string const & recordName)
{
    FieldCreatePtr fieldCreate = getFieldCreate();
    PVDataCreatePtr pvDataCreate = getPVDataCreate();
    StructureConstPtr  topStructure = fieldCreate->createFieldBuilder()->
        add("value",pvString)->
        createStructure();
    PVStructurePtr pvTop(pvDataCreate->createPVStructure(topStructure));
    ExampleHelloRPCRecordPtr pvRecord(
        new ExampleHelloRPCRecord(recordName,pvTop));
    pvRecord->service = Service::shared_pointer(pvRecord);
    pvRecord->initPVRecord();
    return pvRecord;
}

ExampleHelloRPCRecord::ExampleHelloRPCRecord(
    string const & recordName,
    PVStructurePtr const & pvTop)
: PVRecord(recordName,pvTop),
  pvTop(pvTop)
{
}

Service::shared_pointer ExampleHelloRPCRecord::getService(PVStructurePtr const & pvRequest)
{
     return service;
}

PVStructurePtr ExampleHelloRPCRecord::put(
     PVStringPtr const &pvFrom)
{
    lock();
    beginGroupPut();
    PVStringPtr pvTo = pvTop->getSubField<PVString>("value");
    pvTo->put("Hello " + pvFrom->get());
    process();
    epicsThreadSleep(1.0);  // simulate service that takes time
    endGroupPut();
    PVStructurePtr pvResult(getPVDataCreate()->createPVStructure(pvTop));
    unlock();
    return pvResult;
    
}

PVStructurePtr ExampleHelloRPCRecord::request(PVStructurePtr const & pvArgument)
{
    PVDataCreatePtr pvDataCreate = getPVDataCreate();
    PVStringPtr pvFrom = pvArgument->getSubField<PVString>("value");
    if(!pvFrom) {
        stringstream ss;
        ss << " expected string subfield named value. got\n" << pvArgument;
        throw RPCRequestException(
                 Status::STATUSTYPE_ERROR,ss.str());
    }
    return put(pvFrom);
}

}}}
