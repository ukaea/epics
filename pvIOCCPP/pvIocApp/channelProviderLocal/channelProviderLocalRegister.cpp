/*channelProviderLocalRegister.cpp */
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
#include <epicsEvent.h>
#include <epicsThread.h>
#include <iocsh.h>

#include <epicsExport.h>

#include <pv/pvIntrospect.h>
#include <pv/pvData.h>
#include <pv/pvAccess.h>
#include <pv/serverContext.h>

#include <pv/channelProviderLocal.h>

namespace epics { namespace pvIOC {

using namespace epics::pvData;
using namespace epics::pvAccess;

class ChannelProviderLocalRun : public Runnable {
public:
    ChannelProviderLocalRun();
    ~ChannelProviderLocalRun();
    virtual void run();
private:
    Event event;
    ServerContextImpl::shared_pointer ctx;
    Thread *thread;
};

ChannelProviderLocalRun::ChannelProviderLocalRun()
: event(),
  ctx(ServerContextImpl::create()),
  thread(new Thread(String("channelProviderLocalServer"),lowerPriority,this,epicsThreadStackBig))
{}

ChannelProviderLocalRun::~ChannelProviderLocalRun()
{
    ctx->shutdown();
    // we need thead.waitForCompletion()
    event.wait();
    epicsThreadSleep(1.0);
    delete thread;
}

void ChannelProviderLocalRun::run()
{
    ChannelProviderPtr channelProvider(new ChannelProviderLocal());
    ChannelBaseProviderFactory::shared_pointer channelProviderFactory(new ChannelBaseProviderFactory(channelProvider));
    channelProviderFactory->registerSelf();
    ctx->setChannelProviderName(channelProvider->getProviderName());
    ctx->initialize(getChannelAccess());
    ctx->printInfo();
    ctx->run(0);
    ctx->destroy();
    event.signal();
}

}}

using namespace epics::pvIOC;

extern "C" {
static ChannelProviderLocalRun *myRun = 0;

static const iocshFuncDef startChannelProviderLocalFuncDef = {
    "startChannelProviderLocal", 0, 0
};

void startChannelProviderLocal(const iocshArgBuf *args)
{
    if(myRun!=0) {
        printf("server already started\n");
        return;
    }
    myRun = new ChannelProviderLocalRun();
}

static const iocshFuncDef stopChannelProviderLocalFuncDef = {
    "stopChannelProviderLocal", 0, 0
};

void stopChannelProviderLocal(const iocshArgBuf *args)
{
   printf("stopPVAccessServer\n");
   if(myRun!=0) delete myRun;
   myRun = 0;
}

static void startChannelProviderLocalRegister(void)
{
    static int firstTime = 1;
    if (firstTime) {
        firstTime = 0;
        iocshRegister(&startChannelProviderLocalFuncDef, startChannelProviderLocal);
    }
}

static void stopChannelProviderLocalRegister(void)
{
    static int firstTime = 1;
    if (firstTime) {
        firstTime = 0;
        iocshRegister(&stopChannelProviderLocalFuncDef, stopChannelProviderLocal);
    }
}

epicsExportRegistrar(startChannelProviderLocalRegister);
epicsExportRegistrar(stopChannelProviderLocalRegister);

} // extern "C"
