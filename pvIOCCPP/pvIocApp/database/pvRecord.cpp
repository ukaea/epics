/* pvRecord.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 */
/* Marty Kraimer 2011.03 */

#include <pv/pvDatabase.h>

namespace epics { namespace pvIOC { 

using namespace epics::pvData;
using namespace epics::pvAccess;

int32 PVRecord::numberRecords = 0;


PVRecord::PVRecord(String recordName,std::auto_ptr<PVStructure> pvStructure)
: recordName(recordName),
  pvStructure(pvStructure),
  pvRecordStructure(std::auto_ptr<PVRecordStructure>()),
  id(0),
  depthGroupPut(0),
  recordProcess(0),
  convert(*getConvert()),
  recordLock(mutex),
  lockRecursiveCount(0)
{
    id = numberRecords++;
    PVStructure &pvStruct = *pvStructure.get();
    pvRecordStructure = std::auto_ptr<PVRecordStructure>(
        new PVRecordStructure(pvStruct,0,*this));
    recordLock.unlock();
}

PVRecord::~PVRecord()
{
}

RecordProcessPtr PVRecord::getRecordProcess()
{
    return recordProcess;
}

void PVRecord::setRecordProcess(RecordProcess &rp)
{
    recordProcess = &rp;
}

PVRecordStructure &PVRecord::getPVRecordStructure()
{
    return  *pvRecordStructure.get();
}

static PVRecordField &findViaStructure(
    PVRecordStructure &pvrs,PVField &pvField)
{
    int desiredOffset = pvField.getFieldOffset();
    PVField &pvf = pvrs.getPVField();
    int offset = pvf.getFieldOffset();
    if(offset==desiredOffset) return pvrs;
    PVRecordFieldPtrArray pvrss = pvrs.getPVRecordFields();
    int numberFields = pvrs.getPVStructure().getStructure()->getNumberFields();
    for(int i=0; i<numberFields; i++) {
	PVRecordField &pvrf = *pvrss[i];
        PVField &pvf = pvrf.getPVField();
        offset = pvf.getFieldOffset();
        if(offset==desiredOffset) return pvrf;
        int nextOffset = pvf.getNextFieldOffset();
        if(nextOffset<=desiredOffset) continue;
        PVRecordStructure & pvrs = static_cast<PVRecordStructure &>(pvrf);
        return findViaStructure(pvrs,pvField);
    }
    throw std::logic_error(String("pvField not in PVRecord"));
}

PVRecordField &PVRecord::findPVRecordField(PVField &pvField)
{
    return findViaStructure(getPVRecordStructure(),pvField);
}


String PVRecord::getRecordName()
{
    return recordName;
}

String PVRecord::getRequesterName()
{
    return recordName;
}

void PVRecord::message(
        String message,
        MessageType messageType)
{
    if(requesterList.isEmpty()) {
        printf("%s %s\n",
            messageTypeName[messageType].c_str(),
            message.c_str());
        return;
    }
    // no need to synchronize because record must be locked when this is called.
    RequesterListNode *node = requesterList.getHead();
    while(node!=0) {
        Requester &requester = node->getObject();
        requester.message(message,messageType);
        node = requesterList.getNext(*node);
    }
}

void PVRecord::addRequester(Requester &requester)
{
    // no need to synchronize because record must be locked when this is called.
    RequesterListNode *node = requesterList.getHead();
    while(node!=0) {
        Requester &xxx = node->getObject();
        if(&xxx==&requester) {
            requester.message(
                String("already on requesterList"),warningMessage);
            return;
        }
        node = requesterList.getNext(*node);
    }
    node = new RequesterListNode(requester);
    requesterList.addTail(*node);
}


void PVRecord::removeRequester(Requester &requester)
{
    // no need to synchronize because record must be locked when this is called.
    RequesterListNode *node = requesterList.getHead();
    while(node!=0) {
        Requester &xxx = node->getObject();
        if(&xxx==&requester) {
            requesterList.remove(*node);
            delete node;
            return;
        }
        node = requesterList.getNext(*node);
    }
    requester.message(String("PVRecord::removeRequester but not a registered requester"),
        errorMessage);
}

void PVRecord::lock()
{
    recordLock.lock();
    lockRecursiveCount++;
}

void PVRecord::unlock()
{
    lockRecursiveCount--;
    recordLock.unlock();
}

void PVRecord::lockOtherRecord(PVRecord &otherRecord)
{
    int otherId = otherRecord.id;
    if(id<otherId) {
        otherRecord.lock();
        return;
    }
    int count = lockRecursiveCount;
    for(int i=0; i<count; i++) unlock();
    otherRecord.lock();
    for(int i=0; i<count; i++) lock();
}

void PVRecord::beginGroupPut()
{
    if(++depthGroupPut>1) return;
    // no need to synchronize because record must be locked when this is called
    PVListenerListNode *node = listenerList.getHead();
    while(node!=0) {
        PVListener &pvListener = node->getObject();
        pvListener.beginGroupPut(*this);
        node = listenerList.getNext(*node);
    }
}

void PVRecord::endGroupPut()
{
    if(--depthGroupPut>0) return;
    // no need to synchronize because record must be locked when this is called
    PVListenerListNode *node = listenerList.getHead();
    while(node!=0) {
        PVListener &pvListener = node->getObject();
        pvListener.endGroupPut(*this);
        node = listenerList.getNext(*node);
    }
}

void PVRecord::registerListener(PVListener &pvListener)
{
    PVListenerListNode *node = listenerList.getHead();
    while(node!=0) {
        PVListener &listener = node->getObject();
        if(&listener==&pvListener) {
             throw std::logic_error(String(
                 "PVRecord::registerListener but already registered"));
        }
        node = listenerList.getNext(*node);
    }
    node = new PVListenerListNode(pvListener);
    listenerList.addTail(*node);
}

void PVRecord::unregisterListener(PVListener &pvListener)
{
    PVListenerListNode *node = listenerList.getHead();
    while(node!=0) {
        PVListener &listener = node->getObject();
        if(&listener==&pvListener) {
              listenerList.remove(*node);
              delete node;
              break;
        }
        node = listenerList.getNext(*node);
    }
    pvRecordStructure.get()->removeListener(pvListener);
}

bool PVRecord::isRegisteredListener(PVListener &pvListener)
{
    PVListenerListNode *node = listenerList.getHead();
    while(node!=0) {
        PVListener &listener = node->getObject();
        if(&listener==&pvListener) {
            return true;
        }
        node = listenerList.getNext(*node);
    }
    return false;
}

void PVRecord::removeEveryListener()
{
    PVListenerListNode *node = listenerList.removeHead();
    while(node!=0) {
        PVListener &listener = node->getObject();
        listener.unlisten(*this);
        delete node;
        node = listenerList.removeHead();
    }
}

void PVRecord::registerClient(PVRecordClient &pvRecordClient)
{
    PVRecordClientListNode *node = clientList.getHead();
    while(node!=0) {
        PVRecordClient &client = node->getObject();
        if(&client==&pvRecordClient) {
             message(String(
                 "PVRecord::registerListener but already registered"),
                 warningMessage);
             return;
        }
        node = clientList.getNext(*node);
    }
    node = new PVRecordClientListNode(pvRecordClient);
    clientList.addTail(*node);
}

void PVRecord::unregisterClient(PVRecordClient &pvRecordClient)
{
    PVRecordClientListNode *node = clientList.getHead();
    while(node!=0) {
        PVRecordClient &client = node->getObject();
        if(&client==&pvRecordClient) {
             clientList.remove(*node);
             delete node;
             return;
        }
        node = clientList.getNext(*node);
    }
}

void PVRecord::detachClients()
{
    PVRecordClientListNode *node = clientList.removeHead();
    while(node!=0) {
        PVRecordClient &client = node->getObject();
         clientList.remove(*node);
         client.detach(*this);
         delete node;
         node = clientList.removeHead();
    }
}

int PVRecord::getNumberClients()
{
    return clientList.getLength();
}

void PVRecord::toString(StringBuilder buf)
{
    toString(buf,0);
}

void PVRecord::toString(StringBuilder buf,int indentLevel)
{
    convert.newLine(buf,indentLevel);
    *buf += "record " + getRecordName() + " ";
    getPVRecordStructure().getPVStructure().toString(buf,indentLevel);
}

PVRecordField::PVRecordField(PVField &pvField, PVRecordStructurePtr parent,
    PVRecord &pvRecord)
: support(0),
  pvField(pvField),
  pvRecord(pvRecord),
  parent(parent),
  isStructure((pvField.getField()->getType()==structure) ? true : false)
{
    pvField.setPostHandler(this);
}

PVRecordField::~PVRecordField()
{
}

String PVRecordField::getRequesterName()
{
    return pvRecord.getRequesterName();
}

void PVRecordField::message(String message,MessageType messageType)
{
    pvRecord.message(getFullName() + " " + message,messageType);
}

SupportPtr PVRecordField::getSupport()
{
    return support;
}

void PVRecordField::setSupport(Support &sup)
{
    if(support!=0) {
        throw std::logic_error(String(
            "PVRecordField::setSupport support already set"));
    }
    support = &sup;
}

PVRecordStructurePtr PVRecordField::getParent()
{
    return parent;
}

PVField &PVRecordField::getPVField()
{
    return pvField;
}

String PVRecordField::getFullFieldName()
{
    if(fullFieldName.length()<1) createNames();
    return fullFieldName;
}

String PVRecordField::getFullName()
{
    if(fullName.length()<1) createNames();
    return fullName;
}

PVRecord &PVRecordField::getPVRecord()
{
    return pvRecord;
}

void PVRecordField::renameField(String newName)
{
    pvField.renameField(newName);
    createNames();
}

bool PVRecordField::addListener(PVListener &pvListener)
{
    if(!pvRecord.isRegisteredListener(pvListener)) return false;
    PVListenerList &listenerList = listenerList;
    PVListenerListNode *node = listenerList.getHead();
    while(node!=0) {
        PVListener &listener = node->getObject();
        if(&listener==&pvListener) return false;
        node = listenerList.getNext(*node);
    }
    node = new PVListenerListNode(pvListener);
    listenerList.addTail(*node);
    return true;
}

void PVRecordField::removeListener(PVListener &pvListener)
{
    PVListenerListNode *node = listenerList.getHead();
    while(node!=0) {
        PVListener &listener = node->getObject();
        if(&listener==&pvListener) {
              listenerList.remove(*node);
              delete node;
              break;
        }
        node = listenerList.getNext(*node);
    }
    if(pvField.getField()->getType()!=structure) return;
    PVRecordStructure *pvrs = static_cast<PVRecordStructure *>(this);
    int numFields = pvrs->getPVStructure().getStructure()->getNumberFields();
    PVRecordFieldPtrArray pvrfa = pvrs->getPVRecordFields();
    for(int i=0; i<numFields; i++) pvrfa[i]->removeListener(pvListener);
}

void PVRecordField::postPut()
{
    if(parent!=0) parent->postParent(*this);
    postSubField();
}

void PVRecordField::postParent(PVRecordField &subField)
{
    PVRecordStructure &pvrs = static_cast<PVRecordStructure &>(*this);
    PVListenerListNode *node = listenerList.getHead();
    while(node!=0) {
        PVListener &listener = node->getObject();
        listener.dataPut(pvrs,subField);
        node = listenerList.getNext(*node);
    }
    if(parent!=0) parent->postParent(subField);
}

void PVRecordField::postSubField()
{
    callListener();
    bool isStructure = pvField.getField()->getType()==structure;
    if(isStructure) {
        PVRecordStructure *pvrs = static_cast<PVRecordStructure *>(this);
        int numFields = pvrs->getPVStructure().getStructure()->getNumberFields();
        PVRecordFieldPtrArray pvrfa = pvrs->getPVRecordFields();
        for(int i=0; i<numFields; i++) {
            pvrfa[i]->postSubField();
        }
    }
}

void PVRecordField::callListener()
{
    PVListenerListNode *node = listenerList.getHead();
    while(node!=0) {
        PVListener &listener = node->getObject();
        listener.dataPut(*this);
        node = listenerList.getNext(*node);
    }
}

void PVRecordField::createNames()
{
    String name = pvField.getField()->getFieldName();
    while(parent!=0) {
        String fieldName = parent->getPVStructure().getField()->getFieldName();
        if(fieldName.length()<1) break;
        name = fieldName + "." + name;
        parent = parent->getParent();
    }
    fullFieldName = name;
    fullName = pvRecord.getRecordName() + "." + name;
}

PVRecordStructure::PVRecordStructure(PVStructure &pvStructure,
    PVRecordStructure *parent,
    PVRecord &pvRecord)
: PVRecordField(pvStructure,parent,pvRecord),
  pvStructure(pvStructure),
  pvRecordFieldPtrArray(0)
{
    int numberFields = pvStructure.getStructure()->getNumberFields();
    PVRecordField **pvRecordFields = new PVRecordField*[numberFields];
    PVFieldPtrArray pvFields = pvStructure.getPVFields();
    for(int i=0; i<numberFields; i++) {
        PVField *pvField = pvFields[i];
        if(pvField->getField()->getType()==structure) {
            PVStructure *pvs = static_cast<PVStructure *>(pvField);
            pvRecordFields[i] = new PVRecordStructure(
                *pvs,this,pvRecord);
        } else {
           pvRecordFields[i] = new PVRecordField(
                *pvField,this,pvRecord);
        }
    }
    pvRecordFieldPtrArray = pvRecordFields;
}

PVRecordStructure::~PVRecordStructure()
{
    int numberFields = pvStructure.getStructure()->getNumberFields();
    for(int i=0; i<numberFields; i++) {
        delete pvRecordFieldPtrArray[i];
    }
    delete[] pvRecordFieldPtrArray;
}

PVRecordFieldPtrArray PVRecordStructure::getPVRecordFields()
{
    return pvRecordFieldPtrArray;
}

PVStructure &PVRecordStructure::getPVStructure()
{
    return pvStructure;
}

}}
