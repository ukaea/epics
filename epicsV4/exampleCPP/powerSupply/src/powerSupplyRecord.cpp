// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution

/* powerSupply.cpp */

/**
 * @author mrk
 * @date 2013.04.02
 */

#include <pv/standardField.h>
#include <pv/standardPVField.h>

#define epicsExportSharedSymbols
#include <pv/powerSupplyRecord.h>

using namespace epics::pvData;
using namespace epics::pvDatabase;
using std::string;
using std::cout;
using std::cerr;
using std::endl;

namespace epics { namespace exampleCPP {namespace powerSupply { 

PowerSupplyRecordPtr PowerSupplyRecord::create(
    string const & recordName)
{
    FieldCreatePtr fieldCreate = getFieldCreate();
    StandardFieldPtr standardField = getStandardField();
    PVDataCreatePtr pvDataCreate = getPVDataCreate();

    StructureConstPtr  topStructure = fieldCreate->createFieldBuilder()->
            add("alarm",standardField->alarm()) ->
            add("timeStamp",standardField->timeStamp()) ->
            addNestedStructure("power") ->
               add("value",pvDouble) ->
               endNested()->
            addNestedStructure("voltage") ->
               add("value",pvDouble) ->
               endNested()->
            addNestedStructure("current") ->
               add("value",pvDouble) ->
               endNested()->
            createStructure();
    PVStructurePtr pvStructure = pvDataCreate->createPVStructure(topStructure);
    PowerSupplyRecordPtr pvRecord(
        new PowerSupplyRecord(recordName,pvStructure));
    if(!pvRecord->init()) pvRecord.reset();
    return pvRecord;
}

PowerSupplyRecord::PowerSupplyRecord(
    string const & recordName,
    PVStructurePtr const & pvStructure)
: PVRecord(recordName,pvStructure)
{
}

PowerSupplyRecord::~PowerSupplyRecord()
{
}

void PowerSupplyRecord::destroy()
{
    PVRecord::destroy();
}

bool PowerSupplyRecord::init()
{
    initPVRecord();
    PVStructurePtr pvStructure = getPVStructure();
    PVFieldPtr pvField;
    bool result;
    pvField = pvStructure->getSubField("alarm");
    if(!pvField) {
        cerr << "no alarm" << endl;
        return false;
    }
    result = pvAlarm.attach(pvField);
    if(!result) {
        cerr << "no alarm" << endl;
        return false;
    }
    pvCurrent = pvStructure->getSubField<PVDouble>("current.value");
    if(!pvCurrent) {
        cerr << "no current\n";
        return false;
    }
    pvVoltage = pvStructure->getSubField<PVDouble>("voltage.value");
    if(!pvVoltage) {
        cerr << "no current\n";
        return false;
    }
    pvPower = pvStructure->getSubField<PVDouble>("power.value");
    if(!pvPower) {
        cerr << "no power\n";
        return false;
    }
    alarm.setMessage("bad voltage");
    alarm.setSeverity(majorAlarm);
    pvAlarm.set(alarm);
    return true;
}

void PowerSupplyRecord::process()
{
    double voltage = pvVoltage->get();
    double power = pvPower->get();
    if(voltage<1e-3 && voltage>-1e-3) {
        alarm.setMessage("bad voltage");
        alarm.setSeverity(majorAlarm);
        pvAlarm.set(alarm);
        throw std::runtime_error("bad voltage exception");
    }
    double current = power/voltage;
    pvCurrent->put(current);
    pvAlarm.get(alarm);
    if(alarm.getSeverity()!=noAlarm) {
        alarm.setMessage("");
        alarm.setSeverity(noAlarm);
        pvAlarm.set(alarm);
    }
    PVRecord::process();
}

}}}
