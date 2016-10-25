/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

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


static StructureConstPtr makeResultStructure()
{
    static StructureConstPtr resultStructure;
    if (resultStructure.get() == 0)
    {
        FieldCreatePtr fieldCreate = getFieldCreate();

        resultStructure = fieldCreate->createFieldBuilder()->
            createStructure();
    }

    return resultStructure;
}

static StructureConstPtr makePointStructure()
{
    static StructureConstPtr pointStructure;
    if (pointStructure.get() == 0)
    {
        FieldCreatePtr fieldCreate = getFieldCreate();

        pointStructure = fieldCreate->createFieldBuilder()->
            setId("point_t")->
            add("x",pvDouble)->
            add("y",pvDouble)->
            createStructure();
    }

    return pointStructure;
}

static StructureConstPtr makePointTopStructure()
{
    static StructureConstPtr pointStructure;
    if (pointStructure.get() == 0)
    {
        FieldCreatePtr fieldCreate = getFieldCreate();

        pointStructure = fieldCreate->createFieldBuilder()->
            setId("Point")->
            add("value", makePointStructure())->
            add("timeStamp", getStandardField()->timeStamp())->
            createStructure();
    }
    return pointStructure;
}

static StructureConstPtr makeRecordStructure()
{
    static StructureConstPtr recordStructure;
    if (recordStructure.get() == 0)
    {
        FieldCreatePtr fieldCreate = getFieldCreate();

        recordStructure = fieldCreate->createFieldBuilder()->
            add("positionSP", makePointTopStructure())->
            add("positionRB", makePointTopStructure())->
            add("state", getStandardField()->enumerated("timeStamp"))->
            add("timeStamp", getStandardField()->timeStamp())->
            createStructure();
    }
    return recordStructure;
}



PVStructurePtr AbortService::request(
    PVStructure::shared_pointer const & args
) throw (epics::pvAccess::RPCRequestException)
{
    try {
        pvRecord->getDevice()->abort();
    }
    catch (IllegalOperationException & e) {
        throw epics::pvAccess::RPCRequestException(
            Status::STATUSTYPE_ERROR,e.what());
    }
    return getPVDataCreate()->createPVStructure(makeResultStructure());
}


PVStructurePtr ConfigureService::request(
    PVStructure::shared_pointer const & args
) throw (epics::pvAccess::RPCRequestException)
{
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
    
    std::vector<Point> newPoints;
    newPoints.reserve(vals.size());
    for (PVStructureArray::const_svector::const_iterator it = vals.begin();
         it != vals.end(); ++it)
    {
        double x = (*it)->getSubFieldT<PVDouble>("x")->get();
        double y = (*it)->getSubFieldT<PVDouble>("y")->get();
        newPoints.push_back(Point(x,y));
    }

    try {
        pvRecord->getDevice()->configure(newPoints);
    }
    catch (IllegalOperationException & e) {
        throw epics::pvAccess::RPCRequestException(
            Status::STATUSTYPE_ERROR,e.what());
    }
    return getPVDataCreate()->createPVStructure(makeResultStructure());
}

PVStructurePtr RunService::request(
    PVStructure::shared_pointer const & args
) throw (epics::pvAccess::RPCRequestException)
{
    try {
        pvRecord->getDevice()->runScan();
    }
    catch (IllegalOperationException & e) {
        throw epics::pvAccess::RPCRequestException(
            Status::STATUSTYPE_ERROR,e.what());
    }
    return getPVDataCreate()->createPVStructure(makeResultStructure());
}


PVStructurePtr PauseService::request(
    PVStructure::shared_pointer const & args
) throw (epics::pvAccess::RPCRequestException)
{
    try {
        pvRecord->getDevice()->pause();
    }
    catch (IllegalOperationException & e) {
        throw epics::pvAccess::RPCRequestException(
            Status::STATUSTYPE_ERROR,e.what());
    }
    return getPVDataCreate()->createPVStructure(makeResultStructure());
}

PVStructurePtr ResumeService::request(
    PVStructure::shared_pointer const & args
) throw (epics::pvAccess::RPCRequestException)
{
    try {
        pvRecord->getDevice()->resume();
    }
    catch (IllegalOperationException & e) {
        throw epics::pvAccess::RPCRequestException(
            Status::STATUSTYPE_ERROR,e.what());
    }
    return getPVDataCreate()->createPVStructure(makeResultStructure());
}

PVStructurePtr StopService::request(
    PVStructure::shared_pointer const & args
) throw (epics::pvAccess::RPCRequestException)
{
    try {
        pvRecord->getDevice()->stopScan();
    }
    catch (IllegalOperationException & e) {
        throw epics::pvAccess::RPCRequestException(
            Status::STATUSTYPE_ERROR,e.what());
    }
    return getPVDataCreate()->createPVStructure(makeResultStructure());
}


