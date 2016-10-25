/* pvaClientProcess.cpp */
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

class ChannelProcessRequesterImpl : public ChannelProcessRequester
{
    PvaClientProcess::weak_pointer pvaClientProcess;
    PvaClient::weak_pointer pvaClient;
public:
    ChannelProcessRequesterImpl(
        PvaClientProcessPtr const & pvaClientProcess,
        PvaClientPtr const &pvaClient)
    : pvaClientProcess(pvaClientProcess),
      pvaClient(pvaClient)
    {}
    virtual ~ChannelProcessRequesterImpl() {
        if(PvaClient::getDebug()) std::cout << "~ChannelProcessRequesterImpl" << std::endl;
    }

    virtual std::string getRequesterName() {
        PvaClientProcessPtr clientProcess(pvaClientProcess.lock());
        if(!clientProcess) return string("clientProcess is null");
        return clientProcess->getRequesterName();
    }

    virtual void message(std::string const & message, epics::pvData::MessageType messageType) {
        PvaClientProcessPtr clientProcess(pvaClientProcess.lock());
        if(!clientProcess) return;
        clientProcess->message(message,messageType);
    }

    virtual void channelProcessConnect(
        const Status& status,
        ChannelProcess::shared_pointer const & channelProcess)
    {
        PvaClientProcessPtr clientProcess(pvaClientProcess.lock());
        if(!clientProcess) return;
        clientProcess->channelProcessConnect(status,channelProcess);  
    }

    virtual void processDone(
        const Status& status,
        ChannelProcess::shared_pointer const & ChannelProcess)
    {
        PvaClientProcessPtr clientProcess(pvaClientProcess.lock());
        if(!clientProcess) return;
        clientProcess->processDone(status,ChannelProcess);
    }
};

PvaClientProcessPtr PvaClientProcess::create(
        PvaClientPtr const &pvaClient,
        Channel::shared_pointer const & channel,
        PVStructurePtr const &pvRequest)
{
    PvaClientProcessPtr epv(new PvaClientProcess(pvaClient,channel,pvRequest));
    epv->channelProcessRequester = ChannelProcessRequesterImplPtr(
        new ChannelProcessRequesterImpl(epv,pvaClient));
    return epv;
}


PvaClientProcess::PvaClientProcess(
        PvaClientPtr const &pvaClient,
        Channel::shared_pointer const & channel,
        PVStructurePtr const &pvRequest)
: pvaClient(pvaClient),
  channel(channel),
  pvRequest(pvRequest),
  connectState(connectIdle),
  processState(processIdle)
{
    if(PvaClient::getDebug()) cout<< "PvaClientProcess::PvaClientProcess()\n";
}

PvaClientProcess::~PvaClientProcess()
{
    if(PvaClient::getDebug()) cout<< "PvaClientProcess::~PvaClientProcess()\n";
    channelProcess->destroy();
}

// from ChannelProcessRequester
string PvaClientProcess::getRequesterName()
{
     PvaClientPtr yyy = pvaClient.lock();
     if(!yyy) throw std::runtime_error("pvaClient was destroyed");
     return yyy->getRequesterName();
}

void PvaClientProcess::message(string const & message,MessageType messageType)
{
    PvaClientPtr yyy = pvaClient.lock();
    if(!yyy) throw std::runtime_error("pvaClient was destroyed");
    yyy->message(message, messageType);
}

void PvaClientProcess::channelProcessConnect(
    const Status& status,
    ChannelProcess::shared_pointer const & channelProcess)
{
    channelProcessConnectStatus = status;
    connectState = connected;
    this->channelProcess = channelProcess;
    waitForConnect.signal();
    
}

void PvaClientProcess::processDone(
    const Status& status,
    ChannelProcess::shared_pointer const & channelProcess)
{
    channelProcessStatus = status;
    processState = processComplete;
    waitForProcess.signal();
}

void PvaClientProcess::connect()
{
    issueConnect();
    Status status = waitConnect();
    if(status.isOK()) return;
    string message = string("channel ") + channel->getChannelName()
        + " PvaClientProcess::connect " + status.getMessage();
    throw std::runtime_error(message);
}

void PvaClientProcess::issueConnect()
{
    if(connectState!=connectIdle) {
        string message = string("channel ") + channel->getChannelName()
            + " pvaClientProcess already connected ";
        throw std::runtime_error(message);
    }
    connectState = connectActive;
    channelProcess = channel->createChannelProcess(channelProcessRequester,pvRequest);
}

Status PvaClientProcess::waitConnect()
{
    if(connectState==connected) {
         if(!channelProcessConnectStatus.isOK()) connectState = connectIdle;
         return channelProcessConnectStatus;
    }
    if(connectState!=connectActive) {
        string message = string("channel ") + channel->getChannelName()
            + " pvaClientProcess illegal connect state ";
        throw std::runtime_error(message);
    }
    waitForConnect.wait();
    if(!channelProcessConnectStatus.isOK()) connectState = connectIdle;
    return channelProcessConnectStatus;
}

void PvaClientProcess::process()
{
    issueProcess();
    Status status = waitProcess();
    if(status.isOK()) return;
    string message = string("channel ") + channel->getChannelName()
        + " PvaClientProcess::process" + status.getMessage();
    throw std::runtime_error(message);
}

void PvaClientProcess::issueProcess()
{
    if(connectState==connectIdle) connect();
    if(processState!=processIdle) {
        string message = string("channel ") + channel->getChannelName()
            + " PvaClientProcess::issueProcess process aleady active ";
        throw std::runtime_error(message);
    }
    processState = processActive;
    channelProcess->process();
}

Status PvaClientProcess::waitProcess()
{
    if(processState==processComplete) {
        processState = processIdle;
        return channelProcessStatus;
    }
    if(processState!=processActive){
        string message = string("channel ") + channel->getChannelName()
            + " PvaClientProcess::waitProcess llegal process state";
        throw std::runtime_error(message);
    }
    waitForProcess.wait();
    processState = processIdle;
    return channelProcessStatus;
}

}}
