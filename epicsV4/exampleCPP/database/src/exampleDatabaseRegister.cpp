/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
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
using namespace epics::exampleCPP::database;

static const iocshFuncDef exampleDatabaseFuncDef = {"exampleDatabase", 0};

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