int RewindService::getRequestedSteps(PVStructurePtr const & args)
{
    PVIntPtr valueField = args->getSubField<PVInt>("value");
    if (valueField.get() == NULL)
        throw epics::pvAccess::RPCRequestException(Status::STATUSTYPE_ERROR,
            "No int value field");

    return valueField->get();
}

PVStructurePtr RewindService::request(
    PVStructure::shared_pointer const & args
) throw (epics::pvAccess::RPCRequestException)
{
    int n = getRequestedSteps(args);
    try {
        pvRecord->getDevice()->rewind(n);
    }
    catch (IllegalOperationException & e) {
        throw epics::pvAccess::RPCRequestException(
            Status::STATUSTYPE_ERROR,e.what());
    }
    return getPVDataCreate()->createPVStructure(makeResultStructure());
}


ScanService::Callback::shared_pointer ScanService::Callback::create(ScanServicePtr const & service)
{
    return ScanService::Callback::shared_pointer(new ScanService::Callback(service));
}

void ScanService::request(
    PVStructurePtr const & args,
    epics::pvAccess::RPCResponseCallback::shared_pointer const & callback)
{
    pvRecord->getDevice()->runScan();
    ScanService::Callback::shared_pointer cb = ScanService::Callback::create(shared_from_this());
    this->rpcCallback = callback;
    this->deviceCallback = cb;
    pvRecord->getDevice()->registerCallback(cb);
}
 

void ScanService::handleError(const std::string & message)
{
    rpcCallback->requestDone(
        Status(Status::STATUSTYPE_ERROR, std::string(message)),
        PVStructure::shared_pointer());

    pvRecord->getDevice()->unregisterCallback(
         deviceCallback);
}

void ScanService::stateChanged(Device::State state)
{
    if (state == Device::READY)
    {
        handleError("Scan was stopped");
    }
    else if (state == Device::IDLE)
    {
        handleError("Scan was aborted");
    }
}

void ScanService::scanComplete()
{
    rpcCallback->requestDone(Status::Ok, getPVDataCreate()->createPVStructure(makeResultStructure()));
    pvRecord->getDevice()->unregisterCallback(deviceCallback);
}


void ScanService::update(int flags)
{
    if ((flags & Device::Callback::SCAN_COMPLETE) != 0)
    {
       scanComplete();
    }
    else if ((flags & Device::Callback::STATE_CHANGED) != 0)
    {
        Device::State state = pvRecord->getDevice()->getState();
        stateChanged(state);
    }
}

void ScanService::Callback::update(int flags)
{
    service->update(flags);
}


ExampleRPC::Callback::shared_pointer ExampleRPC::Callback::create(ExampleRPCPtr const & record)
{
    return ExampleRPC::Callback::shared_pointer(new ExampleRPC::Callback(record));
}

void ExampleRPC::Callback::update(int flags)
{
    record->update(flags);
}

void ExampleRPC::update(int flags)
{
    lock();
    try {
        TimeStamp timeStamp;
        timeStamp.getCurrent();
        beginGroupPut();

        if ((flags & Device::Callback::SETPOINT_CHANGED) != 0)
        {
            Point sp = device->getPositionSetpoint();
            pvx->put(sp.x);
            pvy->put(sp.y);
            pvTimeStamp_sp.set(timeStamp);
        }

        if ((flags & Device::Callback::READBACK_CHANGED) != 0)
        {
            Point rb = device->getPositionReadback();
            pvx_rb->put(rb.x);
            pvy_rb->put(rb.y);
            pvTimeStamp_rb.set(timeStamp);
        }

        if ((flags & Device::Callback::STATE_CHANGED) != 0)
        {
            int index = static_cast<int>(device->getState());
            if (index != pvStateIndex->get())
            {
                pvStateIndex->put(index);
                pvTimeStamp_st.set(timeStamp);
            }
        }

        pvTimeStamp.set(timeStamp);
        endGroupPut();
    }
    catch(...)
    {
        unlock();
        throw;
    }
    unlock();
}



ExampleRPCPtr ExampleRPC::create(
    string const & recordName)
{
    StandardFieldPtr standardField = getStandardField();
    FieldCreatePtr fieldCreate = getFieldCreate();
    PVDataCreatePtr pvDataCreate = getPVDataCreate();

    PVStructurePtr pvStructure = pvDataCreate->createPVStructure(makeRecordStructure());

    ExampleRPCPtr pvRecord(
        new ExampleRPC(recordName,pvStructure));
    pvRecord->initPvt();
    return pvRecord;
}

ExampleRPC::ExampleRPC(
    string const & recordName,
    PVStructurePtr const & pvStructure)
