/* channelProviderLocal.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 */
#ifndef CHANNELPROVIDERLOCAL_H
#define CHANNELPROVIDERLOCAL_H
#include <string>
#include <cstring>
#include <stdexcept>
#include <memory>

#include <pv/lock.h>
#include <pv/pvType.h>
#include <pv/pvData.h>
#include <pv/pvAccess.h>

namespace epics { namespace pvIOC { 


class ChannelProviderLocal;
typedef std::tr1::shared_ptr<ChannelProviderLocal> ChannelProviderLocalPtr;
typedef epics::pvAccess::ChannelProvider::shared_pointer ChannelProviderPtr;
typedef std::vector<ChannelProviderPtr> ChannelProviderPtrArray;


class ChannelProviderLocal :
    public epics::pvAccess::ChannelProvider,
    public std::tr1::enable_shared_from_this<ChannelProviderLocal>
{
public:
    POINTER_DEFINITIONS(ChannelProviderLocal);

    virtual ~ChannelProviderLocal();
    void registerProvider(
        epics::pvData::String const &channelName,
        ChannelProviderPtr const & provider);
    virtual void destroy();
    virtual  epics::pvData::String getProviderName();
    virtual epics::pvAccess::ChannelFind::shared_pointer channelFind(
        epics::pvData::String const &channelName,
        epics::pvAccess::ChannelFindRequester::shared_pointer const & channelFindRequester);
    virtual epics::pvAccess::Channel::shared_pointer createChannel(
        epics::pvData::String const &channelName,
        epics::pvAccess::ChannelRequester::shared_pointer const &channelRequester,
        short priority);
    virtual epics::pvAccess::Channel::shared_pointer createChannel(
        epics::pvData::String const &channelName,
        epics::pvAccess::ChannelRequester::shared_pointer const &channelRequester,
        short priority,
        epics::pvData::String const &address);
private:
    shared_pointer getPtrSelf()
    {
        return shared_from_this();
    }
    ChannelProviderLocal();
    ChannelProviderPtrArray channelProviders;
    epics::pvData::StringArray channelNames;
    epics::pvData::Mutex mutex;
    bool beingDestroyed;
    friend class ChannelProviderLocalRun;
};


}}

#endif  /* CHANNELPROVIDERLOCAL_H */
