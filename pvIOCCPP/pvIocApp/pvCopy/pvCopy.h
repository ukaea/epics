/* pvCopy.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 */
#ifndef PVCOPY_H
#define PVCOPY_H
#include <string>
#include <stdexcept>
#include <memory>

#include <pv/pvType.h>
#include <pv/pvIntrospect.h>
#include <pv/pvData.h>
#include <pv/noDefaultMethods.h>

#include <pv/pvDatabase.h>
#include <pv/support.h>

namespace epics { namespace pvIOC { 

class PVCopy;
class PVCopyMonitor;
class PVCopyMonitorRequester;
template<typename T>
class SharePVScalarArray;
class SharePVStructureArray;


class PVCopy : private epics::pvData::NoDefaultMethods
{
public:
    static std::tr1::shared_ptr<PVCopy> create(
        PVRecord &pvRecord,
        epics::pvData::PVStructure &pvRequest,
        epics::pvData::String structureName);
    ~PVCopy();
    PVRecord &getPVRecord();
    epics::pvData::StructureConstPtr getStructure();
    std::auto_ptr<epics::pvData::PVStructure> createPVStructure();
    int getCopyOffset(PVRecordField &recordPVField);
    int getCopyOffset(
        PVRecordStructure &recordPVStructure,
        PVRecordField &recordPVField);
    PVRecordField &getRecordPVField(int structureOffset);
    void initCopy(
        epics::pvData::PVStructure &copyPVStructure,
        epics::pvData::BitSet &bitSet,
        bool lockRecord);
    void updateCopySetBitSet(
        epics::pvData::PVStructure &copyPVStructure,
        epics::pvData::BitSet &bitSet,
        bool lockRecord);
    void updateCopyFromBitSet(
        epics::pvData::PVStructure &copyPVStructure,
        epics::pvData::BitSet &bitSet,
        bool lockRecord);
    void updateRecord(
        epics::pvData::PVStructure &copyPVStructure,
        epics::pvData::BitSet &bitSet,
        bool lockRecord);
    PVCopyMonitor &createPVCopyMonitor(
        PVCopyMonitorRequester &pvCopyMonitorRequester);
private:
    PVRecord &pvRecord;
    epics::pvData::StructureConstPtr structure;
    class Node *headNode;
    std::auto_ptr<epics::pvData::PVStructure> cacheInitStructure;
private:
    PVCopy(PVRecord &pvRecord);
    void init(epics::pvData::PVStructure &pvRequest);
    epics::pvData::StructureConstPtr createStructure(
        epics::pvData::PVStructure &pvRecord,
        epics::pvData::PVStructure &pvFromRequest,
        epics::pvData::String fieldName);
    void createStructureNodes(
        PVRecord &pvRecord,
        class StructureNode &structureNode,
        PVRecordStructure &pvRecordStructure,
        epics::pvData::PVStructure &pvFromRequest,
        epics::pvData::PVStructure &pvFromStructure);
    void referenceImmutable(
        epics::pvData::PVField &pvField,
        class Node &node);
    void referenceImmutable(
        epics::pvData::PVField &copyPVField,
        PVRecordField &recordPVField);
    void makeShared(
        epics::pvData::PVField &copyPVField,
        PVRecordField &recordPVField);
    void updateStructureNodeSetBitSet(
        epics::pvData::PVStructure &pvCopy,
        class StructureNode &structureNode,
        epics::pvData::BitSet &bitSet);
    void updateSubFieldSetBitSet(
        epics::pvData::PVField &pvCopy,
        PVRecordField &pvRecord,
        epics::pvData::BitSet &bitSet);
    void updateStructureNodeFromBitSet(
        epics::pvData::PVStructure &pvCopy,
        class StructureNode &structureNode,
        epics::pvData::BitSet &bitSet,
        bool toCopy,
        bool doAll);
    void updateSubFieldFromBitSet(
        epics::pvData::PVField &pvCopy,
        PVRecordField &pvRecordField,
        epics::pvData::BitSet &bitSet,
        bool toCopy,
        bool doAll);
    class RecordNode *getCopyOffset(
        class StructureNode &structureNode,
        PVRecordField &recordPVField);
    class RecordNode *getRecordNode(
        class StructureNode &structureNode,
        int structureOffset);
    
};

class PVCopyMonitor : private epics::pvData::NoDefaultMethods
{
public:
    ~PVCopyMonitor();
    void startMonitoring(
        epics::pvData::BitSet &changeBitSet,
        epics::pvData::BitSet &overrunBitSet);
    void stopMonitoring();
    void switchBitSets(
        epics::pvData::BitSet &newChangeBitSet,
        epics::pvData::BitSet &newOverrunBitSet, bool lockRecord);
private:
    PVCopyMonitor();
    friend class PVCopy;
    // TBD
};

class PVCopyMonitorRequester {
public:
    virtual void dataChanged() = 0;
    virtual void unlisten() = 0;
};


template<typename T>
class SharePVScalarArray :
    public epics::pvData::PVValueArray<T>
{
public:
    typedef T  value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef epics::pvData::PVArrayData<T> ArrayDataType;

    SharePVScalarArray(
        PVRecordField &pvRecordField,
        epics::pvData::PVStructure *parent,
        epics::pvData::PVScalarArray &pvShare);
    virtual ~SharePVScalarArray();
    void lockShare() const;
    void unlockShare() const;
    virtual void message(
        epics::pvData::String message,
        epics::pvData::MessageType messageType);
    virtual void setImmutable();
    bool isImmutable();
    virtual void setCapacity(int capacity);
    void setLength(int length);
    int getCapacity() const;
    int getLength() const;
    bool isCapacityMutable();
    void setCapacityMutable(bool isMutable);
    virtual int get(int offset, int length, ArrayDataType *data) = 0;
    virtual int put(int offset,int length, pointer from, int fromOffset) = 0;
    virtual void shareData(pointer value,int capacity,int length) = 0;
    virtual bool equals(epics::pvData::PVField &pv);
    virtual void serialize(
        epics::pvData::ByteBuffer *pbuffer,
        epics::pvData::SerializableControl *pflusher) const;
    virtual void deserialize(
        epics::pvData::ByteBuffer *pbuffer,
        epics::pvData::DeserializableControl *pflusher);
    virtual void serialize(
        epics::pvData::ByteBuffer *pbuffer,
        epics::pvData::SerializableControl *pflusher, int offset, int count) const;
private:
    PVRecordField &pvRecordField;
};

typedef SharePVScalarArray<bool> SharePVBooleanArray;
typedef SharePVScalarArray<epics::pvData::int8> SharePVByteArray;
typedef SharePVScalarArray<epics::pvData::int16> SharePVShortArray;
typedef SharePVScalarArray<epics::pvData::int32> SharePVIntArray;
typedef SharePVScalarArray<epics::pvData::int64> SharePVLongArray;
typedef SharePVScalarArray<float> SharePVFloatArray;
typedef SharePVScalarArray<double> SharePVDoubleArray;
typedef SharePVScalarArray<epics::pvData::String> SharePVStringArray;


class SharePVStructureArray :
    public epics::pvData::PVStructureArray
{
public:
    SharePVStructureArray(
        PVRecordField &pvRecordField,
        epics::pvData::PVStructure *parent,
        epics::pvData::PVStructureArray &pvShare);
    virtual ~SharePVStructureArray();
    virtual epics::pvData::StructureArrayConstPtr getStructureArray();
    void lockShare() const;
    void unlockShare() const;
    virtual void message(
        epics::pvData::String message,
        epics::pvData::MessageType messageType);
    virtual void setImmutable();
    bool isImmutable();
    virtual void setCapacity(int capacity);
    void setLength(int length);
    int getCapacity() const;
    int getLength() const;
    bool isCapacityMutable();
    void setCapacityMutable(bool isMutable);
    virtual void shareData(
        epics::pvData::PVStructurePtrArray value,
        int capacity,int length);
    virtual int get(int offset, int length,
        epics::pvData::StructureArrayData *data);
    virtual int put(int offset,int length,
        epics::pvData::PVStructurePtrArray from,
        int fromOffset);
    virtual bool equals(epics::pvData::PVField &pv);
    virtual void serialize(
        epics::pvData::ByteBuffer *pbuffer,
        epics::pvData::SerializableControl *pflusher) const;
    virtual void deserialize(
        epics::pvData::ByteBuffer *pbuffer,
        epics::pvData::DeserializableControl *pflusher);
    virtual void serialize(
        epics::pvData::ByteBuffer *pbuffer,
        epics::pvData::SerializableControl *pflusher,
        int offset, int count) const;
private:
    PVRecordField &pvRecordField;
};


}}

#endif  /* PVCOPY_H */
