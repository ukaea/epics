/*exampleServiceRegister.cpp */
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

#include <cantProceed.h>
#include <epicsStdio.h>
#include <epicsMutex.h>
#include <epicsThread.h>
#include <iocsh.h>

#include <epicsExport.h>

#include <pv/pvIntrospect.h>
#include <pv/pvData.h>
#include <pv/pvAccess.h>
#include <pv/serverContext.h>

#include "exampleServiceRPC.h"

using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvIOC;

static ServiceChannelRPC::shared_pointer serviceChannelRPC;

/* define arguments for the following command:
 * startExampleService channelName
 */
static const iocshArg startExampleServiceArg0 = {"channelName", iocshArgString};
static const iocshArg *const startExampleServiceArgs[] = {
    &startExampleServiceArg0};
static const iocshFuncDef startExampleServiceFuncDef = {
    "startExampleService", 1, startExampleServiceArgs};
static void startExampleService(const iocshArgBuf *args)
{
    // the channel name is the single argument which is a string
    const char * channelName = args[0].sval;
    if(channelName==0 || channelName[0]==0) {
        printf("illegal channelName\n");
        return;
    }
    ExampleServiceRPC::shared_pointer example
        = ExampleServiceRPC::shared_pointer(new ExampleServiceRPC());
    serviceChannelRPC = ServiceChannelRPC::shared_pointer(
        new ServiceChannelRPC("serviceRPC",example));
}

static void startExampleServiceRegister(void)
{
    static int firstTime = 1;
    if (firstTime) {
        firstTime = 0;
        iocshRegister(&startExampleServiceFuncDef, startExampleService);
    }
}

epicsExportRegistrar(startExampleServiceRegister);
