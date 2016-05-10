// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution

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

