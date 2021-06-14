/* addRecord.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 * @date 2013.04.18
 */

#include <string>
#include <cstring>
#include <stdexcept>
#include <memory>
#include <set>

#include <pv/lock.h>
#include <pv/pvType.h>
#include <pv/pvData.h>
#include <pv/standardField.h>
#include <pv/pvTimeStamp.h>
#include <pv/timeStamp.h>
#include <pv/rpcService.h>
#include <pv/pvAccess.h>
#include <pv/status.h>
#include <pv/serverContext.h>


#define epicsExportSharedSymbols
#include "pv/pvStructureCopy.h"
#include "pv/pvDatabase.h"
#include "pv/addRecord.h"

using std::tr1::static_pointer_cast;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace std;

namespace epics { namespace pvDatabase { 

AddRecordPtr AddRecord::create(
    std::string const & recordName)
{
    FieldCreatePtr fieldCreate = getFieldCreate();
    PVDataCreatePtr pvDataCreate = getPVDataCreate();
    StructureConstPtr  topStructure = fieldCreate->createFieldBuilder()->
        addNestedStructure("argument")->
            add("recordName",pvString)->
            addNestedUnion("union") ->
                endNested()->
            endNested()->
        addNestedStructure("result") ->
            add("status",pvString) ->
            endNested()->
        createStructure();
    PVStructurePtr pvStructure = pvDataCreate->createPVStructure(topStructure);
    AddRecordPtr pvRecord(
        new AddRecord(recordName,pvStructure));
    if(!pvRecord->init()) pvRecord.reset();
    return pvRecord;
}

AddRecord::AddRecord(
    std::string const & recordName,
    epics::pvData::PVStructurePtr const & pvStructure)
: PVRecord(recordName,pvStructure)
{
}

bool AddRecord::init()
{
    initPVRecord();
    PVStructurePtr pvStructure = getPVStructure();
    pvRecordName = pvStructure->getSubField<PVString>("argument.recordName");
    if(!pvRecordName) return false;
    pvResult = pvStructure->getSubField<PVString>("result.status");
    if(!pvResult) return false;
    return true;
}

void AddRecord::process()
{
    PVDataCreatePtr pvDataCreate = getPVDataCreate();
    string name = pvRecordName->get();
    PVRecordPtr pvRecord = PVDatabase::getMaster()->findRecord(name);
    if(pvRecord) {
        pvResult->put(name + " already exists");
        return;
    }
    PVUnionPtr pvUnion = getPVStructure()->getSubField<PVUnion>("argument.union");
    if(!pvUnion) {
        pvResult->put(name + " argument.union is NULL");
        return;
    }
    PVFieldPtr pvField(pvUnion->get());
    if(!pvField) {
        pvResult->put(name + " union has no value");
        return;
    }
    if(pvField->getField()->getType()!=epics::pvData::structure) {
        pvResult->put(name + " union most be a structure");
        return;
    }
    StructureConstPtr st = static_pointer_cast<const Structure>(pvField->getField());
    PVStructurePtr pvStructure = pvDataCreate->createPVStructure(st);
    PVRecordPtr pvRec = PVRecord::create(name,pvStructure);
    bool result = PVDatabase::getMaster()->addRecord(pvRec);
    if(result) {
        pvResult->put("success");
    } else {
        pvResult->put("failure");
    }
}


}}

