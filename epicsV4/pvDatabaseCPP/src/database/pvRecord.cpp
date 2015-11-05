/* pvRecord.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 * @date 2012.11.21
 */
#include <epicsThread.h>

#define epicsExportSharedSymbols

#include <pv/pvDatabase.h>


using std::tr1::static_pointer_cast;
using namespace epics::pvData;
using namespace std;

namespace epics { namespace pvDatabase {

PVRecordPtr PVRecord::create(
    string const &recordName,
    PVStructurePtr const & pvStructure)
{
    PVRecordPtr pvRecord(new PVRecord(recordName,pvStructure));
    if(!pvRecord->init()) {
        pvRecord->destroy();
        pvRecord.reset();
    }
    return pvRecord;
}


PVRecord::PVRecord(
    string const & recordName,
    PVStructurePtr const & pvStructure)
: recordName(recordName),
  pvStructure(pvStructure),
  depthGroupPut(0),
  traceLevel(0),
  isDestroyed(false),
  isAddListener(false)
{
}

PVRecord::~PVRecord()
{
    if(traceLevel>1) {
        cout << "~PVRecord() " << recordName << endl;
    }
}

void PVRecord::initPVRecord()
{
    PVRecordStructurePtr parent;
    pvRecordStructure = PVRecordStructurePtr(
        new PVRecordStructure(pvStructure,parent,getPtrSelf()));
    pvRecordStructure->init();
    PVFieldPtr pvField = pvStructure->getSubField("timeStamp");
    if(pvField) pvTimeStamp.attach(pvField);
}

void PVRecord::destroy()
{
    if(traceLevel>1) {
        cout << "PVRecord::destroy() " << recordName << endl;
    }
    lock();
    try {
        if(isDestroyed) {
            unlock();
            return;
        }
        isDestroyed = true;
        pvTimeStamp.detach();
    
        std::list<PVRecordClientPtr>::iterator clientIter;
        while(true) {
            clientIter = pvRecordClientList.begin();
            if(clientIter==pvRecordClientList.end()) break;
            pvRecordClientList.erase(clientIter);
            unlock();
            (*clientIter)->detach(getPtrSelf());
            lock();
        }
        std::list<PVListenerPtr>::iterator listenerIter;
        while(true) {
            listenerIter = pvListenerList.begin();
            if(listenerIter==pvListenerList.end()) break;
            pvListenerList.erase(listenerIter);
            unlock();
            (*listenerIter)->unlisten(getPtrSelf());
            lock();
        }
        pvRecordStructure->destroy();
        pvRecordStructure.reset();
        pvStructure.reset();
        unlock();
    } catch(...) {
        unlock();
        throw;
    }
}

void PVRecord::process()
{
    if(traceLevel>2) {
        cout << "PVRecord::process() " << recordName << endl;
    }
    if(pvTimeStamp.isAttached()) {
        timeStamp.getCurrent();
        pvTimeStamp.set(timeStamp);
    }
}

string PVRecord::getRecordName() const {return recordName;}

PVRecordStructurePtr PVRecord::getPVRecordStructure() const {return pvRecordStructure;}

PVStructurePtr PVRecord::getPVStructure() {return pvStructure;}

PVRecordFieldPtr PVRecord::findPVRecordField(PVFieldPtr const & pvField)
{
    return findPVRecordField(pvRecordStructure,pvField);
}

PVRecordFieldPtr PVRecord::findPVRecordField(
    PVRecordStructurePtr const & pvrs,
        PVFieldPtr const & pvField)
{
    size_t desiredOffset = pvField->getFieldOffset();
    PVFieldPtr pvf = pvrs->getPVField();
    size_t offset = pvf->getFieldOffset();
    if(offset==desiredOffset) return pvrs;
    PVRecordFieldPtrArrayPtr  pvrfpap = pvrs->getPVRecordFields();
    PVRecordFieldPtrArray::iterator iter;
    for (iter = pvrfpap.get()->begin(); iter!=pvrfpap.get()->end(); iter++ ) {
        PVRecordFieldPtr pvrf = *iter;
        pvf = pvrf->getPVField();
        offset = pvf->getFieldOffset();
        if(offset==desiredOffset) return pvrf;
        size_t nextOffset = pvf->getNextFieldOffset();
        if(nextOffset<=desiredOffset) continue;
        return findPVRecordField(
            static_pointer_cast<PVRecordStructure>(pvrf),
            pvField);
    }
    throw std::logic_error(
        recordName + " pvField "
        + pvField->getFieldName() + " not in PVRecord");
}

void PVRecord::lock() {
    if(traceLevel>2) {
        cout << "PVRecord::lock() " << recordName << endl;
    }
    mutex.lock();
}

void PVRecord::unlock() {
    if(traceLevel>2) {
        cout << "PVRecord::unlock() " << recordName << endl;
    }
    mutex.unlock();
}

bool PVRecord::tryLock() {
    if(traceLevel>2) {
        cout << "PVRecord::tryLock() " << recordName << endl;
    }
    return mutex.tryLock();
}

void PVRecord::lockOtherRecord(PVRecordPtr const & otherRecord)
{
    if(traceLevel>2) {
        cout << "PVRecord::lockOtherRecord() " << recordName << endl;
    }
    if(this<otherRecord.get()) {
        otherRecord->lock();
        return;
    }
    unlock();
    otherRecord->lock();
    lock();
}

bool PVRecord::addPVRecordClient(PVRecordClientPtr const & pvRecordClient)
{
    if(traceLevel>2) {
        cout << "PVRecord::addPVRecordClient() " << recordName << endl;
    }
    lock();
    try {
        if(isDestroyed) {
            unlock();
            return false;
        }
        std::list<PVRecordClientPtr>::iterator iter;
        for (iter = pvRecordClientList.begin();
        iter!=pvRecordClientList.end();
        iter++ )
        {
            if((*iter).get()==pvRecordClient.get()) {
                unlock();
                return false;
            }
        }
        pvRecordClientList.push_back(pvRecordClient);
        unlock();
        return true;
    } catch (...) {
        unlock();
        throw;
    }
}

bool PVRecord::removePVRecordClient(PVRecordClientPtr const & pvRecordClient)
{
    if(traceLevel>2) {
        cout << "PVRecord::removePVRecordClient() " << recordName << endl;
    }
    lock();
    try {
        if(isDestroyed) {
            unlock();
            return false;
        }
        std::list<PVRecordClientPtr>::iterator iter;
        for (iter = pvRecordClientList.begin();
        iter!=pvRecordClientList.end();
        iter++ )
        {
            if((*iter).get()==pvRecordClient.get()) {
                pvRecordClientList.erase(iter);
                unlock();
                return true;
            }
        }
        unlock();
        return false;
    } catch (...) {
        unlock();
        throw;
    }
}

void PVRecord::detachClients()
{
    if(traceLevel>1) {
        cout << "PVRecord::removePVRecordClient() " << recordName << endl;
    }
    lock();
    try {
        if(isDestroyed) {
            unlock();
            return;
        }
        std::list<PVRecordClientPtr>::iterator iter;
        for (iter = pvRecordClientList.begin();
        iter!=pvRecordClientList.end();
        iter++ )
        {
            unlock();
            (*iter)->detach(getPtrSelf());
            lock();
        }
        pvRecordClientList.clear();
        unlock();
    } catch(...) {
        unlock();
        throw;
    }
}

bool PVRecord::addListener(
    PVListenerPtr const & pvListener,
    PVCopyPtr const & pvCopy)
{
    if(traceLevel>1) {
        cout << "PVRecord::addListener() " << recordName << endl;
    }
    lock();
    try {
        if(isDestroyed) {
            unlock();
            return false;
        }
        std::list<PVListenerPtr>::iterator iter;
        for (iter = pvListenerList.begin(); iter!=pvListenerList.end(); iter++ )
        {
            if((*iter).get()==pvListener.get()) {
                unlock();
                return false;
            }
        }
        pvListenerList.push_back(pvListener);
        this->pvListener = pvListener;
        isAddListener = true;
        pvCopy->traverseMaster(getPtrSelf());
        this->pvListener = PVListenerPtr();;
        unlock();
        return true;
    } catch(...) {
        unlock();
        throw;
    }
}

void PVRecord::nextMasterPVField(PVFieldPtr const & pvField)
{
     PVRecordFieldPtr pvRecordField = findPVRecordField(pvField);
     if(isAddListener) {
         pvRecordField->addListener(pvListener);
     } else {
         pvRecordField->removeListener(pvListener);
     }
}

bool PVRecord::removeListener(
    PVListenerPtr const & pvListener,
    PVCopyPtr const & pvCopy)
{
    if(traceLevel>1) {
        cout << "PVRecord::removeListener() " << recordName << endl;
    }
    lock();
    try {
        if(isDestroyed) {
            unlock();
            return false;
        }
        std::list<PVListenerPtr>::iterator iter;
        for (iter = pvListenerList.begin(); iter!=pvListenerList.end(); iter++ )
        {
            if((*iter).get()==pvListener.get()) {
                pvListenerList.erase(iter);
                this->pvListener = pvListener;
                isAddListener = false;
                pvCopy->traverseMaster(getPtrSelf());
                this->pvListener = PVListenerPtr();;
                unlock();
                return true;
            }
        }
        unlock();
        return false;
    } catch(...) {
        unlock();
        throw;
    }
}

void PVRecord::beginGroupPut()
{
   if(++depthGroupPut>1) return;
    if(traceLevel>2) {
        cout << "PVRecord::beginGroupPut() " << recordName << endl;
    }
   std::list<PVListenerPtr>::iterator iter;
   for (iter = pvListenerList.begin(); iter!=pvListenerList.end(); iter++)
   {
       (*iter).get()->beginGroupPut(getPtrSelf());
   }
}

void PVRecord::endGroupPut()
{
   if(--depthGroupPut>0) return;
    if(traceLevel>2) {
        cout << "PVRecord::endGroupPut() " << recordName << endl;
    }
   std::list<PVListenerPtr>::iterator iter;
   for (iter = pvListenerList.begin(); iter!=pvListenerList.end(); iter++)
   {
       (*iter).get()->endGroupPut(getPtrSelf());
   }
}

std::ostream& operator<<(std::ostream& o, const PVRecord& record)
{
    o << format::indent() << "record " << record.getRecordName() << endl;
    {
        format::indent_scope s(o);
        o <<  *record.getPVRecordStructure()->getPVStructure();
    }
    return o;
}

PVRecordField::PVRecordField(
    PVFieldPtr const & pvField,
    PVRecordStructurePtr const &parent,
    PVRecordPtr const & pvRecord)
:  pvField(pvField),
   isStructure(pvField->getField()->getType()==structure ? true : false),
   parent(parent),
   pvRecord(pvRecord)
{
}

void PVRecordField::init()
{
    fullFieldName = pvField->getFieldName();
    PVRecordStructurePtr pvParent = parent;
    while(pvParent) {
        string parentName = pvParent->getPVField()->getFieldName();
        if(parentName.size()>0) {
            fullFieldName = pvParent->getPVField()->getFieldName()
                + '.' + fullFieldName;
        }
        pvParent = pvParent->getParent();
    }
    if(fullFieldName.size()>0) {
        fullName = pvRecord->getRecordName() + '.' + fullFieldName;
    } else {
        fullName = pvRecord->getRecordName();
    }
    pvField->setPostHandler(getPtrSelf());
}

PVRecordField::~PVRecordField()
{
}

void PVRecordField::destroy()
{
    pvRecord.reset();
    parent.reset();
    pvField.reset();
    pvListenerList.clear();
}

PVRecordStructurePtr PVRecordField::getParent() {return parent;}

PVFieldPtr PVRecordField::getPVField() {return pvField;}

string PVRecordField::getFullFieldName() {return fullFieldName; }

string PVRecordField::getFullName() {return fullName; }

PVRecordPtr PVRecordField::getPVRecord() {return pvRecord;}

bool PVRecordField::addListener(PVListenerPtr const & pvListener)
{
    if(pvRecord->getTraceLevel()>1) {
         cout << "PVRecordField::addListener() " << getFullName() << endl;
    }
    std::list<PVListenerPtr>::iterator iter;
    for (iter = pvListenerList.begin(); iter!=pvListenerList.end(); iter++ ) {
        if((*iter).get()==pvListener.get()) {
            return false;
        }
    }
    pvListenerList.push_back(pvListener);
    return true;
}

void PVRecordField::removeListener(PVListenerPtr const & pvListener)
{
    if(pvRecord->getTraceLevel()>1) {
         cout << "PVRecordField::removeListener() " << getFullName() << endl;
    }
    std::list<PVListenerPtr>::iterator iter;
    for (iter = pvListenerList.begin(); iter!=pvListenerList.end(); iter++ ) {
        if((*iter).get()==pvListener.get()) {
            pvListenerList.erase(iter);
            return;
        }
    }
}

void PVRecordField::postPut()
{
    if(parent) {
        parent->postParent(getPtrSelf());
    }
    postSubField();
}

void PVRecordField::postParent(PVRecordFieldPtr const & subField)
{
    PVRecordStructurePtr pvrs = static_pointer_cast<PVRecordStructure>(getPtrSelf());
    std::list<PVListenerPtr>::iterator iter;
    for(iter = pvListenerList.begin(); iter != pvListenerList.end(); ++iter)
    {
        (*iter)->dataPut(pvrs,subField);
    }
    if(parent) parent->postParent(subField);
}

void PVRecordField::postSubField()
{
    callListener();
    if(isStructure) {
        PVRecordStructurePtr pvrs = static_pointer_cast<PVRecordStructure>(getPtrSelf());
        PVRecordFieldPtrArrayPtr pvRecordFields = pvrs->getPVRecordFields();
        PVRecordFieldPtrArray::iterator iter;
        for(iter = pvRecordFields->begin() ; iter !=pvRecordFields->end(); iter++) {
             (*iter)->postSubField();
        }
    }
}

void PVRecordField::callListener()
{
    std::list<PVListenerPtr>::iterator iter;
    for (iter = pvListenerList.begin(); iter!=pvListenerList.end(); iter++ ) {
        (*iter)->dataPut(getPtrSelf());
    }
}

PVRecordStructure::PVRecordStructure(
    PVStructurePtr const &pvStructure,
    PVRecordStructurePtr const &parent,
    PVRecordPtr const & pvRecord)
:
    PVRecordField(pvStructure,parent,pvRecord),
    pvStructure(pvStructure),
    pvRecordFields(new PVRecordFieldPtrArray)
{
}

PVRecordStructure::~PVRecordStructure()
{
}

void PVRecordStructure::destroy()
{
    PVRecordFieldPtrArray::iterator iter;
    PVRecordField::destroy();
    for(iter = pvRecordFields->begin() ; iter !=pvRecordFields->end(); iter++) {
        (*iter)->destroy();
    }
    PVRecordField::destroy();
    pvRecordFields.reset();
    pvStructure.reset();
}

void PVRecordStructure::init()
{
    PVRecordField::init();
    const PVFieldPtrArray & pvFields = pvStructure->getPVFields();
    size_t numFields = pvFields.size();
    pvRecordFields->reserve( numFields);
    PVRecordStructurePtr self =
        static_pointer_cast<PVRecordStructure>(getPtrSelf());
    PVRecordPtr pvRecord = getPVRecord();
    for(size_t i=0; i<numFields; i++) {    
        PVFieldPtr pvField = pvFields[i];
        if(pvField->getField()->getType()==structure) {
             PVStructurePtr xxx = static_pointer_cast<PVStructure>(pvField);
             PVRecordStructurePtr pvRecordStructure(
                 new PVRecordStructure(xxx,self,pvRecord));
             pvRecordFields->push_back(pvRecordStructure);
             pvRecordStructure->init();
        } else {
             PVRecordFieldPtr pvRecordField(
                new PVRecordField(pvField,self,pvRecord));
             pvRecordFields->push_back(pvRecordField);
             pvRecordField->init();
        }
    }
}

PVRecordFieldPtrArrayPtr PVRecordStructure::getPVRecordFields()
{
    return pvRecordFields;
}

PVStructurePtr PVRecordStructure::getPVStructure() {return pvStructure;}

void PVRecordStructure::removeListener(PVListenerPtr const & pvListener)
{
    PVRecordField::removeListener(pvListener);
    size_t numFields = pvRecordFields->size();
    for(size_t i=0; i<numFields; i++) {
         PVRecordFieldPtr pvRecordField = (*pvRecordFields.get())[i];
         pvRecordField->removeListener(pvListener);
    }
}

void PVRecordStructure::postPut()
{
    PVRecordField::postPut();
    size_t numFields = pvRecordFields->size();
    for(size_t i=0; i<numFields; i++) {
         PVRecordFieldPtr pvRecordField = (*pvRecordFields.get())[i];
         pvRecordField->callListener();
    }
}

}}
