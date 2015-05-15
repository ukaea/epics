/*registerChannelProviderLocal.cpp*/
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 * @date 2014.07.03
 */

/* Author: Marty Kraimer */

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
#include <iocsh.h>
#include <shareLib.h>

#include <pv/pvAccess.h>
#include <pv/serverContext.h>

// this declared epicsExportSharedSymbols
#include <epicsExport.h>

#include <pv/channelProviderLocal.h>

using std::cout;
using std::endl;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvDatabase;

static const iocshFuncDef pvdblFuncDef = {
    "pvdbl", 0, 0
};
extern "C" void pvdbl(const iocshArgBuf *args)
{
    PVDatabasePtr master = PVDatabase::getMaster();
    PVStringArrayPtr pvNames = master->getRecordNames();
    PVStringArray::const_svector xxx = pvNames->view();
    for(size_t i=0; i<xxx.size(); ++i) cout<< xxx[i] << endl;
}


static void registerChannelProviderLocal(void)
{
    static int firstTime = 1;
    if (firstTime) {
        firstTime = 0;
        iocshRegister(&pvdblFuncDef, pvdbl);
        getChannelProviderLocal();
    }
}

extern "C" {
    epicsExportRegistrar(registerChannelProviderLocal);
}
