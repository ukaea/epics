/* powerSupply.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 * @date 2013.04.02
 */

#include <pv/standardField.h>
#include <pv/standardPVField.h>

#define epicsExportSharedSymbols
#include "powerSupply.h"

using namespace epics::pvData;
using std::string;
using std::cout;
using std::cerr;
using std::endl;

namespace epics { namespace pvDatabase { 

PVStructurePtr createPowerSupply()
{
    FieldCreatePtr fieldCreate = getFieldCreate();
    StandardFieldPtr standardField = getStandardField();
    PVDataCreatePtr pvDataCreate = getPVDataCreate();

    return pvDataCreate->createPVStructure(
        fieldCreate->createFieldBuilder()->
            add("alarm",standardField->alarm()) ->
            add("timeStamp",standardField->timeStamp()) ->
            addNestedStructure("power") ->
               add("value",pvDouble) ->
               add("alarm",standardField->alarm()) ->
               endNested()->
            addNestedStructure("voltage") ->
               add("value",pvDouble) ->
               add("alarm",standardField->alarm()) ->
               endNested()->
            addNestedStructure("current") ->
               add("value",pvDouble) ->
               add("alarm",standardField->alarm()) ->
               endNested()->
            createStructure());
}

PowerSupplyPtr PowerSupply::create(
    string const & recordName,
    PVStructurePtr const & pvStructure)
{
    PowerSupplyPtr pvRecord(
        new PowerSupply(recordName,pvStructure));
    if(!pvRecord->init()) pvRecord.reset();
    return pvRecord;
}

PowerSupply::PowerSupply(
    string const & recordName,
    PVStructurePtr const & pvStructure)
: PVRecord(recordName,pvStructure)
{
}

PowerSupply::~PowerSupply()
{
}

void PowerSupply::destroy()
{
    PVRecord::destroy();
}

bool PowerSupply::init()
{
    initPVRecord();
    PVStructurePtr pvStructure = getPVStructure();
    PVFieldPtr pvField;
    bool result;
    pvField = pvStructure->getSubField("timeStamp");
    if(!pvField) {
        cerr << "no timeStamp" << endl;
        return false;
    }
    result = pvTimeStamp.attach(pvField);
    if(!result) {
        cerr << "no timeStamp" << endl;
        return false;
    }
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
        cerr << "no powert\n";
        return false;
    }
    return true;
}

void PowerSupply::process()
{
    timeStamp.getCurrent();
    pvTimeStamp.set(timeStamp);
    double voltage = pvVoltage->get();
    double power = pvPower->get();
    if(voltage<1e-3 && voltage>-1e-3) {
        alarm.setMessage("bad voltage");
        alarm.setSeverity(majorAlarm);
        pvAlarm.set(alarm);
        return;
    }
    double current = power/voltage;
    pvCurrent->put(current);
    alarm.setMessage("");
    alarm.setSeverity(noAlarm);
    pvAlarm.set(alarm);
}

void PowerSupply::put(double power,double voltage)
{
    pvPower->put(power);
    pvVoltage->put(voltage);
}

double PowerSupply::getPower()
{
    return pvPower->get();
}

double PowerSupply::getVoltage()
{
    return pvVoltage->get();
}

double PowerSupply::getCurrent()
{
    return pvCurrent->get();
}


}}
