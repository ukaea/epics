/*exampleDatabase.cpp */
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
#include <pv/exampleDatabase.h>

using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvDatabase;

static const iocshArg testArg0 = { "prefix", iocshArgString };
static const iocshArg *testArgs[] = {
    &testArg0};

static const iocshFuncDef exampleDatabaseFuncDef = {
    "exampleDatabase", 1, testArgs};
static void exampleDatabaseCallFunc(const iocshArgBuf *args)
{
    ExampleDatabase::create();
}

static void exampleDatabaseRegister(void)
{
    static int firstTime = 1;
    if (firstTime) {
        firstTime = 0;
        iocshRegister(&exampleDatabaseFuncDef, exampleDatabaseCallFunc);
    }
}

extern "C" {
    epicsExportRegistrar(exampleDatabaseRegister);
}
