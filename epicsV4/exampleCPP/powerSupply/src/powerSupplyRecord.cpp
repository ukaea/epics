/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

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
    pvRecord->initPvt();
    return pvRecord;
}

PowerSupplyRecord::PowerSupplyRecord(
    string const & recordName,
    PVStructurePtr const & pvStructure)
: PVRecord(recordName,pvStructure)
{
}


void PowerSupplyRecord::initPvt()
{
    initPVRecord();
    PVStructurePtr pvStructure = getPVStructure();
    PVFieldPtr pvField;
    pvField = pvStructure->getSubField("alarm");
    pvAlarm.attach(pvField);
    pvCurrent = pvStructure->getSubField<PVDouble>("current.value");
    pvVoltage = pvStructure->getSubField<PVDouble>("voltage.value");
    pvPower = pvStructure->getSubField<PVDouble>("power.value");
    alarm.setMessage("bad voltage");
    alarm.setSeverity(majorAlarm);
    pvAlarm.set(alarm);
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
