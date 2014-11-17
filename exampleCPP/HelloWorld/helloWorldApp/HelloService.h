/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS exampleCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */


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
