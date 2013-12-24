/* pvServiceChannelCTX.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 */
#include <string>
#include <stdexcept>
#include <memory>

#include <pv/pvIntrospect.h>
#include <pv/pvData.h>
#include <pv/noDefaultMethods.h>
#include <pv/lock.h>
#include <epicsExit.h>
#include <dbAccess.h>

#include <pv/pvServiceProvider.h>

namespace epics { namespace pvIOC { 

using namespace epics::pvData;
using namespace epics::pvAccess;

PVServiceChannelCTXPtr PVServiceChannelCTX::getPVServiceChannelCTX()
{
    static PVServiceChannelCTXPtr pvServiceChannelCTX;
    static Mutex mutex;
    Lock xx(mutex);

   if(pvServiceChannelCTX.get()==0) {
      pvServiceChannelCTX = PVServiceChannelCTXPtr(new PVServiceChannelCTX());
   }
   return pvServiceChannelCTX;
}

PVServiceChannelCTX::PVServiceChannelCTX()
:
  pvServiceProvider(PVServiceProvider::getPVServiceProvider()),
  ctx(ServerContextImpl::create()),
  thread(new Thread(String("pvServiceChannel"),lowerPriority,this,epicsThreadStackBig))
{}

PVServiceChannelCTX::~PVServiceChannelCTX()
{
    ctx->shutdown();
    // we need thead.waitForCompletion()
    event.wait();
    epicsThreadSleep(1.0);
    delete thread;
}
void PVServiceChannelCTX::run()
{
    pvServiceProvider->registerSelf();
    String providerName = pvServiceProvider->getProviderName();
    ctx->setChannelProviderName(providerName);
    ctx->initialize(getChannelAccess());
    ctx->printInfo();
    ctx->run(0);
    ctx->destroy();
    event.signal();
}

}}
