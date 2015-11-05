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
static Status failedToCreateMonitorStatus(Status::STATUSTYPE_ERROR,"failed to create monitor");
static Status wasDestroyedStatus(Status::STATUSTYPE_ERROR,"was destroyed");
static Status alreadyStartedStatus(Status::STATUSTYPE_ERROR,"already started");
static Status notStartedStatus(Status::STATUSTYPE_ERROR,"not started");



typedef Queue<MonitorElement> MonitorElementQueue;
typedef std::tr1::shared_ptr<MonitorElementQueue> MonitorElementQueuePtr;


    
class MonitorLocal :
    public Monitor,
    public PVListener,
    public std::tr1::enable_shared_from_this<MonitorLocal>
{
    enum MonitorState {idle,active, destroyed};
public:
    POINTER_DEFINITIONS(MonitorLocal);
    virtual ~MonitorLocal();
    virtual Status start();
    virtual Status stop();
    virtual MonitorElementPtr poll();
    virtual void destroy();
    virtual void detach(PVRecordPtr const & pvRecord){destroy();}
    virtual void release(MonitorElementPtr const & monitorElement);
    virtual void dataPut(PVRecordFieldPtr const & pvRecordField);
    virtual void dataPut(
        PVRecordStructurePtr const & requested,
        PVRecordFieldPtr const & pvRecordField);
    virtual void beginGroupPut(PVRecordPtr const & pvRecord);
    virtual void endGroupPut(PVRecordPtr const & pvRecord);
    virtual void unlisten(PVRecordPtr const & pvRecord);
    MonitorElementPtr getActiveElement();
    void releaseActiveElement();
    bool init(PVStructurePtr const & pvRequest);
    MonitorLocal(
        MonitorRequester::shared_pointer const & channelMonitorRequester,
        PVRecordPtr const &pvRecord);
    PVCopyPtr getPVCopy() { return pvCopy;}
private:
    MonitorLocalPtr getPtrSelf()
    {
        return shared_from_this();
    }
    MonitorRequester::shared_pointer monitorRequester;
    PVRecordPtr pvRecord;
    MonitorState state;
    PVCopyPtr pvCopy;
    MonitorElementQueuePtr queue;
    MonitorElementPtr activeElement;
    bool isGroupPut;
    bool dataChanged;
    Mutex mutex;
    Mutex queueMutex;
};

MonitorLocal::MonitorLocal(
    MonitorRequester::shared_pointer const & channelMonitorRequester,
    PVRecordPtr const &pvRecord)
: monitorRequester(channelMonitorRequester),
  pvRecord(pvRecord),
  state(idle),
  isGroupPut(false),
  dataChanged(false)
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
        cout << "MonitorLocal::destroy state " << state << endl;
    }
    {
        Lock xx(mutex);
        if(state==destroyed) return;
    }
    if(pvCopy) pvCopy->destroy();
    {
        Lock xx(mutex);
        state = destroyed;
    }
    {
         Lock xx(queueMutex);
         queue.reset();
    }
    pvCopy.reset();
}

Status MonitorLocal::start()
{
    if(pvRecord->getTraceLevel()>0)
    {
        cout << "MonitorLocal::start() "  << endl;
    }
    {
        Lock xx(mutex);
        if(state==destroyed) return wasDestroyedStatus;
        if(state==active) return alreadyStartedStatus;
    }
    pvRecord->addListener(getPtrSelf(),pvCopy);
    pvRecord->lock();
    try {
        Lock xx(mutex);
        state = active;
        queue->clear();
        isGroupPut = false;
        activeElement = queue->getFree();
        activeElement->changedBitSet->clear();
        activeElement->overrunBitSet->clear();
        activeElement->changedBitSet->set(0);
        releaseActiveElement();
        pvRecord->unlock();
    } catch(...) {
        pvRecord->unlock();
    }
    return Status::Ok;
}

Status MonitorLocal::stop()
{
    if(pvRecord->getTraceLevel()>0){
        cout << "MonitorLocal::stop() "  << endl;
    }
    {
        Lock xx(mutex);
        if(state==destroyed) return wasDestroyedStatus;
        if(state==idle) return notStartedStatus;
        state = idle;
   }
    pvRecord->removeListener(getPtrSelf(),pvCopy);
    return Status::Ok;
}

MonitorElementPtr MonitorLocal::poll()
{
    if(pvRecord->getTraceLevel()>1)
    {
        cout << "MonitorLocal::poll() "  << endl;
    }
    {
        Lock xx(queueMutex);
        if(state!=active) return NULLMonitorElement;
        return queue->getUsed();
    }
}

void MonitorLocal::release(MonitorElementPtr const & monitorElement)
{
    if(pvRecord->getTraceLevel()>1)
    {
        cout << "MonitorLocal::release() "  << endl;
    }
    {
        Lock xx(queueMutex);
        if(state!=active) return;
        queue->releaseUsed(monitorElement);
    }
}

