/* pvaClientPut.cpp */
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

class ChannelPutRequesterImpl : public ChannelPutRequester
{
    PvaClientPut::weak_pointer pvaClientPut;
    PvaClient::weak_pointer pvaClient;
public:
    ChannelPutRequesterImpl(
        PvaClientPutPtr const & pvaClientPut,
        PvaClientPtr const &pvaClient)
    : pvaClientPut(pvaClientPut),
      pvaClient(pvaClient)
    {}
    virtual ~ChannelPutRequesterImpl() {
        if(PvaClient::getDebug()) std::cout << "~ChannelPutRequesterImpl" << std::endl;
    }

    virtual std::string getRequesterName() {
        PvaClientPutPtr clientPut(pvaClientPut.lock());
        if(!clientPut) return string("clientPut is null");
        return clientPut->getRequesterName();
    }

    virtual void message(std::string const & message, epics::pvData::MessageType messageType) {
        PvaClientPutPtr clientPut(pvaClientPut.lock());
        if(!clientPut) return;
        clientPut->message(message,messageType);
    }

    virtual void channelPutConnect(
        const Status& status,
        ChannelPut::shared_pointer const & channelPut,
        Structure::const_shared_pointer const & structure)
    {
        PvaClientPutPtr clientPut(pvaClientPut.lock());
        if(!clientPut) return;
        clientPut->channelPutConnect(status,channelPut,structure);  
    }

    virtual void getDone(
        const Status& status,
        ChannelPut::shared_pointer const & channelPut,
        PVStructurePtr const & pvStructure,
        BitSet::shared_pointer const & bitSet)
    {
        PvaClientPutPtr clientPut(pvaClientPut.lock());
        if(!clientPut) return;
        clientPut->getDone(status,channelPut,pvStructure,bitSet);
    }

        virtual void putDone(
        const Status& status,
        ChannelPut::shared_pointer const & channelPut)
    {
        PvaClientPutPtr clientPut(pvaClientPut.lock());
        if(!clientPut) return;
        clientPut->putDone(status,channelPut);
    }
};

PvaClientPutPtr PvaClientPut::create(
        PvaClientPtr const &pvaClient,
        Channel::shared_pointer const & channel,
        PVStructurePtr const &pvRequest)
{
    PvaClientPutPtr epv(new PvaClientPut(pvaClient,channel,pvRequest));
    epv->channelPutRequester = ChannelPutRequesterImplPtr(
        new ChannelPutRequesterImpl(epv,pvaClient));
    return epv;
}



PvaClientPut::PvaClientPut(
        PvaClientPtr const &pvaClient,
        Channel::shared_pointer const & channel,
        PVStructurePtr const &pvRequest)
: pvaClient(pvaClient),
  channel(channel),
  pvRequest(pvRequest),
  connectState(connectIdle),
  putState(putIdle)
{
    if(PvaClient::getDebug()) {
         cout<< "PvaClientPut::PvaClientPut"
             << " channelName " << channel->getChannelName() 
             << endl;
    }
}

PvaClientPut::~PvaClientPut()
{
    if(PvaClient::getDebug()) {
        string channelName("disconnected");
        Channel::shared_pointer chan(channel.lock());
        if(chan) channelName = chan->getChannelName();
        cout<< "PvaClientPut::~PvaClientPut"
           << " channelName " << channelName
           << endl;
    }
    if(channelPut) channelPut->destroy();
}

void PvaClientPut::checkPutState()
{
    if(connectState==connectIdle){
          connect();
          get();
    }
}

string PvaClientPut::getRequesterName()
{
     PvaClientPtr yyy = pvaClient.lock();
     if(!yyy) return string("PvaClientPut::getRequesterName() PvaClient isDestroyed");
     return yyy->getRequesterName();
}

void PvaClientPut::message(string const & message,MessageType messageType)
{
    PvaClientPtr yyy = pvaClient.lock();
    if(!yyy) return;
    yyy->message(message, messageType);
}

void PvaClientPut::channelPutConnect(
    const Status& status,
    ChannelPut::shared_pointer const & channelPut,
    StructureConstPtr const & structure)
{
    if(PvaClient::getDebug()) {
        string channelName("disconnected");
        Channel::shared_pointer chan(channel.lock());
        if(chan) channelName = chan->getChannelName();
        cout << "PvaClientPut::channelPutConnect"
           << " channelName " << channelName
           << " status.isOK " << (status.isOK() ? "true" : "false")
           << endl;
    }
    {
        Lock xx(mutex);
        channelPutConnectStatus = status;
        this->channelPut = channelPut;
        if(status.isOK()) {
            pvaClientData = PvaClientPutData::create(structure);
            pvaClientData->setMessagePrefix(channelPut->getChannel()->getChannelName());
        }
    }
    waitForConnect.signal();
    
}

void PvaClientPut::getDone(
    const Status& status,
    ChannelPut::shared_pointer const & channelPut,
    PVStructurePtr const & pvStructure,
    BitSetPtr const & bitSet)
{
    if(PvaClient::getDebug()) {
        string channelName("disconnected");
        Channel::shared_pointer chan(channel.lock());
        if(chan) channelName = chan->getChannelName();
        cout << "PvaClientPut::getDone"
           << " channelName " << channelName
           << " status.isOK " << (status.isOK() ? "true" : "false")
           << endl;
    }
    channelGetPutStatus = status;
    if(status.isOK()) {
        PVStructurePtr pvs = pvaClientData->getPVStructure();
        pvs->copyUnchecked(*pvStructure,*bitSet);
        BitSetPtr bs = pvaClientData->getChangedBitSet();
        bs->clear();
        *bs |= *bitSet;
    }
    waitForGetPut.signal();
}

