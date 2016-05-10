// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution

/* exampleHelloRPC.cpp */

/**
 * @author mrk
 * @date 2013.04.02
 */

#include <pv/standardField.h>
#include <sstream>
#define epicsExportSharedSymbols
#include <pv/exampleHelloRPC.h>

using namespace epics::pvData;
using namespace epics::pvDatabase;
using namespace epics::pvAccess;
using std::tr1::static_pointer_cast;
using std::tr1::dynamic_pointer_cast;
using namespace std;

namespace epics { namespace exampleCPP { namespace database {

PVStructurePtr ExampleHelloRPCService::request(PVStructurePtr const & pvArgument)
{
    PVStringPtr pvFrom = pvArgument->getSubField<PVString>("value");
    if(!pvFrom) {
        stringstream ss;
        ss << " expected string subfield named value. got\n" << pvArgument;
        throw RPCRequestException(
                 Status::STATUSTYPE_ERROR,ss.str());
    }
    pvRecord->put(pvFrom);
    return pvRecord->pvResult;
}

ExampleHelloRPCPtr  ExampleHelloRPC::create(string const & recordName)
{
    FieldCreatePtr fieldCreate = getFieldCreate();
    PVDataCreatePtr pvDataCreate = getPVDataCreate();
    StructureConstPtr  topStructure = fieldCreate->createFieldBuilder()->
        add("value",pvString)->
        createStructure();
    PVStructurePtr pvStructure = pvDataCreate->createPVStructure(topStructure);
    ExampleHelloRPCPtr pvRecord(
        new ExampleHelloRPC(recordName,pvStructure));
    if(!pvRecord->init()) pvRecord.reset();
    return pvRecord;
}

ExampleHelloRPC::ExampleHelloRPC(
    string const & recordName,
    PVStructurePtr const & pvResult)
: PVRecord(recordName,pvResult),
  pvResult(pvResult)
{
}

bool ExampleHelloRPC::init()
{
    initPVRecord();
    service = ExampleHelloRPCService::create(
        std::tr1::dynamic_pointer_cast<ExampleHelloRPC>(
            shared_from_this()));
    return true;
}


Service::shared_pointer ExampleHelloRPC::getService(PVStructurePtr const & pvRequest)
{
     return service;
}

void ExampleHelloRPC::put(PVStringPtr const &pvFrom)
{
    lock();
    beginGroupPut();
    PVStringPtr pvTo = pvResult->getSubField<PVString>("value");
    pvTo->put("Hello " + pvFrom->get());
    process();
    endGroupPut();
    unlock();
}

}}}
