// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution


/*ArchiverServiceRPCMain.cpp */


#include <cstddef>
#include <cstdlib>
#include <cstddef>
#include <string>
#include <cstdio>
#include <memory>
#include <iostream>

#include <cantProceed.h>
#include <epicsStdio.h>
#include <epicsMutex.h>
#include <epicsEvent.h>
#include <epicsThread.h>

#include <epicsExport.h>

#include <pv/pvIntrospect.h>
#include <pv/pvData.h>
#include <pv/pvAccess.h>
#include <pv/rpcServer.h>

#include "ArchiverServiceRPC.h"


int main(int argc,char *argv[])
{
    using namespace epics::channelArchiverService;

    if(argc < 3)
    {
        std::cerr << "usage: ArchiverServiceRPC channel index1 [index_2 ... index_n]" << std::endl;
        exit(1);
    }

    char * channel = argv[1];

    std::vector<std::string> indexFilenames;
    for (int i = 2; i < argc; ++i)
    {
        char * index = argv[i];
        indexFilenames.push_back(index);
    }

    epics::pvAccess::RPCServer server;
    server.registerService(channel, epics::pvAccess::RPCService::shared_pointer(
        new ArchiverServiceRPC(indexFilenames)));
    server.printInfo();
    server.run();

    return 0;
}

