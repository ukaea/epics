/* service.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/** 
 * @author mrk
 */
#ifndef SERVICE_H
#define SERVICE_H
#include <string>
#include <cstring>
#include <stdexcept>
#include <memory>

#include <pv/status.h>
#include <pv/monitor.h>
#include <pv/pvIntrospect.h>
#include <pv/pvData.h>
#include <pv/lock.h>
#include <pv/pvIntrospect.h>
#include <pv/pvData.h>
#include <pv/standardField.h>
#include <pv/standardPVField.h>
#include <pv/alarm.h>
#include <pv/pvAlarm.h>
#include <pv/timeStamp.h>
#include <pv/pvTimeStamp.h>
#include <pv/pvAccess.h>

namespace epics { namespace pvIOC { 

class ServiceRPC;
class ServiceChannel;

/**
 * ServiceRPC is the interface implemented by a service that
 * uses channelRPC for communication between the client and the service.
 */
class ServiceRPC
{
public:
    POINTER_DEFINITIONS(ServiceRPC);
    virtual ~ServiceRPC(){}
    virtual void destroy() = 0;
    virtual void request(
        epics::pvAccess::ChannelRPCRequester::shared_pointer const & channelRPCRequester,
        epics::pvData::PVStructurePtr const & pvArgument) = 0;
};

class ServiceChannelRPC :
    public std::tr1::enable_shared_from_this<ServiceChannelRPC>
{
public:
    POINTER_DEFINITIONS(ServiceChannelRPC);
    /**
     * Create a pvAccess Channel that implements an RPC service.
     */
    ServiceChannelRPC(
        epics::pvData::String const &channelName,
        ServiceRPC::shared_pointer const & serviceRPC);
    virtual ~ServiceChannelRPC();
    /**
     * Destroy the serviceChannel
     */
    virtual void destroy();
private:
    shared_pointer getPtrSelf()
    {
        return shared_from_this();
    }
    ServiceRPC::shared_pointer serviceRPC;
};

}}

#endif  /* SERVICE_H */
