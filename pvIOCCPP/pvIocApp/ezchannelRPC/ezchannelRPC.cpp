/* ezchannelRPC.cpp */
/*
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 */
/* Author Marty Kraimer 2011.12 */


#include <ezchannelRPC.h>

namespace epics { namespace pvAccess { 

using namespace epics::pvData;

static void deleteStatic(void *)
{
// If the following is called python will crash.
    ClientFactory::stop();
}

static void initStatic(void *) {
    ClientFactory::start();
    epicsAtExit(&deleteStatic,0);
}

static epicsThreadOnceId initOnce = EPICS_THREAD_ONCE_INIT;

EZChannelRPC::EZChannelRPC(
    String const &channelName)
: channelName(channelName),
  requesterName("ezchannelRPC"),
  isOK(true)
{
    epicsThreadOnce(&initOnce, &initStatic, 0);
}

EZChannelRPC::EZChannelRPC(
    String const &channelName,
    PVStructure::shared_pointer pvRequest)
: channelName(channelName),
  pvRequest(pvRequest),
  requesterName("ezchannelRPC"),
  isOK(true)
{
    epicsThreadOnce(&initOnce, &initStatic, 0);
}

EZChannelRPC::~EZChannelRPC()
{
//printf("EZChannelRPC::~EZChannelRPC()\n");
}

void EZChannelRPC::destroy()
{
    Lock xx(mutex);
    if(channel.get()!=0) {
        channel->destroy();
        channel.reset();
        pvRequest.reset();
        channelRPC.reset();
        pvResponse.reset();
    }
}

bool EZChannelRPC::connect(double timeOut)
{
    if(pvRequest.get()==NULL) {
        pvRequest = getCreateRequest()->createRequest(
            "record[process=true]field()",getPtrSelf());
    }
    issueConnect();
    return waitConnect(timeOut);
}

void EZChannelRPC::issueConnect()
{
    event.tryWait(); // make sure event is empty
    ChannelAccess::shared_pointer const &channelAccess = getChannelAccess();
    ChannelProvider::shared_pointer const &channelProvider
         = channelAccess->getProvider(String("pvAccess"));
    channel = channelProvider->createChannel(
        channelName,
        getPtrSelf(),
        ChannelProvider::PRIORITY_DEFAULT);
}

bool EZChannelRPC::waitConnect(double timeOut) {
    // wait for channel to connect
    bool ok = event.wait(timeOut);
    if(!ok) return ok;
    channelRPC = channel->createChannelRPC(getPtrSelf(),pvRequest);
    event.wait();
    return isOK;
}

epics::pvData::PVStructure::shared_pointer EZChannelRPC::request(
    PVStructure::shared_pointer const & pvArgument,
    bool lastRequest)
{
//printf("EZChannelRPC::request\n");
    issueRequest(pvArgument,lastRequest);
//printf("EZChannelRPC::request calling waitRequest\n");
    return waitRequest();
}

void EZChannelRPC::issueRequest(
    PVStructure::shared_pointer const & pvArgument,
    bool lastRequest)
{
//printf("EZChannelRPC::issueRequest\n");
    event.tryWait(); // make sure event is empty
//printf("EZChannelRPC::issueRequest calling channelRPC->request\n");
    channelRPC->request(pvArgument,lastRequest);
}

epics::pvData::PVStructure::shared_pointer EZChannelRPC::waitRequest()
{
//printf("calling event.wait()\n");
    bool ok = event.wait();
//printf("wait returned %s\n",(ok==true ? "true" : "false"));
    if(!ok) {
        isOK = false;
        lastMessage = "event.wait failed\n";
        pvResponse = epics::pvData::PVStructure::shared_pointer();
    }
    return pvResponse;
}

String EZChannelRPC::getMessage() { return lastMessage;}


void EZChannelRPC::channelCreated(
    const Status& status,
    Channel::shared_pointer const & channel)
{
//printf("EZChannelRPC::channelCreate\n");
    isOK = status.isOK();
    this->channel = channel;
    if(!isOK) {
        String message = Status::StatusTypeName[status.getType()];
        message += " " + status.getMessage();
        lastMessage = message;
        event.signal();
    }
}

void EZChannelRPC::channelStateChange(
    Channel::shared_pointer const & channel,
    Channel::ConnectionState connectionState)
{
    this->channel = channel;
    this->connectionState = connectionState;
    event.signal();
}

String EZChannelRPC::getRequesterName(){ return requesterName;}

void EZChannelRPC::message(String const &message,MessageType messageType)
{
    lastMessage = getMessageTypeName(messageType);
    lastMessage += " " + message;
}

void EZChannelRPC::channelRPCConnect(
    const Status& status,
    ChannelRPC::shared_pointer const & channelRPC)
{
    this->channelRPC = channelRPC;
    if(!status.isOK()) {
        isOK = false;
        String message = Status::StatusTypeName[status.getType()];
        message += " " + status.getMessage();
        lastMessage = message;
    }
    event.signal();
}

void EZChannelRPC::requestDone(
    const Status& status,
    PVStructure::shared_pointer const & pvResponse)
{
//printf("EZChannelRPC::requestDone\n");
    this->pvResponse = pvResponse;
    if(!status.isOK()) {
        isOK = false;
        String message = Status::StatusTypeName[status.getType()];
        message += " " + status.getMessage();
        lastMessage = message;
    }
    event.signal();
}
    

}}
