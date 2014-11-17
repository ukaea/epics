/* longArrayMonitor.cpp */
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
#include <longArrayMonitor.h>


namespace epics { namespace pvDatabase { 

using namespace epics::pvData;
using namespace epics::pvAccess;
using std::tr1::static_pointer_cast;
using std::tr1::dynamic_pointer_cast;
using std::cout;
using std::endl;
using std::string;
using std::ostringstream;

static string  requesterName("longArrayMonitor");

static void messagePvt(string const & message, MessageType messageType)
{
    cout << requesterName << " message " << message << endl;
}

class LAMChannelRequester :
    public ChannelRequester
{
public:
    LAMChannelRequester(LongArrayMonitorPtr const &longArrayMonitor)
    : longArrayMonitor(longArrayMonitor)
    {}
    virtual ~LAMChannelRequester(){}
    virtual void destroy(){longArrayMonitor.reset();}
    virtual string  getRequesterName() { return requesterName;}
    virtual void message(string const & message, MessageType messageType)
       { messagePvt(message,messageType);}
    virtual void channelCreated(const Status& status, Channel::shared_pointer const & channel);
    virtual void channelStateChange(Channel::shared_pointer const & channel, Channel::ConnectionState connectionState);
private:
    LongArrayMonitorPtr longArrayMonitor;
};

void LAMChannelRequester::channelCreated(const Status& status, Channel::shared_pointer const & channel)
{
    if(!status.isOK()) messagePvt(status.getMessage(),errorMessage);
    longArrayMonitor->status = status;
    longArrayMonitor->channel = channel;
    longArrayMonitor->event.signal();
}

void LAMChannelRequester::channelStateChange(Channel::shared_pointer const & channel, Channel::ConnectionState connectionState)
{
   MessageType messageType = (connectionState==Channel::CONNECTED ? infoMessage : errorMessage);
   messagePvt("channelStateChange",messageType);
}


class LAMMonitorRequester :
    public MonitorRequester,
    public epicsThreadRunable
{
public:
    LAMMonitorRequester(LongArrayMonitorPtr const &longArrayMonitor,double waitTime)
    : longArrayMonitor(longArrayMonitor),
      waitTime(waitTime),
      isDestroyed(false),
      runReturned(false),
      threadName("longArrayMonitor")
    {}
    virtual ~LAMMonitorRequester(){}
    void init();
    virtual void destroy();
    virtual void run();
    virtual string  getRequesterName() { return requesterName;}
    virtual void message(string const & message, MessageType messageType)
       { messagePvt(message,messageType);}
    virtual void monitorConnect(Status const & status,
        MonitorPtr const & monitor, StructureConstPtr const & structure);
    virtual void monitorEvent(MonitorPtr const & monitor);
    virtual void unlisten(MonitorPtr const & monitor);
private:
    LongArrayMonitorPtr longArrayMonitor;
    double waitTime;
    bool isDestroyed;
    bool runReturned;
    std::string threadName;
    Event event;
    Mutex mutex;
    std::auto_ptr<epicsThread> thread;
};

void LAMMonitorRequester::init()
{
     thread = std::auto_ptr<epicsThread>(new epicsThread(
        *this,
        threadName.c_str(),
        epicsThreadGetStackSize(epicsThreadStackSmall),
        epicsThreadPriorityLow));
     thread->start();
}

void LAMMonitorRequester::destroy()
{
    if(isDestroyed) return;
    isDestroyed = true;
    event.signal();
    while(true) {
        if(runReturned) break;
        epicsThreadSleep(.01);
    }
    thread->exitWait();
    longArrayMonitor.reset();
}


void LAMMonitorRequester::monitorConnect(Status const & status,
        MonitorPtr const & monitor, StructureConstPtr const & structure)
{
    longArrayMonitor->status = status;
    longArrayMonitor->monitor = monitor;
    if(!status.isOK())  {
        messagePvt(status.getMessage(),errorMessage);
        longArrayMonitor->event.signal();
        return;
    }
    bool structureOK(true);
    FieldConstPtr field = structure->getField("timeStamp");
    if(!field) structureOK = false;
    field = structure->getField("value");
    if(!field) {
         structureOK = false;
    } else {
        if(field->getType()!=scalarArray) {
            structureOK = false; 
        } else {
            ScalarArrayConstPtr scalarArray = dynamic_pointer_cast<const ScalarArray>(field);
            if(scalarArray->getElementType()!=pvLong) structureOK = false;
        }
    }
    if(!structureOK) {
        string  message("monitorConnect: illegal structure");
        messagePvt(message,errorMessage);
        longArrayMonitor->status = Status(Status::STATUSTYPE_ERROR,message);
    }
    longArrayMonitor->event.signal();
}

void LAMMonitorRequester::run()
{
    PVLongArrayPtr pvValue;
    PVTimeStamp pvTimeStamp;
    TimeStamp timeStamp;
    TimeStamp timeStampLast;
    timeStampLast.getCurrent();
    size_t nElements = 0;
    int nSinceLastReport = 0;
    while(true) {
        event.wait();
        if(isDestroyed) {
            runReturned = true;
            return;
        }
        while(true) {
            MonitorElementPtr monitorElement;
            PVStructurePtr pvStructure;
            {
                 Lock xx(mutex);
                 monitorElement = longArrayMonitor->monitor->poll();
                 if(monitorElement) pvStructure = monitorElement->pvStructurePtr;
            }
            if(!monitorElement) break;
            if(waitTime>0.0) epicsThreadSleep(waitTime);
            pvTimeStamp.attach(pvStructure->getSubField("timeStamp"));
            pvTimeStamp.get(timeStamp);
            pvValue = dynamic_pointer_cast<PVLongArray>(pvStructure->getSubField("value"));
            shared_vector<const int64> data = pvValue->view();
            if(data.size()>0) {
                nElements += data.size();
                int64 first = data[0];
                int64 last = data[data.size()-1];
                if(first!=last) {
                    cout << "error first=" << first << " last=" << last << endl;
                }
                double diff = TimeStamp::diff(timeStamp,timeStampLast);
                if(diff>=1.0) {
                    ostringstream out;
                    out << " monitors/sec " << nSinceLastReport << " ";
                    out << "first " << first << " last " << last ;
                    BitSetPtr changed = monitorElement->changedBitSet;
                    BitSetPtr overrun = monitorElement->overrunBitSet;
                    out << " changed " << *changed;
                    out << " overrun " << *overrun;
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
                    nSinceLastReport = 0;
                    nElements = 0;
                }
                ++nSinceLastReport;
            } else {
                cout << "size = 0" << endl;
            }
            longArrayMonitor->monitor->release(monitorElement);
        }
    }
}

void LAMMonitorRequester::monitorEvent(MonitorPtr const & monitor)
{
    event.signal();
}

void LAMMonitorRequester::unlisten(MonitorPtr const & monitor)
{
    messagePvt("unlisten called",errorMessage);
}


LongArrayMonitorPtr LongArrayMonitor::create(
    string  const &providerName,
    string  const & channelName,
    int queueSize,
    double waitTime)
{
    LongArrayMonitorPtr longArrayMonitor(new LongArrayMonitor());
    if(!longArrayMonitor->init(providerName,channelName,queueSize,waitTime)) longArrayMonitor.reset();
    return longArrayMonitor;
}

LongArrayMonitor::LongArrayMonitor() {}

LongArrayMonitor::~LongArrayMonitor() {}

bool LongArrayMonitor::init(
    string  const &providerName,
    string  const &channelName,
    int queueSize,
    double waitTime)
{
    channelRequester = LAMChannelRequesterPtr(new LAMChannelRequester(getPtrSelf()));
    monitorRequester = LAMMonitorRequesterPtr(new LAMMonitorRequester(getPtrSelf(),waitTime));
    monitorRequester->init();
    ChannelProvider::shared_pointer channelProvider =
       getChannelProviderRegistry()->getProvider(providerName);
    if(!channelProvider) {
        cout << "provider " << providerName << " not found" << endl;
        return false;
    }
    channel = channelProvider->createChannel(channelName,channelRequester,0);
    event.wait();
    if(!status.isOK()) return false;
    string  request("record[queueSize=");
    char buff[20];
    sprintf(buff,"%d",queueSize);
    request += buff;
    request += "]field(value,timeStamp,alarm)";
    CreateRequest::shared_pointer createRequest = CreateRequest::create();
    PVStructurePtr pvRequest = createRequest->createRequest(request);
    if(!pvRequest) {
        cout << "request logic error " << createRequest->getMessage() << endl;
        return false;
    }
    monitor = channel->createMonitor(monitorRequester,pvRequest);
    event.wait();
    if(!status.isOK()) return false;
    return true;
}

void LongArrayMonitor::start()
{
    monitor->start();
}

void LongArrayMonitor::stop()
{
    monitor->stop();
}

void LongArrayMonitor::destroy()
{
    monitorRequester->destroy();
    monitorRequester.reset();
    monitor->destroy();
    monitor.reset();
    channel->destroy();
    channel.reset();
    channelRequester->destroy();
    channelRequester.reset();
}

}}


