// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution


#ifndef HELLOSERVICERPC_H
#define HELLOSERVICERPC_H

#include <pv/rpcService.h>

namespace epics
{

namespace helloService
{

/**
 * Declaration of the Hello World RPC service.
 */
class HelloService : public epics::pvAccess::RPCService
{
public:
    POINTER_DEFINITIONS(HelloService);

    epics::pvData::PVStructurePtr request(
        epics::pvData::PVStructurePtr const & args
            ) throw (epics::pvAccess::RPCRequestException);
};

}

}

#endif
