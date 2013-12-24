/* pvShareStructureArray.cpp */
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

SharePVStructureArray::SharePVStructureArray(
    PVRecordField &pvRecordField,
    PVStructure *parent,
    PVStructureArray &pvShare)
: PVArray(parent,pvShare.getField()),
  pvRecordField(pvRecordField)
{
}

SharePVStructureArray::~SharePVStructureArray() {}

StructureArrayConstPtr SharePVStructureArray::getStructureArray()
{
    PVStructureArray &pvArray = static_cast<PVStructureArray &>(
        pvRecordField.getPVField());
    return pvArray.getStructureArray();
}

void SharePVStructureArray::lockShare() const
{
    pvRecordField.getPVRecord().lock();
}

void SharePVStructureArray::unlockShare() const
{
    pvRecordField.getPVRecord().unlock();
}

void SharePVStructureArray::message(String message,MessageType messageType)
{
    pvRecordField.message(message,messageType);
}

void SharePVStructureArray::setImmutable()
{
    lockShare();
    try {
        pvRecordField.getPVField().setImmutable();
        unlockShare();
    } catch(...) {
        unlockShare();
        throw;
    }
}

bool SharePVStructureArray::isImmutable()
{
    return pvRecordField.getPVField().isImmutable();
}

void SharePVStructureArray::setCapacityMutable(bool isMutable)
{
    lockShare();
    try {
        PVArray &pvArray = static_cast<PVArray &>(pvRecordField.getPVField());
        pvArray.setCapacityMutable(isMutable);
        unlockShare();
    } catch(...) {
        unlockShare();
        throw;
    }
}

bool SharePVStructureArray::isCapacityMutable()
{
    PVArray &pvArray = static_cast<PVArray &>(pvRecordField.getPVField());
    return pvArray.isCapacityMutable();
}

void SharePVStructureArray::setCapacity(int capacity)
{
    PVArray &pvArray = static_cast<PVArray &>(pvRecordField.getPVField());
    lockShare();
    try {
        pvArray.setCapacity(capacity);
        unlockShare();
    } catch(...) {
        unlockShare();
        throw;
    }
}

void SharePVStructureArray::setLength(int length)
{
    PVArray &pvArray = static_cast<PVArray &>(pvRecordField.getPVField());
    lockShare();
    try {
        pvArray.setLength(length);
        unlockShare();
    } catch(...) {
        unlockShare();
        throw;
    }
}

int SharePVStructureArray::getCapacity() const
{
    PVArray &pvArray = static_cast<PVArray &>(pvRecordField.getPVField());
    return pvArray.getCapacity();
}

int SharePVStructureArray::getLength() const
{
    PVArray &pvArray = static_cast<PVArray &>(pvRecordField.getPVField());
    return pvArray.getLength();
}

int SharePVStructureArray::get(int offset, int length, StructureArrayData *data)
{
    class PVStructureArray &pvArray = static_cast<PVStructureArray &>(
        pvRecordField.getPVField());
    lockShare();
    try {
        return pvArray.get(offset,length,data);
        unlockShare();
    } catch(...) {
        unlockShare();
        throw;
    }
}

int SharePVStructureArray::put(
    int offset,int length, PVStructurePtrArray from, int fromOffset)
{
    class PVStructureArray &pvArray = static_cast<PVStructureArray &>(
        pvRecordField.getPVField());
    lockShare();
    try {
        pvRecordField.getPVRecord().beginGroupPut();
        int len = pvArray.put(offset,length,from,fromOffset);
        pvRecordField.getPVRecord().endGroupPut();
        unlockShare();
        return len;
    } catch(...) {
        unlockShare();
        throw;
    }
}

void SharePVStructureArray::shareData(
    PVStructurePtrArray value,int capacity,int length)
{
    throw std::logic_error("shareData not legal in this context");
}

bool SharePVStructureArray::equals(PVField &pv)
{
    lockShare();
    try {
        return pvRecordField.getPVField().equals(pv);
        unlockShare();
    } catch(...) {
        unlockShare();
        throw;
    }
}

void SharePVStructureArray::serialize(
    ByteBuffer *pbuffer,SerializableControl *pflusher) const
{
   serialize(pbuffer, pflusher, 0, getLength());
}

void SharePVStructureArray::deserialize(
    ByteBuffer *pbuffer,DeserializableControl *pflusher)
{
    class PVStructureArray &pvArray = static_cast<PVStructureArray &>(
        pvRecordField.getPVField());
    lockShare();
    try {
        pvArray.deserialize(pbuffer,pflusher);
        unlockShare();
    } catch(...) {
        unlockShare();
        throw;
    }
}

void SharePVStructureArray::serialize(
   ByteBuffer *pbuffer,SerializableControl *pflusher,int offset,int count) const
{
    class PVStructureArray &pvArray = static_cast<PVStructureArray &>(
        pvRecordField.getPVField());
    lockShare();
    try {
        pvArray.serialize(pbuffer,pflusher,offset,count);
        unlockShare();
    } catch(...) {
        unlockShare();
        throw;
    }
}

}}
