/* pvaClientGet.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 * @date 2015.02
 */

#include <pv/event.h>

#define epicsExportSharedSymbols

#include <pv/pvaClient.h>

using std::tr1::static_pointer_cast;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace std;

namespace epics { namespace pvaClient {

class ChannelGetRequesterImpl : public ChannelGetRequester
{
    PvaClientGet::weak_pointer pvaClientGet;
    PvaClient::weak_pointer pvaClient;
public:
    ChannelGetRequesterImpl(
        PvaClientGetPtr const & pvaClientGet,
        PvaClientPtr const &pvaClient)
    : pvaClientGet(pvaClientGet),
      pvaClient(pvaClient)
    {}
    virtual ~ChannelGetRequesterImpl() {
        if(PvaClient::getDebug()) std::cout << "~ChannelGetRequesterImpl" << std::endl;
    }

    virtual std::string getRequesterName() {
        PvaClientGetPtr clientGet(pvaClientGet.lock());
        if(!clientGet) return string("clientGet is null");
        return clientGet->getRequesterName();
    }

    virtual void message(std::string const & message, epics::pvData::MessageType messageType) {
        PvaClientGetPtr clientGet(pvaClientGet.lock());
        if(!clientGet) return;
        clientGet->message(message,messageType);
    }

    virtual void channelGetConnect(
        const Status& status,
        ChannelGet::shared_pointer const & channelGet,
        Structure::const_shared_pointer const & structure)
    {
        PvaClientGetPtr clientGet(pvaClientGet.lock());
        if(!clientGet) return;
        clientGet->channelGetConnect(status,channelGet,structure);  
    }

