/* pvCopy.cpp */
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

#include <pv/pvData.h>
#include <pv/noDefaultMethods.h>

#include <pv/pvDatabase.h>
#include <pv/support.h>
#include <pv/pvCopy.h>

namespace epics { namespace pvIOC { 

using namespace epics::pvData;

struct Node {
    Node()
    : isStructure(false),
      structureOffset(0),
      nfields(0),
      shareData(false)
    {}
    bool isStructure;
    int structureOffset; // In the copy
    int nfields;
    bool shareData;
};
    
struct RecordNode : public Node{
    RecordNode(PVRecordField &pvRecordField)
    : recordPVField(pvRecordField)
    {}
    PVRecordField &recordPVField;
};
    
struct StructureNode : public  Node {
    StructureNode(int NumNodes)
    : numNodes(numNodes),
      nodes(new Node*[numNodes])
    {
        for(int i=0; i<numNodes; i++) nodes[i] = 0;
    }
    ~StructureNode()
    {
        for(int i=0; i<numNodes; i++) delete nodes[i];
        delete[] nodes;
    }
    int numNodes;
    Node** nodes;
};

std::tr1::shared_ptr<PVCopy> PVCopy::create(
    PVRecord &pvRecord, PVStructure &request, String structureName)
{
    PVStructure *pvRequest = &request;
    if(structureName.length()>0) {
        if(pvRequest->getStructure()->getNumberFields()>0) {
            PVStructure *pvStructure
                 = pvRequest->getStructureField(structureName);
            if(pvStructure==0) return std::tr1::shared_ptr<PVCopy>();
            pvRequest = pvStructure;
        }
    }
    std::tr1::shared_ptr<PVCopy> pvCopy =
        std::tr1::shared_ptr<PVCopy>(new PVCopy(pvRecord));
    PVStructure *pvStruct = pvRequest;
    if(pvRequest->getSubField("field")!=0) {
        pvStruct = pvRequest->getStructureField("field");
    }
    pvCopy->init(*pvStruct);
    return pvCopy;
}

PVCopy::PVCopy(
   PVRecord &pvRecord)
: pvRecord(pvRecord),
  structure(StructureConstPtr()),
  headNode(0),
  cacheInitStructure(std::auto_ptr<PVStructure>())
{
}

PVCopy::~PVCopy()
{
    delete headNode;
}

PVRecord &PVCopy::getPVRecord()
{
    return pvRecord;
}

StructureConstPtr PVCopy::getStructure()
{
    return structure;
}

std::auto_ptr<PVStructure> PVCopy::createPVStructure()
{
    if(cacheInitStructure.get()!=0) return cacheInitStructure;
    std::auto_ptr<PVStructure> pvStructure = std::auto_ptr<PVStructure>(
        getPVDataCreate()->createPVStructure(0,structure));
    if(headNode!=0) referenceImmutable(*pvStructure.get(),*headNode);
    return pvStructure;
}

int PVCopy::getCopyOffset(PVRecordField &recordPVField)
{
    if(!headNode->isStructure) {
        RecordNode *recordNode = static_cast<RecordNode*>(headNode);
        if(&(recordNode->recordPVField)==&recordPVField) {
             return headNode->structureOffset;
        }
        return -1;
    }
    StructureNode *node = static_cast<StructureNode*>(headNode);
    RecordNode *recordNode = getCopyOffset(*node,recordPVField);
    if(recordNode!=0) return recordNode->structureOffset;
    return -1;
}

int PVCopy::getCopyOffset(
    PVRecordStructure &recordPVStructure,PVRecordField &recordPVField)
{
    RecordNode *recordNode = 0;
    if(!headNode->isStructure) {
        RecordNode *recordNode = static_cast<RecordNode*>(headNode);
        if(&(recordNode->recordPVField)!=&recordPVStructure) return -1;
    } else {
        StructureNode *node = static_cast<StructureNode*>(headNode);
        recordNode = getCopyOffset(*node,recordPVStructure);
    }
    if(recordNode==0) return -1;
    int diff = recordPVField.getPVField().getFieldOffset()
        - recordPVStructure.getPVStructure().getFieldOffset();
    return recordNode->structureOffset + diff;
}

PVRecordField &PVCopy::getRecordPVField(int structureOffset)
{
    RecordNode *recordNode = 0;
    if(!headNode->isStructure) {
        recordNode = static_cast<RecordNode*>(headNode);
    } else {
        StructureNode *node = static_cast<StructureNode*>(headNode);
        recordNode = getRecordNode(*node,structureOffset);
    }
    if(recordNode==0) {
        throw std::invalid_argument("structureOffset not valid");
    }
    int diff = structureOffset - recordNode->structureOffset;
    PVRecordField &pvRecordField = recordNode->recordPVField;
    if(diff==0) return pvRecordField;
    PVStructure &pvStructure
        = static_cast<PVStructure &>(pvRecordField.getPVField());
    PVField *pvField = pvStructure.getSubField(
        pvRecordField.getPVField().getFieldOffset() + diff);
    return pvRecord.findPVRecordField(*pvField);
}

void PVCopy::initCopy(
    PVStructure &copyPVStructure, BitSet &bitSet, bool lockRecord)
{
    bitSet.clear();
    bitSet.set(0);
    updateCopyFromBitSet(copyPVStructure,bitSet,lockRecord);
}

void PVCopy::updateCopySetBitSet(
    PVStructure &copyPVStructure,BitSet &bitSet,bool lockRecord)
{
    if(lockRecord) pvRecord.lock();
    try {
        if(headNode->isStructure) {
            StructureNode *node = static_cast<StructureNode*>(headNode);
            updateStructureNodeSetBitSet(copyPVStructure,*node,bitSet);
        } else {
            RecordNode *node = static_cast<RecordNode*>(headNode);
            updateSubFieldSetBitSet(copyPVStructure,node->recordPVField,bitSet);
        }
        if(lockRecord) pvRecord.unlock();
    } catch(...) {
        if(lockRecord) pvRecord.unlock();
        throw;
    }
}

void PVCopy::updateCopyFromBitSet(
    PVStructure &copyPVStructure,BitSet &bitSet,bool lockRecord)
{
    bool doAll = bitSet.get(0);
    if(lockRecord) pvRecord.lock();
    try {
        if(headNode->isStructure) {
            StructureNode *node = static_cast<StructureNode*>(headNode);
            updateStructureNodeFromBitSet(
                copyPVStructure,*node,bitSet,true,doAll);
        } else {
            RecordNode *node = static_cast<RecordNode*>(headNode);
            updateSubFieldFromBitSet(
                copyPVStructure,node->recordPVField,bitSet, true,doAll);
        }
        if(lockRecord) pvRecord.unlock();
    } catch(...) {
        if(lockRecord) pvRecord.unlock();
        throw;
    }
}

void PVCopy::updateRecord(
    PVStructure &copyPVStructure,BitSet &bitSet,bool lockRecord)
{
    bool doAll = bitSet.get(0);
    if(lockRecord) pvRecord.lock();
    try {
        pvRecord.beginGroupPut();
        if(headNode->isStructure) {
            StructureNode *node = static_cast<StructureNode*>(headNode);
            updateStructureNodeFromBitSet(
                copyPVStructure,*node,bitSet, false,doAll);
        } else {
            RecordNode *node = static_cast<RecordNode*>(headNode);
            updateSubFieldFromBitSet(
                copyPVStructure,node->recordPVField,bitSet, false,doAll);
        }
        pvRecord.endGroupPut();
        if(lockRecord) pvRecord.unlock();
    } catch(...) {
        if(lockRecord) pvRecord.unlock();
        throw;
    }
}

PVCopyMonitor &PVCopy::createPVCopyMonitor(
    PVCopyMonitorRequester &pvCopyMonitorRequester)
{
    throw std::logic_error(String("Not Implemented"));
}

void PVCopy::init(epics::pvData::PVStructure &pvRequest)
{
}

StructureConstPtr PVCopy::createStructure(
    PVStructure &pvRecord,
    PVStructure &pvFromRequest,
    String fieldName)
{
    throw std::logic_error(String("Not Implemented"));
}

void PVCopy::createStructureNodes(
    PVRecord &pvRecord,
    class StructureNode &structureNode,
    PVRecordStructure &pvRecordStructure,
    PVStructure &pvFromRequest,
    PVStructure &pvFromStructure)
{
    throw std::logic_error(String("Not Implemented"));
}

void PVCopy::referenceImmutable(
    PVField &pvField,
    class Node &node)
{
    throw std::logic_error(String("Not Implemented"));
}

void PVCopy::referenceImmutable(
    PVField &copyPVField,
    PVRecordField &recordPVField)
{
    throw std::logic_error(String("Not Implemented"));
}

void PVCopy::makeShared(
    PVField &copyPVField,
    PVRecordField &recordPVField)
{
    throw std::logic_error(String("Not Implemented"));
}

void PVCopy::updateStructureNodeSetBitSet(
    PVStructure &pvCopy,
    class StructureNode &structureNode,
    BitSet &bitSet)
{
    throw std::logic_error(String("Not Implemented"));
}

void PVCopy::updateSubFieldSetBitSet(
    PVField &pvCopy,
    PVRecordField &pvRecord,
    BitSet &bitSet)
{
    throw std::logic_error(String("Not Implemented"));
}

void PVCopy::updateStructureNodeFromBitSet(
    PVStructure &pvCopy,
    StructureNode &structureNode,
    BitSet &bitSet,
    bool toCopy,
    bool doAll)
{
    throw std::logic_error(String("Not Implemented"));
}

void PVCopy::updateSubFieldFromBitSet(
    PVField &pvCopy,
    PVRecordField &pvRecordField,
    BitSet &bitSet,
    bool toCopy,
    bool doAll)
{
    throw std::logic_error(String("Not Implemented"));
}

RecordNode *PVCopy::getCopyOffset(
    class StructureNode &structureNode,
    PVRecordField &recordPVField)
{
    throw std::logic_error(String("Not Implemented"));
}

RecordNode *PVCopy::getRecordNode(
    class StructureNode &structureNode,
    int structureOffset)
{
    throw std::logic_error(String("Not Implemented"));
}


PVCopyMonitor::PVCopyMonitor()
{
    throw std::logic_error(String("Not Implemented"));
}

PVCopyMonitor::~PVCopyMonitor()
{
    throw std::logic_error(String("Not Implemented"));
}

void PVCopyMonitor::startMonitoring(
    BitSet &changeBitSet, BitSet &overrunBitSet)
{
    throw std::logic_error(String("Not Implemented"));
}

void PVCopyMonitor::stopMonitoring()
{
    throw std::logic_error(String("Not Implemented"));
}

void PVCopyMonitor::switchBitSets(
    BitSet &newChangeBitSet,BitSet &newOverrunBitSet, bool lockRecord)
{
    throw std::logic_error(String("Not Implemented"));
}


}}
