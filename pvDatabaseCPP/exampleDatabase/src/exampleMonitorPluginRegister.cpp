/*exampleMonitorPlugin.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 * @date 2013.07.24
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
#include <epicsEvent.h>
#include <epicsThread.h>
#include <iocsh.h>

#include <pv/pvIntrospect.h>
#include <pv/pvData.h>
#include <pv/pvAccess.h>
#include <pv/pvDatabase.h>

#include <epicsExport.h>
#include <pv/exampleMonitorPlugin.h>


using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvDatabase;


static const iocshFuncDef exampleMonitorPluginFuncDef = {
    "exampleMonitorPlugin", 0,0 };


static void exampleMonitorPluginCallFunc(const iocshArgBuf *args)
{
    ExampleMonitorPlugin::create();
}

static void exampleMonitorPluginRegister(void)
{
std::cout << "exampleMonitorPluginRegister\n";
    static int firstTime = 1;
    if (firstTime) {
        firstTime = 0;
        iocshRegister(&exampleMonitorPluginFuncDef, exampleMonitorPluginCallFunc);
    }
std::cout << "exampleMonitorPluginRegister returning\n";
}

extern "C" {
    epicsExportRegistrar(exampleMonitorPluginRegister);
}
