/* ntfield.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
#ifndef NTFIELD_H
#define NTFIELD_H

#include <cstdarg>

#ifdef epicsExportSharedSymbols
#   define ntfieldEpicsExportSharedSymbols
#   undef epicsExportSharedSymbols
#endif

#include <pv/standardField.h>
#include <pv/standardPVField.h>
#include <pv/alarm.h>
#include <pv/pvAlarm.h>
#include <pv/timeStamp.h>
#include <pv/pvTimeStamp.h>
#include <pv/sharedVector.h>

#ifdef ntfieldEpicsExportSharedSymbols
#   define epicsExportSharedSymbols
#	undef ntfieldEpicsExportSharedSymbols
#endif

#include <shareLib.h>

namespace epics { namespace nt {
/**
 * Convenience Class for introspection fields of a Normative Type
 * @author mrk
 * 
 */

class NTField;
typedef std::tr1::shared_ptr<NTField> NTFieldPtr;

class PVNTField;
typedef std::tr1::shared_ptr<PVNTField> PVNTFieldPtr;

class epicsShareClass NTField {
public:
    POINTER_DEFINITIONS(NTField);
    /**
     * get the single implementation of this class.
     * @return the implementation
     */
    static NTFieldPtr get();
    /**
     * destructor
     */
    ~NTField() {}
    /**
     * Is field an enumerated structure.
     * @param field The field to test.
     * @return (false,true) if field (is not,is) an enumerated structure.
     */
    bool isEnumerated(epics::pvData::FieldConstPtr const & field);
    /**
     * Is field a timeStamp structure.
     * @param field The field to test.
     * @return (false,true) if field (is not,is) a timeStamp structure.
     */
    bool isTimeStamp(epics::pvData::FieldConstPtr const & field);
    /**
     * Is field an alarm structure.
     * @param field The field to test.
     * @return (false,true) if field (is not,is) an alarm structure.
     */
    bool isAlarm(epics::pvData::FieldConstPtr const & field);
    /**
     * Is field a display structure.
     * @param field The field to test.
     * @return (false,true) if field (is not,is) a display structure.
     */
    bool isDisplay(epics::pvData::FieldConstPtr const & field);
    /**
     * Is field an alarmLimit structure.
     * @param field The field to test.
     * @return (false,true) if field (is not,is) an alarmLimit structure.
     */
    bool isAlarmLimit(epics::pvData::FieldConstPtr const & field);
    /**
     * Is field a control structure.
     * @param field The field to test.
     * @return (false,true) if field (is not,is) a control structure.
     */
    bool isControl(epics::pvData::FieldConstPtr const & field);

    /**
     * Create an enumerated structure.
     * @return an enumerated structure.
     */
    epics::pvData::StructureConstPtr createEnumerated();
    /**
     * Create a timeStamp structure.
     * @return a timeStamp structure.
     */
    epics::pvData::StructureConstPtr createTimeStamp();
    /**
     * Create an alarm structure.
     * @return an alarm structure.
     */
    epics::pvData::StructureConstPtr createAlarm();
    /**
     * Create a display structure.
     * @return a displayalarm structure.
     */
    epics::pvData::StructureConstPtr createDisplay();
    /**
     * Create a control structure.
     * @return a control structure.
     */
    epics::pvData::StructureConstPtr createControl();

    /**
     * Create an array of enumerated structures.
     * @return an array of enumerated structures.
     */
    epics::pvData::StructureArrayConstPtr createEnumeratedArray();
    /**
     * Create an array of timeStamp structures.
     * @return an array of timeStamp structures.
     */
    epics::pvData::StructureArrayConstPtr createTimeStampArray();
    /**
     * Create an array of alarm structures.
     * @return an array of alarm structures.
     */
    epics::pvData::StructureArrayConstPtr createAlarmArray();
private:
    NTField();
    epics::pvData::FieldCreatePtr fieldCreate;
    epics::pvData::StandardFieldPtr standardField;
};

/**
 * Convenience Class for data fields of a Normative Type
 * @author mrk
 * 
 */
class epicsShareClass PVNTField {
public:
    POINTER_DEFINITIONS(PVNTField);
    /**
     * get the single implementation of this class.
     * @return the implementation
     */
    static PVNTFieldPtr get();
    /**
     * destructor
     */
    ~PVNTField() {}
    /**
     * Create an enumerated PVStructure.
     * @param choices The array of choices.
     * @return an enumerated PVStructure..
     */
    epics::pvData::PVStructurePtr createEnumerated(
        epics::pvData::StringArray const & choices);
    /**
     * Create a timeStamp PVStructure.
     * @return a timeStamp PVStructure..
     */
    epics::pvData::PVStructurePtr createTimeStamp();
    /**
     * Create an alarm PVStructure.
     * @return an alarm PVStructure..
     */
    epics::pvData::PVStructurePtr createAlarm();
    /**
     * Create a display PVStructure.
     * @return a display PVStructure..
     */
    epics::pvData::PVStructurePtr createDisplay();
    /**
     * Create an alarmLimit PVStructure.
     * @return an alarmLimit PVStructure..
     */
    epics::pvData::PVStructurePtr createAlarmLimit();
    /**
     * Create a control PVStructure.
     * @return a control PVStructure..
     */
    epics::pvData::PVStructurePtr createControl();

    /**
     * Create an enumerated PVStructureArray.
     * @return an enumerated PVStructureArray..
     */
    epics::pvData::PVStructureArrayPtr createEnumeratedArray();
    /**
     * Create a timeStamp PVStructureArray.
     * @return a timeStamp PVStructureArray
     */
    epics::pvData::PVStructureArrayPtr createTimeStampArray();
    /**
     * Create an alarm PVStructureArray.
     * @return an alarm PVStructureArray..
     */
    epics::pvData::PVStructureArrayPtr createAlarmArray();
private:
    PVNTField();
    epics::pvData::PVDataCreatePtr pvDataCreate;
    epics::pvData::StandardFieldPtr standardField;
    epics::pvData::StandardPVFieldPtr standardPVField;
    NTFieldPtr ntstructureField;
};

}}
#endif  /* NTFIELD_H */
