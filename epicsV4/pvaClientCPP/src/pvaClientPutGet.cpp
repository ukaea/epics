/* pvaClientPutGet.cpp */
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

class ChannelPutGetRequesterImpl : public ChannelPutGetRequester
{
    PvaClientPutGet::weak_pointer pvaClientPutGet;
    PvaClient::weak_pointer pvaClient;
public:
    ChannelPutGetRequesterImpl(
        PvaClientPutGetPtr const & pvaClientPutGet,
        PvaClientPtr const &pvaClient)
    : pvaClientPutGet(pvaClientPutGet),
      pvaClient(pvaClient)
    {}
    virtual ~ChannelPutGetRequesterImpl() {
        if(PvaClient::getDebug()) std::cout << "~ChannelPutGetRequesterImpl" << std::endl;
    }

    virtual std::string getRequesterName() {
        PvaClientPutGetPtr clientPutGet(pvaClientPutGet.lock());
        if(!clientPutGet) return string("clientPutGet is null");
        return clientPutGet->getRequesterName();
    }

    virtual void message(std::string const & message, epics::pvData::MessageType messageType) {
        PvaClientPutGetPtr clientPutGet(pvaClientPutGet.lock());
        if(!clientPutGet) return;
        clientPutGet->message(message,messageType);
    }

    virtual void channelPutGetConnect(
        const Status& status,
        ChannelPutGet::shared_pointer const & channelPutGet,
        Structure::const_shared_pointer const & putStructure,
        Structure::const_shared_pointer const & getStructure)
    {
        PvaClientPutGetPtr clientPutGet(pvaClientPutGet.lock());
        if(!clientPutGet) return;
        clientPutGet->channelPutGetConnect(status,channelPutGet,putStructure,getStructure);  
    }

    virtual void putGetDone(
        const Status& status,
        ChannelPutGet::shared_pointer const & channelPutGet,
        PVStructurePtr const & getPVStructure,
        BitSet::shared_pointer const & getBitSet)
    {
        PvaClientPutGetPtr clientPutGet(pvaClientPutGet.lock());
        if(!clientPutGet) return;
        clientPutGet->putGetDone(status,channelPutGet,getPVStructure,getBitSet);
    }

    virtual void getPutDone(
        const Status& status,
        ChannelPutGet::shared_pointer const & channelPutGet,
        PVStructurePtr const & putPVStructure,
        BitSet::shared_pointer const & putBitSet)
    {
        PvaClientPutGetPtr clientPutGet(pvaClientPutGet.lock());
        if(!clientPutGet) return;
        clientPutGet->getPutDone(status,channelPutGet,putPVStructure,putBitSet);
    }


    virtual void getGetDone(
        const Status& status,
        ChannelPutGet::shared_pointer const & channelPutGet,
        PVStructurePtr const & getPVStructure,
        BitSet::shared_pointer const & getBitSet)
    {
        PvaClientPutGetPtr clientPutGet(pvaClientPutGet.lock());
        if(!clientPutGet) return;
        clientPutGet->getGetDone(status,channelPutGet,getPVStructure,getBitSet);
    }
};

PvaClientPutGetPtr PvaClientPutGet::create(
        PvaClientPtr const &pvaClient,
        Channel::shared_pointer const & channel,
        PVStructurePtr const &pvRequest)
{
    PvaClientPutGetPtr epv(new PvaClientPutGet(pvaClient,channel,pvRequest));
    epv->channelPutGetRequester = ChannelPutGetRequesterImplPtr(
        new ChannelPutGetRequesterImpl(epv,pvaClient));
    return epv;
}

PvaClientPutGet::PvaClientPutGet(
        PvaClientPtr const &pvaClient,
        Channel::shared_pointer const & channel,
        PVStructurePtr const &pvRequest)
: pvaClient(pvaClient),
  channel(channel),
  pvRequest(pvRequest),
  connectState(connectIdle),
  putGetState(putGetIdle)
{
    if(PvaClient::getDebug()) {
         cout<< "PvaClientPutGet::PvaClientPutGet"
             << " channelName " << channel->getChannelName() 
             << endl;
    }
}

PvaClientPutGet::~PvaClientPutGet()
{
    if(PvaClient::getDebug()) {
        string channelName("disconnected");
        Channel::shared_pointer chan(channel.lock());
        if(chan) channelName = chan->getChannelName();
        cout<< "PvaClientPutGet::~PvaClientPutGet"
           << " channelName " << channelName
           << endl;
    }
    channelPutGet->destroy();
}

void PvaClientPutGet::checkPutGetState()
{
    if(connectState==connectIdle){
        connect();
        getPut();
    }
}

string PvaClientPutGet::getRequesterName()
{
     PvaClientPtr yyy = pvaClient.lock();
     if(!yyy) return string("PvaClientPutGet::getRequesterName() PvaClient isDestroyed");
     return yyy->getRequesterName();
}

