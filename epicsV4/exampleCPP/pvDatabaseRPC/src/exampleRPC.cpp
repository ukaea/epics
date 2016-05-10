// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution

/* exampleRPC.cpp */

/**
 * @author dgh
 * @date 2015.12.08
 */

#include <pv/standardField.h>

#define epicsExportSharedSymbols
#include <pv/exampleRPC.h>

#include <pv/standardField.h>
#include <pv/standardPVField.h>

#include <epicsThread.h>

using namespace epics::pvData;
using namespace epics::pvDatabase;
using std::tr1::static_pointer_cast;
using std::string;

namespace epics { namespace exampleCPP { namespace exampleRPC { 


PVStructurePtr ExampleRPCService::request(
    PVStructure::shared_pointer const & args
) throw (epics::pvAccess::RPCRequestException)
{
    bool haveControl = pvRecord->takeControl();
    if (!haveControl)
        throw pvAccess::RPCRequestException(Status::STATUSTYPE_ERROR,
            "I'm busy");

    FieldCreatePtr fieldCreate = getFieldCreate();
    PVDataCreatePtr pvDataCreate = getPVDataCreate();

    PVStructureArrayPtr valueField = args->getSubField<PVStructureArray>("value");
    if (valueField.get() == 0)
        throw pvAccess::RPCRequestException(Status::STATUSTYPE_ERROR,
            "No structure array value field");

    StructureConstPtr valueFieldStructure = valueField->
        getStructureArray()->getStructure();

    ScalarConstPtr xField = valueFieldStructure->getField<Scalar>("x");
    if (xField.get() == 0 || xField->getScalarType() != pvDouble)
        throw pvAccess::RPCRequestException(Status::STATUSTYPE_ERROR,
            "value field's structure has no double field x");

    ScalarConstPtr yField = valueFieldStructure->getField<Scalar>("y");
    if (xField.get() == 0 || xField->getScalarType() != pvDouble)
        throw pvAccess::RPCRequestException(Status::STATUSTYPE_ERROR,
            "value field's structure has no double field y");

    PVStructureArray::const_svector vals = valueField->view();

    for (PVStructureArray::const_svector::const_iterator it = vals.begin();
         it != vals.end(); ++it)
    {
        double x = (*it)->getSubFieldT<PVDouble>("x")->get();
        double y = (*it)->getSubFieldT<PVDouble>("y")->get();
        pvRecord->put(x,y);
        epicsThreadSleep(1.0);
    }

    StructureConstPtr  topStructure = fieldCreate->createFieldBuilder()->
        createStructure();

    PVStructurePtr returned = pvDataCreate->createPVStructure(topStructure);
    pvRecord->releaseControl();
    return returned;
}

void ExampleRPCServiceAsync::request(
    epics::pvData::PVStructurePtr const & args,
    epics::pvAccess::RPCResponseCallback::shared_pointer const & callback)
{
    bool haveControl = pvRecord->takeControl();
    if (!haveControl)
        throw pvAccess::RPCRequestException(Status::STATUSTYPE_ERROR,
            "I'm busy");

    FieldCreatePtr fieldCreate = getFieldCreate();
    PVDataCreatePtr pvDataCreate = getPVDataCreate();

    PVStructureArrayPtr valueField = args->getSubField<PVStructureArray>("value");
    if (valueField.get() == 0)
        throw pvAccess::RPCRequestException(Status::STATUSTYPE_ERROR,
            "No structure array value field");

    StructureConstPtr valueFieldStructure = valueField->
        getStructureArray()->getStructure();

    ScalarConstPtr xField = valueFieldStructure->getField<Scalar>("x");
    if (xField.get() == 0 || xField->getScalarType() != pvDouble)
        throw pvAccess::RPCRequestException(Status::STATUSTYPE_ERROR,
            "value field's structure has no double field x");

    ScalarConstPtr yField = valueFieldStructure->getField<Scalar>("y");
    if (yField.get() == 0 || yField->getScalarType() != pvDouble)
        throw pvAccess::RPCRequestException(Status::STATUSTYPE_ERROR,
            "value field's structure has no double field y");

    PVStructureArray::const_svector vals = valueField->view();

    for (PVStructureArray::const_svector::const_iterator it = vals.begin();
         it != vals.end(); ++it)
    {
        double x = (*it)->getSubFieldT<PVDouble>("x")->get();
        double y = (*it)->getSubFieldT<PVDouble>("y")->get();
        pvRecord->put(x,y);
        epicsThreadSleep(1.0);
    }

    StructureConstPtr  topStructure = fieldCreate->createFieldBuilder()->
        createStructure();

    PVStructurePtr returned = pvDataCreate->createPVStructure(topStructure);
    pvRecord->releaseControl();
    callback->requestDone(Status::Ok, returned);
}



ExampleRPCPtr ExampleRPC::create(
    string const & recordName)
{
    StandardFieldPtr standardField = getStandardField();
    FieldCreatePtr fieldCreate = getFieldCreate();
    PVDataCreatePtr pvDataCreate = getPVDataCreate();
    StructureConstPtr  topStructure = fieldCreate->createFieldBuilder()->
        add("x",pvDouble)->
        add("y",pvDouble)->
        add("timeStamp",standardField->timeStamp()) ->
        createStructure();
    PVStructurePtr pvStructure = pvDataCreate->createPVStructure(topStructure);

    ExampleRPCPtr pvRecord(
        new ExampleRPC(recordName,pvStructure));
    if(!pvRecord->init()) pvRecord.reset();
    return pvRecord;
}

ExampleRPC::ExampleRPC(
    string const & recordName,
    PVStructurePtr const & pvStructure)
: PVRecord(recordName,pvStructure)
{    
}

ExampleRPC::~ExampleRPC()
{
}

void ExampleRPC::destroy()
{
    PVRecord::destroy();
}

bool ExampleRPC::init()
{
    
    initPVRecord();

    service = ExampleRPCService::create(
        std::tr1::dynamic_pointer_cast<ExampleRPC>(
            shared_from_this()));

    PVFieldPtr pvField;
    pvTimeStamp.attach(getPVStructure()->getSubField("timeStamp"));
    return true;
}

epics::pvAccess::Service::shared_pointer ExampleRPC::getService(
        PVStructurePtr const & /*pvRequest*/)
{
    return service;
}

bool ExampleRPC::takeControl()
{
    return taskMutex.tryLock();
}

void ExampleRPC::releaseControl()
{
    taskMutex.unlock();
}

void ExampleRPC::put(double x, double y)
{
    lock();
    beginGroupPut();
    getPVStructure()->getSubField<PVDouble>("x")->put(x);
    getPVStructure()->getSubField<PVDouble>("y")->put(y);
    process();
    endGroupPut();
    unlock();
}

void ExampleRPC::process()
{
    PVRecord::process();
}


}}}
