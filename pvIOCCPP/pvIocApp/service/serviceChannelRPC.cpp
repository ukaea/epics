/* serviceChannelRPC.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 */
#include <string>
#include <stdexcept>
#include <memory>

#include <pv/service.h>
#include <pv/pvServiceProvider.h>

namespace epics { namespace pvIOC { 

using namespace epics::pvData;
using namespace epics::pvAccess;

class PVTop;
class ChannelImpl;
class ChannelRPCImpl;


class PVTop :
    public ServicePVTop,
    public std::tr1::enable_shared_from_this<PVTop>
{
public:
    POINTER_DEFINITIONS(PVTop);
    PVTop(String channelName,ServiceRPC::shared_pointer const &serviceRPC);
    virtual ~PVTop();
    virtual String getName();
    virtual ChannelBase::shared_pointer createChannel(
        ChannelRequester::shared_pointer const &requester,
        ChannelProvider::shared_pointer const &provider);
    virtual void destroy();
private:
    shared_pointer getPtrSelf()
    {
        return shared_from_this();
    }
    String channelName;
    ServiceRPC::shared_pointer serviceRPC;
};

class ChannelImpl :
    public ChannelBase
{
public:
    POINTER_DEFINITIONS(ChannelImpl);
    ChannelImpl(
        ChannelProvider::shared_pointer const & channelProvider,
        ChannelRequester::shared_pointer const & requester,
        String name,
        ServiceRPC::shared_pointer const &serviceRPC);
    virtual ~ChannelImpl();
    virtual epics::pvAccess::ChannelRPC::shared_pointer createChannelRPC(
        ChannelRPCRequester::shared_pointer const &channelRPCRequester,
        PVStructure::shared_pointer const &pvRequest);
    virtual void printInfo();
    virtual void printInfo(StringBuilder out);
private:
    ServiceRPC::shared_pointer serviceRPC;
};

class ChannelRPCImpl :
  public epics::pvAccess::ChannelRPC,
  public std::tr1::enable_shared_from_this<ChannelRPCImpl>
{
public:
    POINTER_DEFINITIONS(ChannelRPCImpl);
    ChannelRPCImpl(
        ChannelBase::shared_pointer const & channel,
        ChannelRPCRequester::shared_pointer const &channelRPCRequester,
        ServiceRPC::shared_pointer const &serviceRPC);
    virtual ~ChannelRPCImpl();
    bool init(PVStructure::shared_pointer const & pvRequest);
    virtual String getRequesterName();
    virtual void message(
        String message,
        MessageType messageType);
    virtual void destroy();
    virtual void request(
        PVStructure::shared_pointer const & pvArgument,bool lastRequest);
    virtual void lock();
    virtual void unlock();
private:
    shared_pointer getPtrSelf()
    {
        return shared_from_this();
    }
    ChannelBase::shared_pointer channel;
    ChannelRPCRequester::shared_pointer channelRPCRequester;
    ServiceRPC::shared_pointer serviceRPC;
    epics::pvData::Mutex dataMutex;
};


PVTop::PVTop(String channelName,ServiceRPC::shared_pointer const &serviceRPC)
: channelName(channelName),
  serviceRPC(serviceRPC)
{}

PVTop::~PVTop()
{
}

String PVTop::getName() {return channelName;}

ChannelBase::shared_pointer PVTop::createChannel(
    ChannelRequester::shared_pointer const &requester,
    ChannelProvider::shared_pointer const &provider)
{
    return ChannelBase::shared_pointer(
        new ChannelImpl(provider,requester,channelName,serviceRPC));
}

void PVTop::destroy()
{
}

ChannelImpl::ChannelImpl(
    ChannelProvider::shared_pointer const & channelProvider,
    ChannelRequester::shared_pointer const & requester,
    String name,
    ServiceRPC::shared_pointer const &serviceRPC)
: ChannelBase(channelProvider,requester,name),
  serviceRPC(serviceRPC)
{
}

ChannelImpl::~ChannelImpl()
{
}

epics::pvAccess::ChannelRPC::shared_pointer ChannelImpl::createChannelRPC(
    ChannelRPCRequester::shared_pointer const &channelRPCRequester,
    PVStructure::shared_pointer const &pvRequest)
{
    ChannelRPC *channel = new ChannelRPCImpl(getPtrSelf(),channelRPCRequester,serviceRPC);
    ChannelRPC::shared_pointer channelRPC(channel);
    addChannelRPC(channelRPC);
    channelRPCRequester->channelRPCConnect(Status::Ok,channelRPC);
    return channelRPC;
}

void ChannelImpl::printInfo(){}
void ChannelImpl::printInfo(StringBuilder out){}

ChannelRPCImpl::ChannelRPCImpl(
    ChannelBase::shared_pointer const & channel,
    ChannelRPCRequester::shared_pointer const &channelRPCRequester,
    ServiceRPC::shared_pointer const &serviceRPC)
: channel(channel),
  channelRPCRequester(channelRPCRequester),
  serviceRPC(serviceRPC)
{
}
 
ChannelRPCImpl::~ChannelRPCImpl()
{
}

String ChannelRPCImpl::getRequesterName() 
{
    return channelRPCRequester->getRequesterName();
}

void ChannelRPCImpl::message(String message,MessageType messageType)
{
    channelRPCRequester->message(message,messageType);
}

void ChannelRPCImpl::destroy() {
    channel->removeChannelRPC(getPtrSelf());
}

void ChannelRPCImpl::request(
        PVStructure::shared_pointer const & pvArgument,bool lastRequest)
{
    {
        Lock lock(dataMutex);
        serviceRPC->request(channelRPCRequester,pvArgument);
    }
    if(lastRequest) destroy();
}

void ChannelRPCImpl::lock()
{
    dataMutex.lock();
}

void ChannelRPCImpl::unlock()
{
    dataMutex.unlock();
}
  
ServiceChannelRPC::ServiceChannelRPC(
    String const &channelName,
    ServiceRPC::shared_pointer const & serviceRPC)
: serviceRPC(serviceRPC)
{
    ChannelBaseProvider::shared_pointer serviceProvider
        = PVServiceProvider::getPVServiceProvider();
    PVTop::shared_pointer  pvTop = PVTop::shared_pointer(
        new PVTop(channelName,serviceRPC));
    PVServiceProvider* xxx = static_cast<PVServiceProvider*>(serviceProvider.get());
    xxx->addRecord(pvTop);
}

ServiceChannelRPC::~ServiceChannelRPC()
{
    serviceRPC->destroy();
}

void ServiceChannelRPC::destroy()
{ 
}

}}
