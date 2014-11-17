/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 */
/* Marty Kraimer 2011.03 */
/* This connects to a DB record and presents the data as a PVStructure
 * It provides access to  value, alarm, display, and control.
 */

#include <cstddef>
#include <cstdlib>
#include <cstddef>
#include <string>
#include <cstdio>
#include <stdexcept>
#include <memory>

#include <dbAccess.h>
#include <dbEvent.h>
#include <dbNotify.h>
#include <dbCommon.h>

#include <pv/pvIntrospect.h>
#include <pv/pvData.h>
#include <pv/pvAccess.h>
#include <pv/standardPVField.h>

#include "dbPv.h"
#include "dbUtil.h"

using namespace epics::pvData;
using namespace epics::pvAccess;
using std::tr1::static_pointer_cast;
using std::string;

namespace epics { namespace pvaSrv { 

DbPvMultiGet::DbPvMultiGet(
    DbPvPtr const &dbPv,
    ChannelGetRequester::shared_pointer const &channelGetRequester,
    DbAddr &dbAddr)
: dbUtil(DbUtil::getDbUtil()),
  dbPv(dbPv),
  channelGetRequester(channelGetRequester),
  dbAddr(dbAddr),
  propertyMask(0),
  process(false),
  firstTime(true),
  beingDestroyed(false)
{
    if(DbPvDebug::getLevel()>0)
        printf("dbPvMultiGet::dbPvMultiGet()\n");
}

DbPvMultiGet::~DbPvMultiGet()
{
    if(DbPvDebug::getLevel()>0)
        printf("dbPvMultiGet::~dbPvMultiGet()\n");
}


bool DbPvMultiGet::init(PVStructurePtr const &pvRequest)
{
    if(DbPvDebug::getLevel()>0)
        printf("dbPvMultiGet::init()\n");
    propertyMask = dbUtil->getProperties(
        channelGetRequester,
        pvRequest,
        dbAddr,
        true);
    if(propertyMask==dbUtil->noAccessBit) return false;
    if(propertyMask==dbUtil->noModBit) {
        channelGetRequester->message("field not allowed to be changed",errorMessage);
        return 0;
    }
    string channelName(dbPv->getChannelName());
    if(channelName.find('.') != string::npos) {
        string message(channelName);
        message += " field of a record not allowed";
        channelGetRequester->message(message,errorMessage);
        return 0;
    }
    if((propertyMask&dbUtil->processBit)!=0) process = true;
    PVStructurePtr pvPutField = pvRequest->getStructureField("getField");
    StringArray const & extraNames = pvPutField->getStructure()->getFieldNames();
    size_t n = extraNames.size() + 1;
    dbAddrArray.resize(n);
    dbAddrArray[0] = dbAddr;
    short fieldType = dbAddr.field_type;
    for(size_t i = 1; i<n; i++) {
        channelName = extraNames[i-1];
        if(channelName.find('.') != string::npos) {
            string message(channelName);
            message += " field of a record not allowed";
            channelGetRequester->message(message,errorMessage);
            return 0;
        }
        if(dbNameToAddr(extraNames[i-1].c_str(),&dbAddrArray[i])!=0) {
            if(DbPvDebug::getLevel()>0) {
                printf("dbNameToAddr failed for %s\n",extraNames[i-1].c_str());
            }
            string message("record not found ");
            message += extraNames[i-1];
            channelGetRequester->message(message,errorMessage);
            return 0;
        }
        if(dbAddrArray[i].field_type!=fieldType) {
            if(DbPvDebug::getLevel()>0) {
                printf("scalarType not the same failed %s\n",extraNames[i-1].c_str());
            }
            string message("scalarType not the same for ");
            message += extraNames[i-1];
            channelGetRequester->message(message,errorMessage);
            return 0;
        }
    }
    ScalarType scalarType(pvBoolean);
    switch(dbAddr.field_type) {
        case DBF_CHAR:
            scalarType = pvByte; break;
        case DBF_UCHAR:
            scalarType = pvUByte; break;
        case DBF_SHORT:
            scalarType = pvShort; break;
        case DBF_USHORT:
            scalarType = pvUShort; break;
        case DBF_LONG:
            scalarType = pvInt; break;
        case DBF_ULONG:
            scalarType = pvUInt; break;
        case DBF_FLOAT:
            scalarType = pvFloat; break;
        case DBF_DOUBLE:
            scalarType = pvDouble; break;
        default:
            channelGetRequester->message("record is not a scalar",errorMessage);
            return 0;
    }
    string prop;
    pvStructure = getStandardPVField()->scalarArray(scalarType,prop);
    pvScalarArray = pvStructure->getScalarArrayField("value",scalarType);
    pvScalarArray->setCapacity(n);
    pvScalarArray->setCapacityMutable(false);
    pvScalarArray->setLength(n);
    int numFields = pvStructure->getNumberFields();
    bitSet.reset(new BitSet(numFields));
    channelGetRequester->channelGetConnect(
       Status::Ok,
       getPtrSelf(),
       pvStructure->getStructure());
    if(DbPvDebug::getLevel()>0)
        printf("dbPvMultiGet::init() returning true\n");
    return true;
}

string DbPvMultiGet::getRequesterName() {
    return channelGetRequester->getRequesterName();
}

void DbPvMultiGet::message(string const &message,MessageType messageType)
{
    channelGetRequester->message(message,messageType);
}

void DbPvMultiGet::destroy() {
    if(DbPvDebug::getLevel()>0) printf("dbPvMultiGet::destroy beingDestroyed %s\n",
         (beingDestroyed ? "true" : "false"));
    {
        Lock xx(mutex);
        if(beingDestroyed) return;
        beingDestroyed = true;
    }
}

void DbPvMultiGet::get()
{
    if(DbPvDebug::getLevel()>0)
        printf("dbPvMultiGet::get()\n");
    Lock lock(dataMutex);
    bool isSameLockSet = true;
    size_t n = dbAddrArray.size();
    struct dbCommon *precord = dbAddr.precord;
    dbScanLock(precord);
    unsigned long lockId = dbLockGetLockId(precord);
    for(size_t i=1; i<n; i++) {
        precord = dbAddrArray[i].precord;
        unsigned long id = dbLockGetLockId(precord);
        if(id!=lockId) {
            isSameLockSet = false;
        }
    }
    if(!isSameLockSet) dbScanUnlock(dbAddr.precord);
    ScalarType scalarType = pvScalarArray->getScalarArray()->getElementType();
    switch(scalarType) {
    case pvByte: {
        shared_vector<int8> xxx(n);
        for(size_t i=0; i<n; i++) {
            precord = dbAddrArray[i].precord;
            if(!isSameLockSet) dbScanLock(precord);
            if(process) {
                if(!precord->disp) dbProcess(precord);
            }
            int8 * val = static_cast<int8 *>(dbAddrArray[i].pfield);
            xxx[i] = *val;
            if(!isSameLockSet) dbScanUnlock(precord);
        }
        shared_vector<const int8> data(freeze(xxx));
        PVByteArrayPtr pv = static_pointer_cast<PVByteArray>(pvScalarArray);
        pv->replace(data);
        break;
    }
    case pvUByte: {
        shared_vector<uint8> xxx(n);
        for(size_t i=0; i<n; i++) {
            precord = dbAddrArray[i].precord;
            if(!isSameLockSet) dbScanLock(precord);
            if(process) {
                if(!precord->disp) dbProcess(precord);
            }
            uint8 * val = static_cast<uint8 *>(dbAddrArray[i].pfield);
            xxx[i] = *val;
            if(!isSameLockSet) dbScanUnlock(precord);
        }
        shared_vector<const uint8> data(freeze(xxx));
        PVUByteArrayPtr pv = static_pointer_cast<PVUByteArray>(pvScalarArray);
        pv->replace(data);
        break;
    }
    case pvShort: {
        shared_vector<int16> xxx(n);
        for(size_t i=0; i<n; i++) {
            precord = dbAddrArray[i].precord;
            if(!isSameLockSet) dbScanLock(precord);
            if(process) {
                if(!precord->disp) dbProcess(precord);
            }
            int16 * val = static_cast<int16 *>(dbAddrArray[i].pfield);
            xxx[i] = *val;
            if(!isSameLockSet) dbScanUnlock(precord);
        }
        shared_vector<const int16> data(freeze(xxx));
        PVShortArrayPtr pv = static_pointer_cast<PVShortArray>(pvScalarArray);
        pv->replace(data);
        break;
    }
    case pvUShort: {
        shared_vector<uint16> xxx(n);
        for(size_t i=0; i<n; i++) {
            precord = dbAddrArray[i].precord;
            if(!isSameLockSet) dbScanLock(precord);
            if(process) {
                if(!precord->disp) dbProcess(precord);
            }
            uint16 * val = static_cast<uint16 *>(dbAddrArray[i].pfield);
            xxx[i] = *val;
            if(!isSameLockSet) dbScanUnlock(precord);
        }
        shared_vector<const uint16> data(freeze(xxx));
        PVUShortArrayPtr pv = static_pointer_cast<PVUShortArray>(pvScalarArray);
        pv->replace(data);
        break;
    }
    case pvInt: {
        shared_vector<int32> xxx(n);
        for(size_t i=0; i<n; i++) {
            precord = dbAddrArray[i].precord;
            if(!isSameLockSet) dbScanLock(precord);
            if(process) {
                if(!precord->disp) dbProcess(precord);
            }
            int32 * val = static_cast<int32 *>(dbAddrArray[i].pfield);
            xxx[i] = *val;
            if(!isSameLockSet) dbScanUnlock(precord);
        }
        shared_vector<const int32> data(freeze(xxx));
        PVIntArrayPtr pv = static_pointer_cast<PVIntArray>(pvScalarArray);
        pv->replace(data);
        break;
    }
    case pvUInt: {
        shared_vector<uint32> xxx(n);
        for(size_t i=0; i<n; i++) {
            precord = dbAddrArray[i].precord;
            if(!isSameLockSet) dbScanLock(precord);
            if(process) {
                if(!precord->disp) dbProcess(precord);
            }
            uint32 * val = static_cast<uint32 *>(dbAddrArray[i].pfield);
            xxx[i] = *val;
            if(!isSameLockSet) dbScanUnlock(precord);
        }
        shared_vector<const uint32> data(freeze(xxx));
        PVUIntArrayPtr pv = static_pointer_cast<PVUIntArray>(pvScalarArray);
        pv->replace(data);
        break;
    }
    case pvFloat: {
        shared_vector<float> xxx(n);
        for(size_t i=0; i<n; i++) {
            precord = dbAddrArray[i].precord;
            if(!isSameLockSet) dbScanLock(precord);
            if(process) {
                if(!precord->disp) dbProcess(precord);
            }
            float * val = static_cast<float *>(dbAddrArray[i].pfield);
            xxx[i] = *val;
            if(!isSameLockSet) dbScanUnlock(precord);
        }
        shared_vector<const float> data(freeze(xxx));
        PVFloatArrayPtr pv = static_pointer_cast<PVFloatArray>(pvScalarArray);
        pv->replace(data);
        break;
    }
    case pvDouble: {
        shared_vector<double> xxx(n);
        for(size_t i=0; i<n; i++) {
            precord = dbAddrArray[i].precord;
            if(!isSameLockSet) dbScanLock(precord);
            if(process) {
                if(!precord->disp) dbProcess(precord);
            }
            double * val = static_cast<double *>(dbAddrArray[i].pfield);
            xxx[i] = *val;
            if(!isSameLockSet) dbScanUnlock(precord);
        }
        shared_vector<const double> data(freeze(xxx));
        PVDoubleArrayPtr pv = static_pointer_cast<PVDoubleArray>(pvScalarArray);
        pv->replace(data);
        break;
    }
    default:
        channelGetRequester->message("Logic Error did not handle scalarType",errorMessage);
    }
    bitSet->clear();
    bitSet->set(0);
    if(isSameLockSet) dbScanUnlock(dbAddr.precord);
    lock.unlock();
    string message("atomic ");
    message += (isSameLockSet ? "true" : "false");
    channelGetRequester->message(message,infoMessage);
    channelGetRequester->getDone(
        Status::Ok,
        getPtrSelf(),
        pvStructure,
        bitSet);
}

void DbPvMultiGet::lock()
{
    dataMutex.lock();
}

void DbPvMultiGet::unlock()
{
    dataMutex.unlock();
}

}}
