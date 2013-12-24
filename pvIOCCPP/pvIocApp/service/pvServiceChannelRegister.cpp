/*pvServiceChannelRegister.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 */

#include <cstddef>
#include <cstdlib>
#include <cstddef>
#include <string>
#include <cstdio>
#include <memory>

#include <cantProceed.h>
#include <epicsStdio.h>
#include <epicsMutex.h>
#include <epicsEvent.h>
#include <epicsThread.h>
#include <iocsh.h>

#include <pv/pvIntrospect.h>
#include <pv/pvData.h>
#include <pv/pvAccess.h>
#include <pv/serverContext.h>

#define epicsExportSharedSymbols
#include <epicsExport.h>
#include <pv/pvServiceProvider.h>

using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvIOC;

static const iocshArg setChannelBaseDebugLevelArg0 = { "level",iocshArgInt};
static const iocshArg *setChannelBaseDebugLevelArgs[] =
     {&setChannelBaseDebugLevelArg0};
static const iocshFuncDef setChannelBaseDebugLevelFuncDef = {
    "setChannelBaseDebugLevel", 1, setChannelBaseDebugLevelArgs };
extern "C" void setChannelBaseDebugLevel(const iocshArgBuf *args)
{
    int level = args[0].ival;
    ChannelBaseDebug::setLevel(level);
    printf("new level %d\n",level);
}

extern "C" void startPVServiceChannel(const iocshArgBuf *args)
{
    PVServiceChannelCTXPtr pvServiceChannelCTX = PVServiceChannelCTX::getPVServiceChannelCTX();
    epicsThreadSleep(.1);
    pvServiceChannelCTX->getPVServiceProvider()->registerSelf();
}
extern "C" void stopPVServiceChannel(const iocshArgBuf *args)
{
    PVServiceChannelCTXPtr pvServiceChannelCTX = PVServiceChannelCTX::getPVServiceChannelCTX();
    pvServiceChannelCTX->getPVServiceProvider()->unregisterSelf();
}

static void setChannelBaseDebugLevelRegister(void)
{
    static int firstTime = 1;
    if (firstTime) {
        firstTime = 0;
        iocshRegister(&setChannelBaseDebugLevelFuncDef, setChannelBaseDebugLevel);
    }
}

static void startPVServiceChannelRegister(void)
{
    static const iocshFuncDef startPVServiceChannelFuncDef = {"startPVServiceChannel", 0, 0};
    static int firstTime = 1;
    if (firstTime) {
        firstTime = 0;
        iocshRegister(&startPVServiceChannelFuncDef, startPVServiceChannel);
    }
}

static void stopPVServiceChannelRegister(void)
{

    static const iocshFuncDef stopPVServiceChannelFuncDef = { "stopPVServiceChannel", 0, 0 };
    static int firstTime = 1;
    if (firstTime) {
        firstTime = 0;
        iocshRegister(&stopPVServiceChannelFuncDef, stopPVServiceChannel);
    }
}

epicsExportRegistrar(setChannelBaseDebugLevelRegister);
epicsExportRegistrar(startPVServiceChannelRegister);
epicsExportRegistrar(stopPVServiceChannelRegister);
