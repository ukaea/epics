/* channelBase.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 */
#ifndef CHANNELBASE_H
#define CHANNELBASE_H
#include <string>
#include <cstring>
#include <stdexcept>
#include <memory>

#include <pv/status.h>
#include <pv/monitor.h>
#include <pv/pvIntrospect.h>
#include <pv/pvData.h>
#include <pv/lock.h>
#include <pv/pvAccess.h>

#include <set>

namespace epics { namespace pvAccess { 

class ChannelBaseDebug {
public:
    static void setLevel(int level);
    static int getLevel();
};

class ChannelBaseProvider;
typedef std::tr1::shared_ptr<ChannelBaseProvider> ChannelBaseProviderPtr;

class ChannelBase;
class ChannelBaseProcess;
class ChannelBaseGet;
class ChannelBasePut;
class ChannelBasePutGet;
class ChannelBaseMonitor;
class ChannelBaseRPC;
class ChannelBaseArray;
typedef std::tr1::shared_ptr<ChannelBase> ChannelBasePtr;
typedef std::set<ChannelBasePtr> ChannelBaseList;

/* ChannelBase is a base class for implementing Channel
 * ChannelBase implements all Channel methods.
 * A derived class  need only implement the methods it supports and lets
 * the base class handle the others.
 */

class ChannelBase :
  public Channel,
  public std::tr1::enable_shared_from_this<ChannelBase>
{
public:
    POINTER_DEFINITIONS(ChannelBase);
    ChannelBase(
        ChannelProvider::shared_pointer const &channelProvider,
        ChannelRequester::shared_pointer const & requester,
        epics::pvData::String const & channelName
    );
    virtual ~ChannelBase();
    virtual void destroy();
    virtual epics::pvData::String getRequesterName();
    virtual void message(
        epics::pvData::String const & message,
        epics::pvData::MessageType messageType);
    virtual ChannelProvider::shared_pointer getProvider();
    virtual epics::pvData::String getRemoteAddress();
    virtual Channel::ConnectionState getConnectionState();
    virtual epics::pvData::String getChannelName();
    virtual ChannelRequester::shared_pointer getChannelRequester();
    virtual bool isConnected();
    virtual void getField(
        GetFieldRequester::shared_pointer const &requester,
        epics::pvData::String const & subField);
    virtual AccessRights getAccessRights(
        epics::pvData::PVField::shared_pointer const &pvField);
    virtual ChannelProcess::shared_pointer createChannelProcess(
        ChannelProcessRequester::shared_pointer const &requester,
        epics::pvData::PVStructure::shared_pointer const &pvRequest);
    virtual ChannelGet::shared_pointer createChannelGet(
        ChannelGetRequester::shared_pointer const &requester,
        epics::pvData::PVStructure::shared_pointer const &pvRequest);
    virtual ChannelPut::shared_pointer createChannelPut(
        ChannelPutRequester::shared_pointer const &requester,
        epics::pvData::PVStructure::shared_pointer const &pvRequest);
    virtual ChannelPutGet::shared_pointer createChannelPutGet(
        ChannelPutGetRequester::shared_pointer const &requester,
        epics::pvData::PVStructure::shared_pointer const &pvRequest);
    virtual ChannelRPC::shared_pointer createChannelRPC(
        ChannelRPCRequester::shared_pointer const &requester,
        epics::pvData::PVStructure::shared_pointer const &pvRequest);
    virtual epics::pvData::Monitor::shared_pointer createMonitor(
        epics::pvData::MonitorRequester::shared_pointer const &requester,
        epics::pvData::PVStructure::shared_pointer const &pvRequest);
    virtual ChannelArray::shared_pointer createChannelArray(
        ChannelArrayRequester::shared_pointer const &requester,
        epics::pvData::PVStructure::shared_pointer const &pvRequest);
    virtual void printInfo();
    virtual void printInfo(epics::pvData::StringBuilder out);
    // following called by derived classes
    void addChannelProcess(ChannelProcess::shared_pointer const &);
    void addChannelGet(ChannelGet::shared_pointer const &);
    void addChannelPut(ChannelPut::shared_pointer const &);
    void addChannelPutGet(ChannelPutGet::shared_pointer const &);
    void addChannelMonitor(epics::pvData::Monitor::shared_pointer const &);
    void addChannelRPC(ChannelRPC::shared_pointer const &);
    void addChannelArray(ChannelArray::shared_pointer const &);
    void removeChannelProcess(ChannelProcess::shared_pointer const &);
    void removeChannelGet(ChannelGet::shared_pointer const &);
    void removeChannelPut(ChannelPut::shared_pointer const &);
    void removeChannelPutGet(ChannelPutGet::shared_pointer const &);
    void removeChannelMonitor(epics::pvData::Monitor::shared_pointer const &);
    void removeChannelRPC(ChannelRPC::shared_pointer const &);
    void removeChannelArray(ChannelArray::shared_pointer const &);
protected:
    shared_pointer getPtrSelf()
    {
        return shared_from_this();
    }
private:
    ChannelProvider::shared_pointer  provider;
    ChannelRequester::shared_pointer requester;
    epics::pvData::String channelName;
    std::set<ChannelProcess::shared_pointer> channelProcessList;
    std::set<ChannelGet::shared_pointer> channelGetList;
    std::set<ChannelPut::shared_pointer> channelPutList;
    std::set<ChannelPutGet::shared_pointer> channelPutGetList;
    std::set<epics::pvData::Monitor::shared_pointer> channelMonitorList;
    std::set<ChannelRPC::shared_pointer> channelRPCList;
    std::set<ChannelArray::shared_pointer> channelArrayList;
    bool beingDestroyed;
    epics::pvData::Mutex mutex;
};

class ChannelBaseProvider :
    public ChannelProvider,
    public std::tr1::enable_shared_from_this<ChannelBaseProvider>
{
public:
    POINTER_DEFINITIONS(ChannelBaseProvider);
    ChannelBaseProvider(
        epics::pvData::String const &providerName
    );
    virtual ~ChannelBaseProvider();
    virtual epics::pvData::String getProviderName();
    virtual void destroy();
    virtual ChannelFind::shared_pointer channelFind(
        epics::pvData::String const & channelName,
        ChannelFindRequester::shared_pointer const & channelFindRequester) = 0;
    virtual Channel::shared_pointer createChannel(
        epics::pvData::String const & channelName,
        ChannelRequester::shared_pointer const &requester, short priority);
    virtual Channel::shared_pointer createChannel(
        epics::pvData::String const & channelName,
        ChannelRequester::shared_pointer  const & channelRequester,
        short priority,
        epics::pvData::String const & address) = 0;
    // following called by derived class
    void channelFound(
        bool found,
        ChannelFindRequester::shared_pointer const & requester);
    void channelNotCreated(
        ChannelRequester::shared_pointer const & requester);
    void channelCreated(ChannelBase::shared_pointer const &channel);
    void removeChannel(ChannelBase::shared_pointer const &channel);
    void unregisterSelf();
    void registerSelf();
protected:
    shared_pointer getPtrSelf()
    {
        return shared_from_this();
    }
private:
    epics::pvData::String providerName;
    ChannelBaseList channelList;
    bool isRegistered;
    bool beingDestroyed;
    epics::pvData::Mutex mutex;
};

}}

#endif  /* CHANNELBASE_H */