void PvaClientPut::putDone(
    const Status& status,
    ChannelPut::shared_pointer const & channelPut)
{
    if(PvaClient::getDebug()) {
        string channelName("disconnected");
        Channel::shared_pointer chan(channel.lock());
        if(chan) channelName = chan->getChannelName();
        cout << "PvaClientPut::putDone"
           << " channelName " << channelName
           << " status.isOK " << (status.isOK() ? "true" : "false")
           << endl;
    }
    channelGetPutStatus = status;
    waitForGetPut.signal();
}

void PvaClientPut::connect()
{
    issueConnect();
    Status status = waitConnect();
    if(status.isOK()) return;
    Channel::shared_pointer chan(channel.lock());
    string channelName("disconnected");
    if(chan) channelName = chan->getChannelName();
    string message = string("channel ") 
        + channelName
        + " PvaClientPut::connect "
        + status.getMessage();
    throw std::runtime_error(message);
}

void PvaClientPut::issueConnect()
{
    Channel::shared_pointer chan(channel.lock());
    if(connectState!=connectIdle) {
        string channelName("disconnected");
        if(chan) channelName = chan->getChannelName();
        string message = string("channel ") + channelName
            + " pvaClientPut already connected ";
        throw std::runtime_error(message);
    }
    if(chan) {
        connectState = connectActive;
        channelPut = chan->createChannelPut(channelPutRequester,pvRequest);
        return;
    }
    throw std::runtime_error("PvaClientPut::issueConnect() but channel disconnected");
}

Status PvaClientPut::waitConnect()
{
    {
        Lock xx(mutex);
        if(connectState==connected) {
             if(!channelPutConnectStatus.isOK()) connectState = connectIdle;
             return channelPutConnectStatus;
        }
        if(connectState!=connectActive) {
            Channel::shared_pointer chan(channel.lock());
            string channelName("disconnected");
            if(chan) channelName = chan->getChannelName();
            string message = string("channel ") + channelName
                + " PvaClientPut::waitConnect illegal connect state ";
            throw std::runtime_error(message);
        }
    }
    waitForConnect.wait();
    if(!channelPutConnectStatus.isOK()) connectState = connectIdle;
    return channelPutConnectStatus;
}

void PvaClientPut::get()
{
    issueGet();
    Status status = waitGet();
    if(status.isOK()) return;
    Channel::shared_pointer chan(channel.lock());
    string channelName("disconnected");
    if(chan) channelName = chan->getChannelName();
    string message = string("channel ") 
        + channelName
        + " PvaClientPut::get "
        + status.getMessage();
    throw std::runtime_error(message);
}

void PvaClientPut::issueGet()
{
    if(connectState==connectIdle) connect();
    if(putState!=putIdle) {
        Channel::shared_pointer chan(channel.lock());
        string channelName("disconnected");
        if(chan) channelName = chan->getChannelName();
        string message = string("channel ")
            + channelName
            +  "PvaClientPut::issueGet get or put aleady active ";
        throw std::runtime_error(message);
    }
    putState = getActive;
    channelPut->get();
}

Status PvaClientPut::waitGet()
{
    if(putState!=getActive){
        Channel::shared_pointer chan(channel.lock());
        string channelName("disconnected");
        if(chan) channelName = chan->getChannelName();
        string message = string("channel ")
            + channelName
            +  " PvaClientPut::waitGet illegal put state";
        throw std::runtime_error(message);
    }
    waitForGetPut.wait();
    putState = putIdle;
    return channelGetPutStatus;
}

void PvaClientPut::put()
{
    issuePut();
    Status status = waitPut();
    if(status.isOK()) return;
    Channel::shared_pointer chan(channel.lock());
    string channelName("disconnected");
    if(chan) channelName = chan->getChannelName();
    string message = string("channel ")
        + channelName
        + " PvaClientPut::put "
        + status.getMessage();
    throw std::runtime_error(message);
}

void PvaClientPut::issuePut()
{
    if(connectState==connectIdle) connect();
    if(putState!=putIdle) {
         Channel::shared_pointer chan(channel.lock());
         string channelName("disconnected");
         if(chan) channelName = chan->getChannelName();
         string message = string("channel ")
            + channelName
            +  "PvaClientPut::issueGet get or put aleady active ";
         throw std::runtime_error(message);
    }
    putState = putActive;
    channelPut->put(pvaClientData->getPVStructure(),pvaClientData->getChangedBitSet());
}

Status PvaClientPut::waitPut()
{
    if(putState!=putActive){
         Channel::shared_pointer chan(channel.lock());
         string channelName("disconnected");
         if(chan) channelName = chan->getChannelName();
         string message = string("channel ")
            + channelName
            +  " PvaClientPut::waitPut illegal put state";
         throw std::runtime_error(message);
    }
    waitForGetPut.wait();
    putState = putIdle;
    if(channelGetPutStatus.isOK()) pvaClientData->getChangedBitSet()->clear();
    return channelGetPutStatus;
}

PvaClientPutDataPtr PvaClientPut::getData()
{
    checkPutState();
    return pvaClientData;
}


}}
