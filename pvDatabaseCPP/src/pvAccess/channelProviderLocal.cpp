/* channelChannelProviderLocal.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author Marty Kraimer
 * @date 2013.04
 */

#include <pv/serverContext.h>
#include <pv/syncChannelFind.h>

#define epicsExportSharedSymbols

#include <pv/channelProviderLocal.h>
#include <pv/traceRecord.h>

using namespace epics::pvData;
using namespace epics::pvAccess;
using std::tr1::static_pointer_cast;
using std::tr1::dynamic_pointer_cast;
using std::cout;
using std::endl;
using std::string;

namespace epics { namespace pvDatabase { 

static string providerName("local");


class LocalChannelProviderFactory;
typedef std::tr1::shared_ptr<LocalChannelProviderFactory> LocalChannelProviderFactoryPtr;

class LocalChannelProviderFactory : public ChannelProviderFactory
{
    
public:
    POINTER_DEFINITIONS(LocalChannelProviderFactory);
    virtual string getFactoryName() { return providerName;}
    static LocalChannelProviderFactoryPtr create(
        ChannelProviderLocalPtr const &channelProvider)
    {
        LocalChannelProviderFactoryPtr xxx(
            new LocalChannelProviderFactory(channelProvider));
        registerChannelProviderFactory(xxx);
        return xxx;
    }
    virtual  ChannelProvider::shared_pointer sharedInstance()
    {
        return channelProvider;
    }
    virtual  ChannelProvider::shared_pointer newInstance()
    {
        return channelProvider;
    }
private:
    LocalChannelProviderFactory(
        ChannelProviderLocalPtr const &channelProvider)
    : channelProvider(channelProvider)
    {}
    ChannelProviderLocalPtr channelProvider;
};

ChannelProviderLocalPtr getChannelProviderLocal()
{
    static ChannelProviderLocalPtr channelProviderLocal;
    static Mutex mutex;
    Lock xx(mutex);
    if(channelProviderLocal.get()==NULL) {
        channelProviderLocal = ChannelProviderLocalPtr(
            new ChannelProviderLocal());
        ChannelProvider::shared_pointer xxx =
            dynamic_pointer_cast<ChannelProvider>(channelProviderLocal);
        channelProviderLocal->channelFinder =
            SyncChannelFind::shared_pointer(new SyncChannelFind(xxx));
        LocalChannelProviderFactory::create(channelProviderLocal);
    }
    return channelProviderLocal;
}

ChannelProviderLocal::ChannelProviderLocal()
: pvDatabase(PVDatabase::getMaster()),
  beingDestroyed(false)
{
}

ChannelProviderLocal::~ChannelProviderLocal()
{
    cout << "~ChannelProviderLocal()" << endl;
}

void ChannelProviderLocal::destroy()
{
    cout << "ChannelProviderLocal::destroy()" << endl;
    Lock xx(mutex);
    if(beingDestroyed) return;
    beingDestroyed = true;
    pvDatabase->destroy();
}

string ChannelProviderLocal::getProviderName()
{
    return providerName;
}

ChannelFind::shared_pointer ChannelProviderLocal::channelFind(
    string const & channelName,
    ChannelFindRequester::shared_pointer  const &channelFindRequester)
{
    Lock xx(mutex);
    PVRecordPtr pvRecord = pvDatabase->findRecord(channelName);
    if(pvRecord.get()!=NULL) {
        channelFindRequester->channelFindResult(
            Status::Ok,
            channelFinder,
            true);
        
    } else {
        Status notFoundStatus(Status::STATUSTYPE_ERROR,"pv not found");
        channelFindRequester->channelFindResult(
            notFoundStatus,
            channelFinder,
            false);
    }
    return channelFinder;
}

ChannelFind::shared_pointer ChannelProviderLocal::channelList(
    ChannelListRequester::shared_pointer const & channelListRequester)
{
    PVStringArrayPtr records;
    {
        Lock guard(mutex);
        records = pvDatabase->getRecordNames();
    }

    channelListRequester->channelListResult(Status::Ok, channelFinder, records->view(), false);
    return channelFinder;
}

Channel::shared_pointer ChannelProviderLocal::createChannel(
    string const & channelName,
    ChannelRequester::shared_pointer  const &channelRequester,
    short priority)
{
    return createChannel(channelName,channelRequester,priority,"");
}

Channel::shared_pointer ChannelProviderLocal::createChannel(
    string const & channelName,
    ChannelRequester::shared_pointer  const &channelRequester,
    short priority,
    string const &address)
{
    Lock xx(mutex);
    PVRecordPtr pvRecord = pvDatabase->findRecord(channelName);
    if(pvRecord.get()!=NULL) {
        ChannelLocalPtr channel(new ChannelLocal(
            getPtrSelf(),channelRequester,pvRecord));
        channelRequester->channelCreated(
            Status::Ok,
            channel);
        pvRecord->addPVRecordClient(channel);
        return channel;
    }   
    Status notFoundStatus(Status::STATUSTYPE_ERROR,"pv not found");
    channelRequester->channelCreated(
        notFoundStatus,
        Channel::shared_pointer());
    return Channel::shared_pointer();
}

}}
