/* exampleLink.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 * @date 2013.08.02
 */

#include <pv/standardPVField.h>

#define epicsExportSharedSymbols
#include <pv/exampleLink.h>

using namespace epics::pvData;
using namespace epics::pvAccess;
using std::tr1::static_pointer_cast;
using std::tr1::dynamic_pointer_cast;
using std::cout;
using std::endl;
using std::string;

namespace epics { namespace pvDatabase { 

ExampleLinkPtr ExampleLink::create(
    string const & recordName,
    string const & providerName,
    string const & channelName)
{
    PVStructurePtr pvStructure = getStandardPVField()->scalarArray(
        pvDouble,"alarm.timeStamp");
    ExampleLinkPtr pvRecord(
        new ExampleLink(
           recordName,providerName,channelName,pvStructure));    
    if(!pvRecord->init()) pvRecord.reset();
    return pvRecord;
}

ExampleLink::ExampleLink(
    string const & recordName,
    string providerName,
    string channelName,
    PVStructurePtr const & pvStructure)
: PVRecord(recordName,pvStructure),
  providerName(providerName),
  channelName(channelName)
{
}

void ExampleLink::destroy()
{
    PVRecord::destroy();
}

bool ExampleLink::init()
{
    initPVRecord();

    PVStructurePtr pvStructure = getPVRecordStructure()->getPVStructure();
    pvTimeStamp.attach(pvStructure->getSubField("timeStamp"));
    pvAlarm.attach(pvStructure->getSubField("alarm"));
    pvValue = pvStructure->getSubField<PVDoubleArray>("value");
    if(!pvValue) {
        return false;
    }
    ChannelProvider::shared_pointer provider =
        getChannelProviderRegistry()->getProvider(providerName);
    if(!provider) {
         cout << getRecordName() << " provider "
              << providerName << " does not exist" << endl;
        return false;
    }
    ChannelRequester::shared_pointer channelRequester =
        dynamic_pointer_cast<ChannelRequester>(getPtrSelf());
    channel = provider->createChannel(channelName,channelRequester);
    event.wait();
    if(!status.isOK()) {
        cout << getRecordName() << " createChannel failed "
             << status.getMessage() << endl;
        return false;
    }
    ChannelGetRequester::shared_pointer channelGetRequester =
        dynamic_pointer_cast<ChannelGetRequester>(getPtrSelf());
    PVStructurePtr pvRequest = CreateRequest::create()->createRequest(
        "value,alarm,timeStamp");
    channelGet = channel->createChannelGet(channelGetRequester,pvRequest);
    event.wait();
    if(!status.isOK()) {
        cout << getRecordName() << " createChannelGet failed "
             << status.getMessage() << endl;
        return false;
    }
    getPVValue = getPVStructure->getSubField<PVDoubleArray>("value");
    if(!getPVValue) {
        cout << getRecordName() << " get value not  PVDoubleArray" << endl;
        return false;
    }
    return true;
}

void ExampleLink::process()
{
    status = Status::Ok;
    channelGet->get();
    event.wait();
    timeStamp.getCurrent();
    pvTimeStamp.set(timeStamp);
    AlarmSeverity severity(noAlarm);
    if(!status.isOK()) {
        switch(status.getType()) {
        case Status::STATUSTYPE_OK: severity = noAlarm; break;
        case Status::STATUSTYPE_WARNING: severity = minorAlarm; break;
        case Status::STATUSTYPE_ERROR: severity = majorAlarm; break;
        case Status::STATUSTYPE_FATAL: severity = invalidAlarm; break;
        }
        alarm.setSeverity(severity);
    } else {
        pvValue->copyUnchecked(*getPVValue);
    }
    alarm.setMessage(status.getMessage());
    pvAlarm.set(alarm);
}

void ExampleLink::channelCreated(
        const Status& status,
        Channel::shared_pointer const & channel)
{
    this->status = status;
    this->channel = channel;
    event.signal();
}

void ExampleLink::channelStateChange(
        Channel::shared_pointer const & channel,
        Channel::ConnectionState connectionState)
{
}

void ExampleLink::channelGetConnect(
        const Status& status,
        ChannelGet::shared_pointer const & channelGet,
        StructureConstPtr const & structure)
{
    this->status = status;
    this->channelGet = channelGet;
    getPVStructure = getPVDataCreate()->createPVStructure(structure);
    event.signal();
}

void ExampleLink::getDone(
        const Status& status,
        ChannelGet::shared_pointer const & channelGet,
        PVStructurePtr const & pvStructure,
        BitSetPtr const & bitSet)
{
    this->status = status;
    getPVStructure->copyUnchecked(*pvStructure);
    this->bitSet = bitSet;
    event.signal();
}

}}