void MonitorLocal::releaseActiveElement()
{
    if(pvRecord->getTraceLevel()>1)
    {
        cout << "MonitorLocal::releaseActiveElement() "  << endl;
    }
    {
        Lock xx(queueMutex);
        if(state!=active) return;
        MonitorElementPtr newActive = queue->getFree();
        if(!newActive) return;
        pvCopy->updateCopyFromBitSet(activeElement->pvStructurePtr,activeElement->changedBitSet);
        BitSetUtil::compress(activeElement->changedBitSet,activeElement->pvStructurePtr);
        BitSetUtil::compress(activeElement->overrunBitSet,activeElement->pvStructurePtr);
        queue->setUsed(activeElement);
        activeElement = newActive;
        activeElement->changedBitSet->clear();
        activeElement->overrunBitSet->clear();
    }
    monitorRequester->monitorEvent(getPtrSelf());
    return;
}

void MonitorLocal::dataPut(PVRecordFieldPtr const & pvRecordField)
{
    if(pvRecord->getTraceLevel()>1)
    {
        cout << "PVCopyMonitor::dataPut(pvRecordField)" << endl;
    }
    if(state!=active) return;
    {
        Lock xx(mutex);
        size_t offset = pvCopy->getCopyOffset(pvRecordField->getPVField());
        BitSetPtr const &changedBitSet = activeElement->changedBitSet;
        BitSetPtr const &overrunBitSet = activeElement->overrunBitSet;
        bool isSet = changedBitSet->get(offset);
        changedBitSet->set(offset);
        if(isSet) overrunBitSet->set(offset);
        dataChanged = true;
    }
    if(!isGroupPut) {
        releaseActiveElement();
        dataChanged = false;
    }
}

void MonitorLocal::dataPut(
        PVRecordStructurePtr const & requested,
        PVRecordFieldPtr const & pvRecordField)
{
    if(pvRecord->getTraceLevel()>1)
    {
        cout << "PVCopyMonitor::dataPut(requested,pvRecordField)" << endl;
    }
    if(state!=active) return;
    {
        Lock xx(mutex);
        BitSetPtr const &changedBitSet = activeElement->changedBitSet;
        BitSetPtr const &overrunBitSet = activeElement->overrunBitSet;
        size_t offsetCopyRequested = pvCopy->getCopyOffset(
            requested->getPVField());
        size_t offset = offsetCopyRequested
             + (pvRecordField->getPVField()->getFieldOffset()
                 - requested->getPVField()->getFieldOffset());
        bool isSet = changedBitSet->get(offset);
        changedBitSet->set(offset);
        if(isSet) overrunBitSet->set(offset);
        dataChanged = true;
    }
    if(!isGroupPut) {
        releaseActiveElement();
        dataChanged = false;
    }
}

void MonitorLocal::beginGroupPut(PVRecordPtr const & pvRecord)
{
    if(pvRecord->getTraceLevel()>1)
    {
        cout << "PVCopyMonitor::beginGroupPut()" << endl;
    }
    if(state!=active) return;
    {
        Lock xx(mutex);
        isGroupPut = true;
        dataChanged = false;
    }
}

void MonitorLocal::endGroupPut(PVRecordPtr const & pvRecord)
{
    if(pvRecord->getTraceLevel()>1)
    {
        cout << "PVCopyMonitor::endGroupPut() dataChanged " << dataChanged << endl;
    }
    if(state!=active) return;
    {
        Lock xx(mutex);
        isGroupPut = false;
    }
    if(dataChanged) {
        dataChanged = false;
        releaseActiveElement();
    }
}

void MonitorLocal::unlisten(PVRecordPtr const & pvRecord)
{
    if(pvRecord->getTraceLevel()>1)
    {
        cout << "PVCopyMonitor::unlisten\n";
    }
    pvRecord->removeListener(getPtrSelf(),pvCopy);
}


bool MonitorLocal::init(PVStructurePtr const & pvRequest)
{
    PVFieldPtr pvField;
    size_t queueSize = 2;
    PVStructurePtr pvOptions = pvRequest->getSubField<PVStructure>("record._options");
    if(pvOptions) {
        PVStringPtr pvString  = pvOptions->getSubField<PVString>("queueSize");
        if(pvString) {
            try {
                int32 size;
                std::stringstream ss;
                ss << pvString->get();
                ss >> size;
                queueSize = size;
            } catch (...) {
                 monitorRequester->message("queueSize " +pvString->get() + " illegal",errorMessage);
                 return false;
            }
        }
    }
    pvField = pvRequest->getSubField("field");
    if(!pvField) {
        pvCopy = PVCopy::create(
            pvRecord->getPVRecordStructure()->getPVStructure(),
            pvRequest,"");
        if(!pvCopy) {
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
        if(!pvCopy) {
            monitorRequester->message("illegal pvRequest",errorMessage);
            return false;
        }
    }
    if(queueSize<2) queueSize = 2;
    std::vector<MonitorElementPtr> monitorElementArray;
    monitorElementArray.reserve(queueSize);
    for(size_t i=0; i<queueSize; i++) {
         PVStructurePtr pvStructure = pvCopy->createPVStructure();
         MonitorElementPtr monitorElement(
             new MonitorElement(pvStructure));
         monitorElementArray.push_back(monitorElement);
    }
    queue = MonitorElementQueuePtr(new MonitorElementQueue(monitorElementArray));
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
    if(!result) {
        MonitorPtr monitor;
        StructureConstPtr structure;
        monitorRequester->monitorConnect(failedToCreateMonitorStatus,monitor,structure);
        return nullMonitor;
    }
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
