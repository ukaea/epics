/* channelProviderLocal.cpp */
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
#include <stdexcept>
#include <memory>

#include <pv/channelProviderLocal.h>


namespace epics { namespace pvIOC { 

using namespace epics::pvData;
using namespace epics::pvAccess;
using std::tr1::static_pointer_cast;

static String providerName("local");

ChannelProviderLocal::ChannelProviderLocal()
: beingDestroyed(false)
{
}

ChannelProviderLocal::~ChannelProviderLocal()
{
}

static ChannelFind::shared_pointer nullChannelFind;
static Channel::shared_pointer nullChannel;

String ChannelProviderLocal::getProviderName()
{
    return providerName;
}

void ChannelProviderLocal::destroy()
{
    Lock xx(mutex);
    beingDestroyed = true;
    unregisterChannelProvider(getPtrSelf());
    for(size_t i=0; i<channelNames.size(); i++)
    {
         channelProviders[i]->destroy();
    }
    channelNames = StringArray();
    channelProviders = ChannelProviderPtrArray();
}

void ChannelProviderLocal::registerProvider(
    String const &channelName, ChannelProviderPtr const & provider)
{
    Lock lock(mutex);
    channelNames.push_back(channelName);
    channelProviders.push_back(provider);
}

ChannelFind::shared_pointer ChannelProviderLocal::channelFind(
    String const &channelName,
    ChannelFindRequester::shared_pointer const & channelFindRequester)
{
    Lock lock(mutex);
    for(size_t i=0; i<channelNames.size(); i++)
    {
         if((channelName.compare(channelNames[i])==0)) {
             return channelProviders[i]->channelFind(
                 channelName,channelFindRequester);
         }
    }
    return nullChannelFind;
}
Channel::shared_pointer ChannelProviderLocal::createChannel(
    String const &channelName,
    ChannelRequester::shared_pointer  const &channelRequester,
    short priority,
    String const &address)
{
    return createChannel(channelName,channelRequester,priority);
}

Channel::shared_pointer ChannelProviderLocal::createChannel(
    String const &channelName,
    ChannelRequester::shared_pointer  const &channelRequester,
    short priority)
{
    Lock lock(mutex);
    for(size_t i=0; i<channelNames.size(); i++)
    {
         if(channelNames[i]==channelName) {
             return channelProviders[i]->createChannel(
                  channelName,channelRequester);
         }
    }
    return nullChannel;
}


}}

