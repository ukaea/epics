/* pvShare.cpp */
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

template<typename T>
SharePVScalarArray<T>::SharePVScalarArray(
        PVRecordField &pvRecordField,
        epics::pvData::PVStructure *parent,
        epics::pvData::PVScalarArray &pvShare)
: PVField(parent,pvShare.getField()),
  pvRecordField(pvRecordField)
{
}

template<typename T>
SharePVScalarArray<T>::~SharePVScalarArray()
{
}

template<typename T>
void SharePVScalarArray<T>::lockShare() const
{
    pvRecordField.getPVRecord().lock();
}

template<typename T>
void SharePVScalarArray<T>::unlockShare() const
{
    pvRecordField.getPVRecord().unlock();
}

template<typename T>
void SharePVScalarArray<T>::message(String message,MessageType messageType)
{
    pvRecordField.message(message,messageType);
}

template<typename T>
void SharePVScalarArray<T>::setImmutable()
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

template<typename T>
bool SharePVScalarArray<T>::isImmutable()
{
    return pvRecordField.getPVField().isImmutable();
}

template<typename T>
void SharePVScalarArray<T>::setCapacityMutable(bool isMutable)
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

template<typename T>
bool SharePVScalarArray<T>::isCapacityMutable()
{
    PVArray &pvArray = static_cast<PVArray &>(pvRecordField.getPVField());
    return pvArray.isCapacityMutable();
}

template<typename T>
void SharePVScalarArray<T>::setCapacity(int capacity)
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

template<typename T>
void SharePVScalarArray<T>::setLength(int length)
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

template<typename T>
int SharePVScalarArray<T>::getCapacity() const
{
    PVArray &pvArray = static_cast<PVArray &>(pvRecordField.getPVField());
    return pvArray.getCapacity();
}

template<typename T>
int SharePVScalarArray<T>::getLength() const
{
    PVArray &pvArray = static_cast<PVArray &>(pvRecordField.getPVField());
    return pvArray.getLength();
}

template<typename T>
int SharePVScalarArray<T>::get(int offset, int length, ArrayDataType *data)
{
    class PVValueArray<T> &pvArray = static_cast<PVValueArray<T> &>(
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

template<typename T>
int SharePVScalarArray<T>::put(
    int offset,int length, pointer from, int fromOffset)
{
    class PVValueArray<T> &pvArray = static_cast<PVValueArray<T> &>(
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

template<typename T>
void SharePVScalarArray<T>::shareData(pointer value,int capacity,int length)
{
    class PVValueArray<T> &pvArray = static_cast<PVValueArray<T> &>(
        pvRecordField.getPVField());
    lockShare();
    try {
        pvArray.setImmutable(value,capacity,length);
        unlockShare();
    } catch(...) {
        unlockShare();
        throw;
    }
}

template<typename T>
bool SharePVScalarArray<T>::equals(PVField &pv)
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

template<typename T>
void SharePVScalarArray<T>::serialize(
    ByteBuffer *pbuffer,SerializableControl *pflusher) const
{
    serialize(pbuffer, pflusher, 0, getLength());
}

template<typename T>
void SharePVScalarArray<T>::deserialize(
    ByteBuffer *pbuffer,DeserializableControl *pflusher)
{
    class PVValueArray<T> &pvArray = static_cast<PVValueArray<T> &>(
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

template<typename T>
void SharePVScalarArray<T>::serialize(
   ByteBuffer *pbuffer,SerializableControl *pflusher,int offset,int count) const
{
    class PVValueArray<T> &pvArray = static_cast<PVValueArray<T> &>(
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