: PVRecord(recordName,pvStructure), firstTime(true)
{
    pvx    = pvStructure->getSubFieldT<PVDouble>("positionSP.value.x");
    pvy    = pvStructure->getSubFieldT<PVDouble>("positionSP.value.y");
    pvx_rb = pvStructure->getSubFieldT<PVDouble>("positionRB.value.x");
    pvy_rb = pvStructure->getSubFieldT<PVDouble>("positionRB.value.y");

    pvStateIndex = pvStructure->getSubFieldT<PVInt>("state.value.index");
    pvStateChoices = pvStructure->getSubFieldT<PVStringArray>("state.value.choices");

    pvTimeStamp.attach(pvStructure->getSubFieldT<PVStructure>("timeStamp"));
    pvTimeStamp_sp.attach(pvStructure->getSubFieldT<PVStructure>("positionSP.timeStamp"));
    pvTimeStamp_rb.attach(pvStructure->getSubFieldT<PVStructure>("positionRB.timeStamp"));
        pvTimeStamp_st.attach(pvStructure->getSubFieldT<PVStructure>("state.timeStamp"));

    PVStringArray::svector choices;
    choices.reserve(4);
    choices.push_back(Device::toString(Device::IDLE));
    choices.push_back(Device::toString(Device::READY));
    choices.push_back(Device::toString(Device::RUNNING));
    choices.push_back(Device::toString(Device::PAUSED));
    pvStateChoices->replace(freeze(choices));

    device = Device::create();
}

void ExampleRPC::initPvt()
{
    initPVRecord();

    PVFieldPtr pvField;
    pvTimeStamp.attach(getPVStructure()->getSubField("timeStamp"));

    device->registerCallback(Callback::create(std::tr1::dynamic_pointer_cast<ExampleRPC>(shared_from_this())));

    process();
}

epics::pvAccess::Service::shared_pointer ExampleRPC::getService(
        PVStructurePtr const & pvRequest)
{
    PVStringPtr methodField = pvRequest->getSubField<PVString>("method");

    if (methodField.get() != 0)
    {
        std::string method = methodField->get();
        if (method == "abort")
        {
             return AbortService::create(
                 std::tr1::dynamic_pointer_cast<ExampleRPC>(
                 shared_from_this()));
        }
        else if (method == "configure")
        {
            return ConfigureService::create(
                 std::tr1::dynamic_pointer_cast<ExampleRPC>(
                 shared_from_this()));
        }
        else if (method == "run")
        {
            return RunService::create(
                 std::tr1::dynamic_pointer_cast<ExampleRPC>(
                 shared_from_this()));
        }
        else if (method == "resume")
        {
            return ResumeService::create(
                 std::tr1::dynamic_pointer_cast<ExampleRPC>(
                 shared_from_this()));
        }
        else if (method == "pause")
        {
            return PauseService::create(
                 std::tr1::dynamic_pointer_cast<ExampleRPC>(
                 shared_from_this()));
        }
        else if (method == "stop")
        {
            return StopService::create(
                 std::tr1::dynamic_pointer_cast<ExampleRPC>(
                 shared_from_this()));
        }
        else if (method == "rewind")
        {
            return RewindService::create(
                 std::tr1::dynamic_pointer_cast<ExampleRPC>(
                 shared_from_this()));
        }
        else if (method  == "scan")
        {
            return ScanService::create(
                 std::tr1::dynamic_pointer_cast<ExampleRPC>(
                 shared_from_this()));
        }
    }
    return epics::pvAccess::Service::shared_pointer();
}

void ExampleRPC::process()
{
    TimeStamp timeStamp;
    timeStamp.getCurrent();

    Point newSP = Point(pvx->get(), pvy->get()); 
    try
    {
        Point sp_initial = device->getPositionSetpoint();
      
        if (sp_initial != newSP)
        {
            device->setSetpoint(newSP);
            pvTimeStamp_sp.set(timeStamp);
        }
    }
    catch (IllegalOperationException & o)
    {
        // If write to device fails restore values
        Point sp = device->getPositionSetpoint();
        if (sp != newSP)
        {
            pvx->put(sp.x);
            pvy->put(sp.y);
        }
    }

    // If readback is written to, restore value
    Point device_rb = device->getPositionReadback();
    Point record_rb = Point(pvx_rb->get(), pvx_rb->get());
    if (record_rb != device_rb)
    {
        pvx_rb->put(device_rb.x);
        pvy_rb->put(device_rb.y);        
    }

    // If state is written to, restore value
    int index = static_cast<int>(device->getState());
    if (index != pvStateIndex->get())
    {
        pvStateIndex->put(index);
    }

    if (firstTime) {
        pvTimeStamp_sp.set(timeStamp);
        pvTimeStamp_rb.set(timeStamp);
        pvTimeStamp_st.set(timeStamp);
        firstTime = false;
    }

    pvTimeStamp.set(timeStamp);
}


}}}
