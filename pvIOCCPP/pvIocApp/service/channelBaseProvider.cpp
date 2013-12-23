/* channelBaseProvider.cpp */
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

#include <pv/channelBase.h>

namespace epics { namespace pvAccess { 

using namespace epics::pvData;
using namespace epics::pvAccess;

ChannelBaseProvider::ChannelBaseProvider(
    String const &providerName
)
: providerName(providerName),
  isRegistered(false),
  beingDestroyed(false)
{
}

void ChannelBaseProvider::registerSelf()
{
    if(!isRegistered) {
        isRegistered = true;
        registerChannelProvider(getPtrSelf());
    }
}

void ChannelBaseProvider::unregisterSelf()
{
    if(isRegistered) {
       isRegistered = false;
        unregisterChannelProvider(getPtrSelf());
    }
}

ChannelBaseProvider::~ChannelBaseProvider()
{
}

void ChannelBaseProvider::destroy()
{
    Lock xx(mutex);
    beingDestroyed = true;
    unregisterSelf();
    ChannelBaseList::iterator iter;
    for(iter = channelList.begin(); iter!=channelList.end(); ++iter) {
        ChannelBasePtr channel = *iter;
        channel->destroy();
    }
    channelList.clear();
}

String ChannelBaseProvider::getProviderName()
{
    return providerName;
}

void ChannelBaseProvider::channelFound(
    bool found,
    ChannelFindRequester::shared_pointer const & channelFindRequester)
{
    if(found) {
        channelFindRequester->channelFindResult(
            Status::Ok,
            ChannelFind::shared_pointer(),
            true);
    } else {
        Status notFoundStatus(Status::STATUSTYPE_ERROR,String("pv not found"));
        channelFindRequester.get()->channelFindResult(
            notFoundStatus,
            ChannelFind::shared_pointer(),
            false);
    }
}

Channel::shared_pointer ChannelBaseProvider::createChannel(
    String const & channelName,
    ChannelRequester::shared_pointer  const &channelRequester,
    short priority)
{
    return createChannel(channelName,channelRequester,priority,"");
}

void ChannelBaseProvider::channelNotCreated(
    ChannelRequester::shared_pointer const &channelRequester)
{
    Status notFoundStatus(Status::STATUSTYPE_ERROR,String("pv not found"));
    channelRequester->channelCreated(
        notFoundStatus,
        Channel::shared_pointer());
}

void ChannelBaseProvider::channelCreated(ChannelBasePtr const &channel)
{
    Lock xx(mutex);
    channelList.insert(channel);
    channel->getChannelRequester()->channelCreated(Status::Ok,channel);
}

void ChannelBaseProvider::removeChannel(ChannelBase::shared_pointer  const &channel)
{
    Lock xx(mutex);
    if(beingDestroyed) return;
    channelList.erase(channel);
}

}}
