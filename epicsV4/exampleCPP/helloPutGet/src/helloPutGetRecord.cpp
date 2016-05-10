// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution

/* helloPutGet.cpp */
/**
 * @author mrk
 * @date 2013.04.02
 */

#include <pv/standardField.h>

#define epicsExportSharedSymbols
#include <pv/helloPutGetRecord.h>

using namespace epics::pvData;
using namespace epics::pvDatabase;
using std::tr1::static_pointer_cast;
using std::string;

namespace epics { namespace exampleCPP { namespace helloPutGet { 


HelloPutGetRecordPtr HelloPutGetRecord::create(
    string const & recordName)
{
    StandardFieldPtr standardField = getStandardField();
    FieldCreatePtr fieldCreate = getFieldCreate();
    PVDataCreatePtr pvDataCreate = getPVDataCreate();
    StructureConstPtr  topStructure = fieldCreate->createFieldBuilder()->
        add("timeStamp",standardField->timeStamp()) ->
        addNestedStructure("argument")->
            add("value",pvString)->
            endNested()->
        addNestedStructure("result") ->
            add("value",pvString) ->    
            endNested()->
        createStructure();
    PVStructurePtr pvStructure = pvDataCreate->createPVStructure(topStructure);

    HelloPutGetRecordPtr pvRecord(
        new HelloPutGetRecord(recordName,pvStructure));
    if(!pvRecord->init()) pvRecord.reset();
    return pvRecord;
}

HelloPutGetRecord::HelloPutGetRecord(
    string const & recordName,
    PVStructurePtr const & pvStructure)
: PVRecord(recordName,pvStructure)
{
}

HelloPutGetRecord::~HelloPutGetRecord()
{
}

void HelloPutGetRecord::destroy()
{
    PVRecord::destroy();
}

bool HelloPutGetRecord::init()
{
    
    initPVRecord();
    pvArgumentValue = getPVStructure()->getSubField<PVString>("argument.value");
    if(pvArgumentValue.get()==NULL) return false;
    pvResultValue = getPVStructure()->getSubField<PVString>("result.value");
    if(pvResultValue.get()==NULL) return false;
    return true;
}

void HelloPutGetRecord::process()
{
    pvResultValue->put("Hello " + pvArgumentValue->get());
    PVRecord::process();
}

}}}