void PvaClientPutGet::message(string const & message,MessageType messageType)
{
    PvaClientPtr yyy = pvaClient.lock();
    if(!yyy) return;
    yyy->message(message, messageType);
}

void PvaClientPutGet::channelPutGetConnect(
    const Status& status,
    ChannelPutGet::shared_pointer const & channelPutGet,
    StructureConstPtr const & putStructure,
    StructureConstPtr const & getStructure)
{
    if(PvaClient::getDebug()) {
        cout << "PvaClientPutGet::channelPutGetConnect"
           << " channelName " << channelPutGet->getChannel()->getChannelName()
           << " status.isOK " << (status.isOK() ? "true" : "false")
           << endl;
    }
    channelPutGetConnectStatus = status;
    this->channelPutGet = channelPutGet;
    if(status.isOK()) {
        pvaClientPutData = PvaClientPutData::create(putStructure);
        pvaClientPutData->setMessagePrefix(channelPutGet->getChannel()->getChannelName());
        pvaClientGetData = PvaClientGetData::create(getStructure);
        pvaClientGetData->setMessagePrefix(channelPutGet->getChannel()->getChannelName());
    }
    waitForConnect.signal();
    
}

void PvaClientPutGet::putGetDone(
        const Status& status,
        ChannelPutGet::shared_pointer const & channelPutGet,
        PVStructurePtr const & getPVStructure,
        BitSetPtr const & getChangedBitSet)
{
    if(PvaClient::getDebug()) {
        string channelName("disconnected");
        Channel::shared_pointer chan(channel.lock());
        if(chan) channelName = chan->getChannelName();
        cout << "PvaClientPutGet::putGetDone"
           << " channelName " << channelName
           << " status.isOK " << (status.isOK() ? "true" : "false")
           << endl;
    }
    channelPutGetStatus = status;
    if(status.isOK()) {
        pvaClientGetData->setData(getPVStructure,getChangedBitSet);
    }
    waitForPutGet.signal();
}

void PvaClientPutGet::getPutDone(
    const Status& status,
    ChannelPutGet::shared_pointer const & channelPutGet,
    PVStructurePtr const & putPVStructure,
    BitSetPtr const & putBitSet)
{
    if(PvaClient::getDebug()) {
        string channelName("disconnected");
        Channel::shared_pointer chan(channel.lock());
        if(chan) channelName = chan->getChannelName();
        cout << "PvaClientPutGet::getPutDone"
           << " channelName " << channelName
           << " status.isOK " << (status.isOK() ? "true" : "false")
           << endl;
    }
    channelPutGetStatus = status;
    if(status.isOK()) {
        PVStructurePtr pvs = pvaClientPutData->getPVStructure();
        pvs->copyUnchecked(*putPVStructure,*putBitSet);
        BitSetPtr bs = pvaClientPutData->getChangedBitSet();
        bs->clear();
        *bs |= *putBitSet;
    }
    waitForPutGet.signal();
}

void PvaClientPutGet::getGetDone(
        const Status& status,
        ChannelPutGet::shared_pointer const & channelPutGet,
        PVStructurePtr const & getPVStructure,
        BitSetPtr const & getChangedBitSet)
{
    if(PvaClient::getDebug()) {
        string channelName("disconnected");
        Channel::shared_pointer chan(channel.lock());
        if(chan) channelName = chan->getChannelName();
        cout << "PvaClientPutGet::getGetDone"
           << " channelName " << channelName
           << " status.isOK " << (status.isOK() ? "true" : "false")
           << endl;
    }
    channelPutGetStatus = status;
    if(status.isOK()) {
        pvaClientGetData->setData(getPVStructure,getChangedBitSet);
    }
    waitForPutGet.signal();
}

void PvaClientPutGet::connect()
{
    issueConnect();
    Status status = waitConnect();
    if(status.isOK()) return;
    Channel::shared_pointer chan(channel.lock());
    string channelName("disconnected");
    if(chan) channelName = chan->getChannelName();
    string message = string("channel ") 
        + channelName
        + " PvaClientPutGet::connect "
        + status.getMessage();
    throw std::runtime_error(message);
}

void PvaClientPutGet::issueConnect()
{
    Channel::shared_pointer chan(channel.lock());
    if(connectState!=connectIdle) {
        string channelName("disconnected");
        if(chan) channelName = chan->getChannelName();
        string message = string("channel ") + channelName
            + " pvaClientPutGet already connected ";
        throw std::runtime_error(message);
    }
    if(chan) {
        connectState = connectActive;
        channelPutGet = chan->createChannelPutGet(channelPutGetRequester,pvRequest);
        return;
    }
    throw std::runtime_error("PvaClientPutGet::issueConnect() but channel disconnected");
}