    virtual void getDone(
        const Status& status,
        ChannelGet::shared_pointer const & channelGet,
        PVStructurePtr const & pvStructure,
        BitSet::shared_pointer const & bitSet)
    {
        PvaClientGetPtr clientGet(pvaClientGet.lock());
        if(!clientGet) return;
        clientGet->getDone(status,channelGet,pvStructure,bitSet);
    }
};

PvaClientGetPtr PvaClientGet::create(
        PvaClientPtr const &pvaClient,
        Channel::shared_pointer const & channel,
        PVStructurePtr const &pvRequest)
{
    PvaClientGetPtr epv(new PvaClientGet(pvaClient,channel,pvRequest));
    epv->channelGetRequester = ChannelGetRequesterImplPtr(
        new ChannelGetRequesterImpl(epv,pvaClient));
    return epv;
}


PvaClientGet::PvaClientGet(
        PvaClientPtr const &pvaClient,
        Channel::shared_pointer const & channel,
        PVStructurePtr const &pvRequest)
: pvaClient(pvaClient),
  channel(channel),
  pvRequest(pvRequest),
  connectState(connectIdle),
  getState(getIdle)
{
     if(PvaClient::getDebug()) {
        cout << "PvaClientGet::PvaClientGet::PvaClientGet"
           << " channelName " << channel->getChannelName()
           << endl;
    }
}

PvaClientGet::~PvaClientGet()
{
    if(PvaClient::getDebug()) {
        string channelName("disconnected");
        Channel::shared_pointer chan(channel.lock());
        if(chan) channelName = chan->getChannelName();
         cout<< "PvaClientGet::~PvaClientGet"
           << " channelName " << channelName
           << endl;
    }
    if(channelGet) channelGet->destroy();
}

void PvaClientGet::checkGetState()
{
    if(connectState==connectIdle) connect();
    if(getState==getIdle) get();
}

// from ChannelGetRequester
string PvaClientGet::getRequesterName()
{
    PvaClientPtr yyy = pvaClient.lock();
    if(!yyy) return string("PvaClientGet::getRequesterName PvaClient isDestroyed");
    return yyy->getRequesterName();
}

void PvaClientGet::message(string const & message,MessageType messageType)
{
    PvaClientPtr yyy = pvaClient.lock();
    if(!yyy) return;
    yyy->message(message, messageType);
}

void PvaClientGet::channelGetConnect(
    const Status& status,
    ChannelGet::shared_pointer const & channelGet,
    StructureConstPtr const & structure)
{
    if(PvaClient::getDebug()) {
        string channelName("disconnected");
        Channel::shared_pointer chan(channel.lock());
        if(chan) channelName = chan->getChannelName();
        cout << "PvaClientGet::channelGetConnect"
           << " channelName " << channelName
           << " status.isOK " << (status.isOK() ? "true" : "false")
           << endl;
    }
    {
        Lock xx(mutex);
        channelGetConnectStatus = status;
        connectState = connected;
        this->channelGet = channelGet;
        if(status.isOK()) {
            pvaClientData = PvaClientGetData::create(structure);
            pvaClientData->setMessagePrefix(channelGet->getChannel()->getChannelName());
        }
    }
    waitForConnect.signal();
    
}

void PvaClientGet::getDone(
    const Status& status,
    ChannelGet::shared_pointer const & channelGet,
    PVStructurePtr const & pvStructure,
    BitSetPtr const & bitSet)
{
    if(PvaClient::getDebug()) {
        string channelName("disconnected");
        Channel::shared_pointer chan(channel.lock());
        if(chan) channelName = chan->getChannelName();
        cout << "PvaClientGet::getDone"
           << " channelName " << channelName
           << " status.isOK " << (status.isOK() ? "true" : "false")
           << endl;
    }
    {
        Lock xx(mutex);
        channelGetStatus = status;
        if(status.isOK()) {
            pvaClientData->setData(pvStructure,bitSet);
        }
    }
    waitForGet.signal();
}

void PvaClientGet::connect()
{
    issueConnect();
    Status status = waitConnect();
    if(status.isOK()) return;
    Channel::shared_pointer chan(channel.lock());
    string channelName("disconnected");
    if(chan) channelName = chan->getChannelName();
    string message = string("channel ") + channelName 
         + " PvaClientGet::connect " + status.getMessage();
    throw std::runtime_error(message);
}

void PvaClientGet::issueConnect()
{
    Channel::shared_pointer chan(channel.lock());
    if(connectState!=connectIdle) {
        string channelName("disconnected");
        if(chan) channelName = chan->getChannelName();
        string message = string("channel ") + channelName
            + " pvaClientGet already connected ";
        throw std::runtime_error(message);
    }
    if(chan) {
        connectState = connectActive;
        channelGet = chan->createChannelGet(channelGetRequester,pvRequest);
        return;
    }
    throw std::runtime_error("PvaClientGet::issueConnect channel was destroyed");
}

Status PvaClientGet::waitConnect()
{
    {
        Lock xx(mutex);
        if(connectState==connected) {
             if(!channelGetConnectStatus.isOK()) connectState = connectIdle;
             return channelGetConnectStatus;
        }
        if(connectState!=connectActive) {
            Channel::shared_pointer chan(channel.lock());
            string channelName("disconnected");
            if(chan) channelName = chan->getChannelName();
            string message = string("channel ") + channelName
                + " PvaClientGet::waitConnect illegal connect state ";
            throw std::runtime_error(message);
        }
    }
    waitForConnect.wait();
    if(!channelGetConnectStatus.isOK()) connectState = connectIdle;
    return channelGetConnectStatus;
}

void PvaClientGet::get()
{
    issueGet();
    Status status = waitGet();
    if(status.isOK()) return;
    Channel::shared_pointer chan(channel.lock());
    string channelName("disconnected");
    if(chan) channelName = chan->getChannelName();
    string message = string("channel ") + channelName
            + " PvaClientGet::get " + status.getMessage();
    throw std::runtime_error(message);
}

void PvaClientGet::issueGet()
{
    if(connectState==connectIdle) connect();
    if(getState!=getIdle) {
        Channel::shared_pointer chan(channel.lock());
        string channelName("disconnected");
        if(chan) channelName = chan->getChannelName();
        string message = string("channel ") + channelName
            + " PvaClientGet::issueGet get aleady active ";
        throw std::runtime_error(message);
    }
    getState = getActive;
    channelGet->get();
}

Status PvaClientGet::waitGet()
{
    {
        Lock xx(mutex);
        if(getState==getComplete) {
            getState =getIdle;
            return channelGetStatus;
        }
        if(getState!=getActive){
            Channel::shared_pointer chan(channel.lock());
            string channelName("disconnected");
            if(chan) channelName = chan->getChannelName();
            string message = string("channel ") + channelName 
                + " PvaClientGet::waitGet llegal get state";
            throw std::runtime_error(message);
        }
    }
    waitForGet.wait();
    getState = getIdle;
    return channelGetStatus;
}
PvaClientGetDataPtr PvaClientGet::getData()
{
    checkGetState();
    return pvaClientData;
}


}}
