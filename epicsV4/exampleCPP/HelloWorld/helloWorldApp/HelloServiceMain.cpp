/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS exampleCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */


#include <pv/rpcServer.h>
#include "HelloService.h"


using namespace epics::pvAccess;

// Main is the entry point of the HelloService server side executable.

int main(int argc,char *argv[])
{
    RPCServer server;

    // register our service as "helloService"
    server.registerService("helloService",
        RPCService::shared_pointer(new epics::helloService::HelloService()));

    server.printInfo();
    server.run();

    return 0;
}