Status PvaClientPutGet::waitConnect()
{
    {
         Lock xx(mutex);
         if(connectState==connected) {
             if(!channelPutGetConnectStatus.isOK()) connectState = connectIdle;
             return channelPutGetConnectStatus;
        }
        if(connectState!=connectActive)  {
            Channel::shared_pointer chan(channel.lock());
            string channelName("disconnected");
            if(chan) channelName = chan->getChannelName();
            string message = string("channel ") + channelName
                + " PvaClientPutGet::waitConnect illegal connect state ";
            throw std::runtime_error(message);
        }
    }
    waitForConnect.wait();
    connectState = channelPutGetConnectStatus.isOK() ? connected : connectIdle;
    return channelPutGetConnectStatus;
}


void PvaClientPutGet::putGet()
{
    issuePutGet();
    Status status = waitPutGet();
    if(status.isOK()) return;
    Channel::shared_pointer chan(channel.lock());
    string channelName("disconnected");
    if(chan) channelName = chan->getChannelName();
    string message = string("channel ") 
        + channelName
        + " PvaClientPut::putGet "
        + status.getMessage();
    throw std::runtime_error(message);
}

void PvaClientPutGet::issuePutGet()
{
    if(connectState==connectIdle) connect();
    if(putGetState!=putGetIdle) {
        Channel::shared_pointer chan(channel.lock());
        string channelName("disconnected");
        if(chan) channelName = chan->getChannelName();
        string message = string("channel ") + channelName
            + " PvaClientPutGet::issuePutGet get or put aleady active ";
        throw std::runtime_error(message);
    }
    putGetState = putGetActive;
    channelPutGet->putGet(pvaClientPutData->getPVStructure(),pvaClientPutData->getChangedBitSet());
}


Status PvaClientPutGet::waitPutGet()
{
    if(putGetState!=putGetActive){
        Channel::shared_pointer chan(channel.lock());
        string channelName("disconnected");
        if(chan) channelName = chan->getChannelName();
        string message = string("channel ") + channelName
            + " PvaClientPutGet::waitPutGet get or put aleady active ";
        throw std::runtime_error(message);
    }
    waitForPutGet.wait();
    putGetState = putGetIdle;
    return channelPutGetStatus;
}

void PvaClientPutGet::getGet()
{
    issueGetGet();
    Status status = waitGetGet();
    if(status.isOK()) return;
    Channel::shared_pointer chan(channel.lock());
    string channelName("disconnected");
    if(chan) channelName = chan->getChannelName();
    string message = string("channel ") 
        + channelName
        + " PvaClientPut::getGet "
        + status.getMessage();
    throw std::runtime_error(message);
}

void PvaClientPutGet::issueGetGet()
{
    if(connectState==connectIdle) connect();
    if(putGetState!=putGetIdle) {
        Channel::shared_pointer chan(channel.lock());
        string channelName("disconnected");
        if(chan) channelName = chan->getChannelName();
        string message = string("channel ") + channelName
            + " PvaClientPutGet::issueGetGet get or put aleady active ";
        throw std::runtime_error(message);
    }
    putGetState = putGetActive;
    channelPutGet->getGet();
}

Status PvaClientPutGet::waitGetGet()
{
    if(putGetState!=putGetActive){
        Channel::shared_pointer chan(channel.lock());
        string channelName("disconnected");
        if(chan) channelName = chan->getChannelName();
        string message = string("channel ") + channelName
            + " PvaClientPutGet::waitGetGet get or put aleady active ";
        throw std::runtime_error(message);
    }
    waitForPutGet.wait();
    putGetState = putGetIdle;
    return channelPutGetStatus;
}

void PvaClientPutGet::getPut()
{
    issueGetPut();
    Status status = waitGetPut();
    if(status.isOK()) return;
    Channel::shared_pointer chan(channel.lock());
    string channelName("disconnected");
    if(chan) channelName = chan->getChannelName();
    string message = string("channel ") 
        + channelName
        + " PvaClientPut::getPut "
        + status.getMessage();
    throw std::runtime_error(message);
}

void PvaClientPutGet::issueGetPut()
{
    if(connectState==connectIdle) connect();
    if(putGetState!=putGetIdle) {
        Channel::shared_pointer chan(channel.lock());
        string channelName("disconnected");
        if(chan) channelName = chan->getChannelName();
        string message = string("channel ") + channelName
            + " PvaClientPutGet::issueGetPut get or put aleady active ";
        throw std::runtime_error(message);
    }
    putGetState = putGetActive;
    channelPutGet->getPut();
}

Status PvaClientPutGet::waitGetPut()
{
    if(putGetState!=putGetActive){
        Channel::shared_pointer chan(channel.lock());
        string channelName("disconnected");
        if(chan) channelName = chan->getChannelName();
        string message = string("channel ") + channelName
            + " PvaClientPutGet::waitGetPut get or put aleady active ";
        throw std::runtime_error(message);
    }
    waitForPutGet.wait();
    putGetState = putGetIdle;
    return channelPutGetStatus;
}

PvaClientGetDataPtr PvaClientPutGet::getGetData()
{
    checkPutGetState();
    return pvaClientGetData;
}

PvaClientPutDataPtr PvaClientPutGet::getPutData()
{
    checkPutGetState();
    return pvaClientPutData;
}

}}
