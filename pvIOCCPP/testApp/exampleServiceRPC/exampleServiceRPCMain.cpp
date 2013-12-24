/*exampleServiceRPCMain.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 */

/* Author: Marty Kraimer */

#include <cstddef>
#include <cstdlib>
#include <cstddef>
#include <string>
#include <cstdio>
#include <memory>
#include <iostream>

#include <epicsStdio.h>
#include <epicsMutex.h>
#include <epicsEvent.h>
#include <epicsThread.h>

#include <epicsExport.h>

#include <pv/pvIntrospect.h>
#include <pv/pvData.h>
#include <pv/pvAccess.h>
#include <pv/serverContext.h>

#include <pv/pvServiceProvider.h>
#include "exampleServiceRPC.h"

using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvIOC;

void example()
{
    PVServiceChannelCTX::shared_pointer myCTX = PVServiceChannelCTX::getPVServiceChannelCTX();
    ExampleServiceRPC::shared_pointer example
        = ExampleServiceRPC::shared_pointer(new ExampleServiceRPC());
    ServiceChannelRPC::shared_pointer serviceChannelRPC
        = ServiceChannelRPC::shared_pointer(
            new ServiceChannelRPC("serviceRPC",example));
    cout << "serviceRPC\n";
    string str;
    while(true) {
        cout << "Type exit to stop: \n";
        getline(cin,str);
        if(str.compare("exit")==0) break;
        
    }
}

int main(int argc,char *argv[])
{
    example();
    return 0;
}

