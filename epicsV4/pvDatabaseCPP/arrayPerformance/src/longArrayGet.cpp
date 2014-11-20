/* longArrayGet.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 * @date 2013.08.09
 */

#include <epicsThread.h>
#include <pv/caProvider.h>

#define epicsExportSharedSymbols
#include <longArrayGet.h>

namespace epics { namespace pvDatabase { 

using namespace epics::pvData;
using namespace epics::pvAccess;
using std::tr1::static_pointer_cast;
using std::tr1::dynamic_pointer_cast;
using std::cout;
using std::endl;
using std::ostringstream;
using std::string;

static string requesterName("longArrayGet");
static string request("value,timeStamp,alarm");
static epics::pvData::Mutex printMutex;

class LongArrayChannelRequester;
typedef std::tr1::shared_ptr<LongArrayChannelRequester> LongArrayChannelRequesterPtr;
class LongArrayChannelGetRequester;
typedef std::tr1::shared_ptr<LongArrayChannelGetRequester> LongArrayChannelGetRequesterPtr;

class LongArrayChannelRequester :
    virtual public ChannelRequester,
    public std::tr1::enable_shared_from_this<LongArrayChannelRequester>
{
public:
    LongArrayChannelRequester(
        LongArrayChannelGetPtr const & longArrayChannelGet)
    : longArrayChannelGet(longArrayChannelGet),
      isDestroyed(false)
    {}
    virtual ~LongArrayChannelRequester(){}
    virtual void destroy()
        {
            Lock guard(mutex);
            if(isDestroyed) return;
            isDestroyed = true;
            longArrayChannelGet.reset();
        }
    virtual string  getRequesterName() { return requesterName;}
    virtual void message(string const & message, MessageType messageType)
       {
            Lock guard(printMutex);
            cout << requesterName << " message " << message << endl;
       }
    virtual void channelCreated(
        const Status& status,
        Channel::shared_pointer const & channel);
    virtual void channelStateChange(
        Channel::shared_pointer const & channel,
        Channel::ConnectionState connectionState);
private:
    LongArrayChannelRequesterPtr getPtrSelf()
    {
        return shared_from_this();
    }
    LongArrayChannelGetPtr longArrayChannelGet;
    bool isDestroyed;
    Mutex mutex;
};


class LongArrayChannelGetRequester :
    virtual public ChannelGetRequester,
    public std::tr1::enable_shared_from_this<LongArrayChannelGetRequester>
{
public:
    LongArrayChannelGetRequester(
        LongArrayChannelGetPtr const & longArrayChannelGet)
    : longArrayChannelGet(longArrayChannelGet),
      isDestroyed(false)
    {}
    virtual ~LongArrayChannelGetRequester(){}
    virtual void destroy()
        {
            Lock guard(mutex);
            if(isDestroyed) return;
            isDestroyed = true;
            longArrayChannelGet.reset();
        }
    virtual string  getRequesterName() { return requesterName;}
    virtual void message(string const & message, MessageType messageType)
       {
            Lock guard(printMutex);
            cout << requesterName << " message " << message << endl;
       }
    virtual void channelGetConnect(
        Status const & status,
        ChannelGet::shared_pointer const & channelGet,
        StructureConstPtr const &structure);
    virtual void getDone(
        Status const & status,
        ChannelGet::shared_pointer const & channelGet,
        PVStructurePtr const &pvStructure,
        BitSetPtr const & bitSet);
private:
    LongArrayChannelGetRequesterPtr getPtrSelf()
    {
        return shared_from_this();
    }
    LongArrayChannelGetPtr longArrayChannelGet;
    bool isDestroyed;
    Mutex mutex;
};



class LongArrayChannelGet :
    public std::tr1::enable_shared_from_this<LongArrayChannelGet>,
    public epicsThreadRunable
{
public:
    LongArrayChannelGet(
        string  providerName,
        string  channelName,
        int iterBetweenCreateChannel,
        int iterBetweenCreateChannelGet,
        double delayTime)
    : providerName(providerName),
      channelName(channelName),
      iterBetweenCreateChannel(iterBetweenCreateChannel),
      iterBetweenCreateChannelGet(iterBetweenCreateChannelGet),
      delayTime(delayTime),
      isDestroyed(false),
      runReturned(false),
      threadName("longArrayGet")
    {}
    virtual ~LongArrayChannelGet(){}
    bool init();
    virtual void destroy();
    virtual void run();
    void message(string const & message, MessageType messageType)
       {
            Lock guard(printMutex);
            cout << requesterName << " message " << message << endl;
       }
    virtual void channelCreated(
        const Status& status,
        Channel::shared_pointer const & channel);
    virtual void channelStateChange(
        Channel::shared_pointer const & channel,
        Channel::ConnectionState connectionState);
    virtual void channelGetConnect(
        Status const & status,
        ChannelGet::shared_pointer const & channelGet,
        StructureConstPtr const &structure);
    virtual void getDone(
        Status const & status,
        ChannelGet::shared_pointer channelGet,
        PVStructurePtr const &pvStructure,
        BitSetPtr const & bitSet);
private:
    LongArrayChannelGetPtr getPtrSelf()
    {
        return shared_from_this();
    }
    size_t checkResult();
    string  providerName;
    string  channelName;
    int iterBetweenCreateChannel;
    int iterBetweenCreateChannelGet;
    double delayTime;
    bool isDestroyed;
    bool runReturned;
    std::string threadName;
    Status status;
    Event event;
    Mutex mutex;
    std::auto_ptr<epicsThread> thread;
    Channel::shared_pointer channel;
    ChannelGet::shared_pointer channelGet;
    PVStructurePtr pvStructure;
    BitSetPtr bitSet;
    LongArrayChannelRequesterPtr longArrayChannelRequester;
    LongArrayChannelGetRequesterPtr longArrayChannelGetRequester;
};

void LongArrayChannelRequester::channelCreated(
    const Status& status,
    Channel::shared_pointer const & channel)
{
     Lock guard(mutex);
     if(isDestroyed) return;
     longArrayChannelGet->channelCreated(status,channel);
}

void LongArrayChannelRequester::channelStateChange(
    Channel::shared_pointer const & channel,
    Channel::ConnectionState connectionState)
{
    string  mess(Channel::ConnectionStateNames[connectionState]);
    message(mess,infoMessage);
    Lock guard(mutex);
    if(isDestroyed) return;
    longArrayChannelGet->channelStateChange(channel,connectionState);
}

void LongArrayChannelGetRequester::channelGetConnect(
    Status const & status,
        ChannelGet::shared_pointer const & channelGet,
        StructureConstPtr const &structure)
{
    Lock guard(mutex);
    if(isDestroyed) return;
    longArrayChannelGet->channelGetConnect(
        status,channelGet,structure);
}

void LongArrayChannelGetRequester::getDone(
        Status const & status,
        ChannelGet::shared_pointer const & channelGet,
        PVStructurePtr const &pvStructure,
        BitSetPtr const & bitSet)
{
    Lock guard(mutex);
    if(isDestroyed) return;
    longArrayChannelGet->getDone(status,channelGet,pvStructure,bitSet);
}

void LongArrayChannelGet::channelCreated(
    const Status& status,
    Channel::shared_pointer const & channel)
{
    if(!status.isOK()) message(status.getMessage(),errorMessage);
    this->status = status;
    this->channel = channel;
    event.signal();
}

void LongArrayChannelGet::channelStateChange(
    Channel::shared_pointer const & channel,
    Channel::ConnectionState connectionState)
{
   MessageType messageType =
       (connectionState==Channel::CONNECTED ? infoMessage : errorMessage);
   message("channelStateChange",messageType);
}
void LongArrayChannelGet::channelGetConnect(
        Status const & status,
        ChannelGet::shared_pointer const & channelGet,
        StructureConstPtr const &structure)
{
    this->status = status;
    if(!status.isOK())  {
        message(status.getMessage(),errorMessage);
        event.signal();
        return;
    }
    this->channelGet = channelGet;
    bool structureOK(true);
    PVStructurePtr pvStructure = getPVDataCreate()->createPVStructure(structure);
    PVFieldPtr pvField = pvStructure->getSubField("timeStamp");
    if(!pvField) structureOK = false;
    pvField = pvStructure->getSubField("value");
    if(!pvField) {
         structureOK = false;
    } else {
        FieldConstPtr field = pvField->getField();
        if(field->getType()!=scalarArray) {
            structureOK = false;
        } else {
            ScalarArrayConstPtr scalarArray = dynamic_pointer_cast<const ScalarArray>(field);
            if(scalarArray->getElementType()!=pvLong) structureOK = false;
        }
    }
    if(!structureOK) {
        string  mess("channelGetConnect: illegal structure");
        message(mess,errorMessage);
        this->status = Status(Status::STATUSTYPE_ERROR,mess);
    }
    event.signal();
}


bool LongArrayChannelGet::init()
{
    ChannelProvider::shared_pointer channelProvider =
        getChannelProviderRegistry()->getProvider(providerName);
    if(!channelProvider) {
        cout << "provider " << providerName << " not found" << endl;
        return false;
    }
    longArrayChannelRequester.reset(new LongArrayChannelRequester(getPtrSelf()));
    channel = channelProvider->createChannel(
          channelName,
          longArrayChannelRequester);
    event.wait();
    channelProvider.reset();
    if(!status.isOK()) return false;
    CreateRequest::shared_pointer createRequest = CreateRequest::create();
    PVStructurePtr pvRequest = createRequest->createRequest(request);
    if(!pvRequest) {
        cout << "request logic error " << createRequest->getMessage() << endl;
        return false;
    }
    longArrayChannelGetRequester.reset(new LongArrayChannelGetRequester(getPtrSelf()));
    channelGet = channel->createChannelGet(
        longArrayChannelGetRequester,
        pvRequest);
    event.wait();
    if(!status.isOK()) return false;
     thread = std::auto_ptr<epicsThread>(new epicsThread(
        *this,
        threadName.c_str(),
        epicsThreadGetStackSize(epicsThreadStackSmall),
        epicsThreadPriorityLow));
     thread->start();
     event.signal();
     return true;
}

void LongArrayChannelGet::destroy()
{
    if(isDestroyed) return;
    isDestroyed = true;
    event.signal();
    while(true) {
        if(runReturned) break;
        epicsThreadSleep(.01);
    }
    if(longArrayChannelRequester) {
        longArrayChannelRequester->destroy();
    }
    if(longArrayChannelGetRequester) {
        longArrayChannelGetRequester->destroy();
    }
    thread->exitWait();
    channel->destroy();
    channelGet.reset();
    channel.reset();
}


void LongArrayChannelGet::run()
{
    while(true) {
        event.wait();
        if(isDestroyed) {
            runReturned = true;
            return;
        }
        TimeStamp timeStamp;
        TimeStamp timeStampLast;
        timeStampLast.getCurrent();
        int numChannelGet = 0;
        int numChannelCreate = 0;
        size_t nElements = 0;
        while(true) {
            channelGet->get();
            event.wait();
            if(isDestroyed) {
                runReturned = true;
                return;
            }
            size_t latestSize = checkResult();
            nElements += latestSize;
            timeStamp.getCurrent();
            double diff = TimeStamp::diff(timeStamp,timeStampLast);
            if(diff>=1.0) {
                ostringstream out;
                out << "get";
                double elementsPerSec = nElements;
                elementsPerSec /= diff;
                if(elementsPerSec>10.0e9) {
                     elementsPerSec /= 1e9;
                     out << " gigaElements/sec " << elementsPerSec;
                } else if(elementsPerSec>10.0e6) {
                     elementsPerSec /= 1e6;
                     out << " megaElements/sec " << elementsPerSec;
                } else if(elementsPerSec>10.0e3) {
                     elementsPerSec /= 1e3;
                     out << " kiloElements/sec " << elementsPerSec;
                } else  {
                     out << " Elements/sec " << elementsPerSec;
                }
                cout << out.str() << endl;
                timeStampLast = timeStamp;
                nElements = 0;
            }
            if(delayTime>0.0) epicsThreadSleep(delayTime);
            if(isDestroyed) {
                runReturned = true;
                return;
            }
            ++numChannelCreate;
            bool createGet = false;
            if(iterBetweenCreateChannel!=0) {
                if(numChannelCreate>=iterBetweenCreateChannel) {
                    longArrayChannelRequester->destroy();
                    channel->destroy();
                    ChannelProvider::shared_pointer channelProvider =
                         getChannelProviderRegistry()->getProvider(providerName);
                    longArrayChannelRequester.reset(new LongArrayChannelRequester(getPtrSelf()));
                    channel = channelProvider->createChannel(
                         channelName,
                         longArrayChannelRequester);
                    event.wait();
                    channelProvider.reset();
                    if(!status.isOK()) {
                         message(status.getMessage(),errorMessage);
                         return;
                    }
                    cout<< "createChannel success" << endl;
                    createGet = true;
                    numChannelCreate = 0;
                }
            }
            ++numChannelGet;
            if(iterBetweenCreateChannelGet!=0) {
                if(numChannelGet>=iterBetweenCreateChannelGet) createGet = true;
            }
            if(createGet) {
                 numChannelGet = 0;
                 longArrayChannelGetRequester->destroy();
                 channelGet->destroy();
                 CreateRequest::shared_pointer createRequest =
                     CreateRequest::create();
                 PVStructurePtr pvRequest =
                     createRequest->createRequest(request);
                 if(!pvRequest) {
                     cout << "request logic error " << createRequest->getMessage() << endl;
                     return ;
                 }
                 longArrayChannelGetRequester.reset(new LongArrayChannelGetRequester(getPtrSelf()));
                 channelGet = channel->createChannelGet(
                     longArrayChannelGetRequester,
                     pvRequest);
                 event.wait();
                 if(!status.isOK()) {
                      message(status.getMessage(),errorMessage);
                      return;
                 }
                 cout<< "createChannelGet success" << endl;
            }
        }
    }
}

void LongArrayChannelGet::getDone(
        Status const & status,
        ChannelGet::shared_pointer channelGet,
        PVStructurePtr const &pvStructure,
        BitSetPtr const & bitSet)
{
    this->pvStructure = pvStructure;
    this->bitSet = bitSet;
    event.signal();
}

size_t LongArrayChannelGet::checkResult()
{
    PVLongArrayPtr pvValue;
    if(!status.isOK()) {
         message(status.getMessage(),errorMessage);
         return 0;
    }
    pvValue = dynamic_pointer_cast<PVLongArray>(pvStructure->getSubField("value"));
    if(!bitSet->get(pvValue->getFieldOffset())) {
        return 0;
    }
    bitSet->clear();
    shared_vector<const int64> data = pvValue->view();
    if(data.size()>0) {
        int64 first = data[0];
        int64 last = data[data.size()-1];
        if(first!=last) {
           cout << "error first=" << first << " last=" << last << endl;
        }
    }
    return data.size();
}


LongArrayGetPtr LongArrayGet::create(
    string  const &providerName,
    string  const & channelName,
    int iterBetweenCreateChannel,
    int iterBetweenCreateChannelGet,
    double delayTime)
{
    LongArrayGetPtr longArrayGet(
        new LongArrayGet(
            providerName,
            channelName,
            iterBetweenCreateChannel,
            iterBetweenCreateChannelGet,
            delayTime));
    if(!longArrayGet->init()) longArrayGet.reset();
    return longArrayGet;
}

LongArrayGet::LongArrayGet(
    string  const &providerName,
    string  const & channelName,
    int iterBetweenCreateChannel,
    int iterBetweenCreateChannelGet,
    double delayTime)
: providerName(providerName),
  channelName(channelName),
  iterBetweenCreateChannel(iterBetweenCreateChannel),
  iterBetweenCreateChannelGet(iterBetweenCreateChannelGet),
  delayTime(delayTime)
{}


LongArrayGet::~LongArrayGet() {}

bool LongArrayGet::init()
{
    longArrayChannelGet = LongArrayChannelGetPtr(new LongArrayChannelGet(
        providerName,
        channelName,
        iterBetweenCreateChannel,
        iterBetweenCreateChannelGet,
        delayTime));
    return  longArrayChannelGet->init();
}

void LongArrayGet::destroy()
{
    longArrayChannelGet->destroy();
    longArrayChannelGet.reset();
}

}}


