/* exampleRPCService.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 */
#ifndef EXAMPLE_RPCSERVICE_H
#define EXAMPLE_RPCSERVICE_H
#include <string>
#include <cstring>
#include <stdexcept>
#include <memory>

#include <pv/service.h>

namespace epics { namespace pvIOC { 

class ExampleServiceRPC;

class ExampleServiceRPC :
  public virtual ServiceRPC,
  public std::tr1::enable_shared_from_this<ExampleServiceRPC>
{
public:
    POINTER_DEFINITIONS(ExampleServiceRPC);
    ExampleServiceRPC();
    virtual ~ExampleServiceRPC();
    virtual void destroy();
    virtual void request(
        epics::pvAccess::ChannelRPCRequester::shared_pointer const & channelRPCRequester,
        epics::pvData::PVStructure::shared_pointer const & pvArgument);
private:
    shared_pointer getPtrSelf()
    {
        return shared_from_this();
    }
};

}}

#endif  /* EXAMPLE_RPCSERVICE_H */
