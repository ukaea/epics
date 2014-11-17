/* monitorFactory.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author Marty Kraimer
 * @date 2013.04
 */

#include <sstream>

#include <pv/thread.h>
#include <pv/bitSetUtil.h>
#include <pv/queue.h>
#include <pv/timeStamp.h>

#define epicsExportSharedSymbols

#include <pv/pvCopyMonitor.h>
#include <pv/channelProviderLocal.h>

using namespace epics::pvData;
using namespace epics::pvAccess;
using std::tr1::static_pointer_cast;
using std::cout;
using std::endl;
using std::string;

namespace epics { namespace pvDatabase { 


static MonitorPtr nullMonitor;
static MonitorElementPtr NULLMonitorElement;
static Status wasDestroyedStatus(Status::STATUSTYPE_ERROR,"was destroyed");

static ConvertPtr convert = getConvert();


typedef Queue<MonitorElement> MonitorElementQueue;
typedef std::tr1::shared_ptr<MonitorElementQueue> MonitorElementQueuePtr;


    
class MonitorLocal :
    public Monitor,
    public PVCopyMonitorRequester,
    public std::tr1::enable_shared_from_this<MonitorLocal>
{
public:
    POINTER_DEFINITIONS(MonitorLocal);
    virtual ~MonitorLocal();
    virtual Status start();
    virtual Status stop();
    virtual MonitorElementPtr poll();
    virtual void destroy();
    virtual void release(MonitorElementPtr const & monitorElement);
    MonitorElementPtr getActiveElement();
    MonitorElementPtr releaseActiveElement();
    void unlisten();
    bool init(PVStructurePtr const & pvRequest);
    MonitorLocal(
        MonitorRequester::shared_pointer const & channelMonitorRequester,
        PVRecordPtr const &pvRecord);
    PVCopyPtr getPVCopy() { return pvCopy;}
    PVCopyMonitorPtr getPVCopyMonitor() { return pvCopyMonitor;}
private:
    MonitorLocalPtr getPtrSelf()
    {
        return shared_from_this();
    }
    MonitorRequester::shared_pointer monitorRequester;
    PVRecordPtr pvRecord;
    bool isDestroyed;
    bool firstMonitor;
    PVCopyPtr pvCopy;
    MonitorElementQueuePtr queue;
    MonitorElementPtr activeElement;
    PVCopyMonitorPtr pvCopyMonitor;
    Mutex mutex;
};

MonitorLocal::MonitorLocal(
    MonitorRequester::shared_pointer const & channelMonitorRequester,
    PVRecordPtr const &pvRecord)
: monitorRequester(channelMonitorRequester),
  pvRecord(pvRecord),
  isDestroyed(false),
  firstMonitor(true)
{
}

MonitorLocal::~MonitorLocal()
{
    if(pvRecord->getTraceLevel()>0)
    {
        cout << "MonitorLocal::~MonitorLocal()" << endl;
    }
}

void MonitorLocal::destroy()
{
    if(pvRecord->getTraceLevel()>0)
    {
        cout << "MonitorLocal::destroy " << isDestroyed << endl;
    }
    {
        Lock xx(mutex);
        if(isDestroyed) return;
        isDestroyed = true;
    }
    pvCopyMonitor->destroy();
    pvCopy->destroy();
    pvCopyMonitor.reset();
    queue.reset();
    pvCopy.reset();
}

Status MonitorLocal::start()
{
    if(pvRecord->getTraceLevel()>0)
    {
        cout << "MonitorLocal::start() "  << endl;
    }
    if(isDestroyed) return wasDestroyedStatus;
    {
        Lock xx(mutex);
        firstMonitor = true;
        queue->clear();
        activeElement = queue->getFree();
        activeElement->changedBitSet->clear();
        activeElement->overrunBitSet->clear();
    }
    pvCopyMonitor->startMonitoring(activeElement);
    return Status::Ok;
}

Status MonitorLocal::stop()
{
    if(pvRecord->getTraceLevel()>0){
        cout << "MonitorLocal::stop() "  << endl;
    }
    if(isDestroyed) return  Status::Ok;
    pvCopyMonitor->stopMonitoring();
    return Status::Ok;
}

MonitorElementPtr MonitorLocal::poll()
{
    if(pvRecord->getTraceLevel()>1)
    {
        cout << "MonitorLocal::poll() "  << endl;
    }
    if(isDestroyed) return NULLMonitorElement;
    Lock xx(mutex);
    return queue->getUsed();
}

void MonitorLocal::release(MonitorElementPtr const & monitorElement)
{
    if(pvRecord->getTraceLevel()>1)
    {
        cout << "MonitorLocal::release() "  << endl;
    }
    if(isDestroyed) return;
    Lock xx(mutex);
    queue->releaseUsed(monitorElement);
}

MonitorElementPtr MonitorLocal::releaseActiveElement()
{
    if(pvRecord->getTraceLevel()>1)
    {
        cout << "MonitorLocal::releaseActiveElement() "  << endl;
    }
    if(isDestroyed) return activeElement;
    {
        Lock xx(mutex);
        MonitorElementPtr newActive = queue->getFree();
        if(!newActive) return activeElement;
        pvCopy->updateCopyFromBitSet(activeElement->pvStructurePtr,activeElement->changedBitSet);
        BitSetUtil::compress(activeElement->changedBitSet,activeElement->pvStructurePtr);
        BitSetUtil::compress(activeElement->overrunBitSet,activeElement->pvStructurePtr);
        queue->setUsed(activeElement);
        activeElement = newActive;
        activeElement->changedBitSet->clear();
        activeElement->overrunBitSet->clear();
    }
    monitorRequester->monitorEvent(getPtrSelf());
    return activeElement;
}

void MonitorLocal::unlisten()
{
    if(pvRecord->getTraceLevel()>0)
    {
        cout << "MonitorLocal::unlisten() "  << endl;
    }
    monitorRequester->unlisten(getPtrSelf());
}

bool MonitorLocal::init(PVStructurePtr const & pvRequest)
{
    PVFieldPtr pvField;
    PVStructurePtr pvOptions;
    size_t queueSize = 2;
    pvField = pvRequest->getSubField("record._options");
    if(pvField.get()!=NULL) {
        pvOptions = static_pointer_cast<PVStructure>(pvField);
        pvField = pvOptions->getSubField("queueSize");
        if(pvField.get()!=NULL) {
            PVStringPtr pvString = pvOptions->getStringField("queueSize");
            if(pvString.get()!=NULL) {
                int32 size;
                std::stringstream ss;
                ss << pvString->get();
                ss >> size;
                queueSize = size;
            }
        }
    }

    pvField = pvRequest->getSubField("field");
    if(pvField.get()==NULL) {
        pvCopy = PVCopy::create(
            pvRecord->getPVRecordStructure()->getPVStructure(),
            pvRequest,"");
        if(pvCopy.get()==NULL) {
            monitorRequester->message("illegal pvRequest",errorMessage);
            return false;
        }
    } else {
        if(pvField->getField()->getType()!=structure) {
            monitorRequester->message("illegal pvRequest",errorMessage);
            return false;
        }
        pvCopy = PVCopy::create(
            pvRecord->getPVRecordStructure()->getPVStructure(),
            pvRequest,"field");
        if(pvCopy.get()==NULL) {
            monitorRequester->message("illegal pvRequest",errorMessage);
            return false;
        }
    }
    pvCopyMonitor = PVCopyMonitor::create(
          getPtrSelf(),pvRecord,pvCopy);
    // MARTY MUST IMPLEMENT periodic
    if(queueSize<2) queueSize = 2;
    std::vector<MonitorElementPtr> monitorElementArray;
    monitorElementArray.reserve(queueSize);
    size_t nfields = 0;
    for(size_t i=0; i<queueSize; i++) {
         PVStructurePtr pvStructure = pvCopy->createPVStructure();
         if(nfields==0) nfields = pvStructure->getNumberFields();
         MonitorElementPtr monitorElement(
             new MonitorElement(pvStructure));
         monitorElementArray.push_back(monitorElement);
    }
    queue = MonitorElementQueuePtr(new MonitorElementQueue(monitorElementArray));
    // MARTY MUST IMPLEMENT algorithm
    monitorRequester->monitorConnect(
        Status::Ok,
        getPtrSelf(),
        pvCopy->getStructure());
    return true;
}


MonitorFactory::MonitorFactory()
: isDestroyed(false)
{
}

MonitorFactory::~MonitorFactory()
{
}

void MonitorFactory::destroy()
{
    Lock lock(mutex);
    if(isDestroyed) return;
    isDestroyed = true;
}

MonitorPtr MonitorFactory::createMonitor(
    PVRecordPtr const & pvRecord,
    MonitorRequester::shared_pointer const & monitorRequester,
    PVStructurePtr const & pvRequest)
{
    Lock xx(mutex);
    if(isDestroyed) {
        monitorRequester->message("MonitorFactory is destroyed",errorMessage);
        return nullMonitor;
    }
    MonitorLocalPtr monitor(new MonitorLocal(
        monitorRequester,pvRecord));
    bool result = monitor->init(pvRequest);
    if(!result) return nullMonitor;
    if(pvRecord->getTraceLevel()>0)
    {
        cout << "MonitorFactory::createMonitor";
        cout << " recordName " << pvRecord->getRecordName() << endl;
    }
    return monitor;
}



MonitorFactoryPtr getMonitorFactory()
{
    static MonitorFactoryPtr monitorFactoryPtr;
    static Mutex mutex;
    Lock xx(mutex);

    if(monitorFactoryPtr.get()==NULL) {
        monitorFactoryPtr = MonitorFactoryPtr(
            new MonitorFactory());
    }
    return monitorFactoryPtr;
}

}}
