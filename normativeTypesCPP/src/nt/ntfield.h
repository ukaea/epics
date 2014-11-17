/* ntfield.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
#ifndef NTFIELD_H
#define NTFIELD_H

#include <cstdarg>

#include <pv/standardField.h>
#include <pv/standardPVField.h>
#include <pv/alarm.h>
#include <pv/pvAlarm.h>
#include <pv/timeStamp.h>
#include <pv/pvTimeStamp.h>
#include <pv/sharedVector.h>

namespace epics { namespace pvData { 
/**
 * Convenience Class for introspection fields of a Normative Type
 * @author mrk
 * 
 */

class NTField;
typedef std::tr1::shared_ptr<NTField> NTFieldPtr;

class PVNTField;
typedef std::tr1::shared_ptr<PVNTField> PVNTFieldPtr;

class NTField {
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
    bool isEnumerated(FieldConstPtr const & field);
    /**
     * Is field a timeStamp structure.
     * @param field The field to test.
     * @return (false,true) if field (is not,is) a timeStamp structure.
     */
    bool isTimeStamp(FieldConstPtr const & field);
    /**
     * Is field an alarm structure.
     * @param field The field to test.
     * @return (false,true) if field (is not,is) an alarm structure.
     */
    bool isAlarm(FieldConstPtr const & field);
    /**
     * Is field a display structure.
     * @param field The field to test.
     * @return (false,true) if field (is not,is) a display structure.
     */
    bool isDisplay(FieldConstPtr const & field);
    /**
     * Is field an alarmLimit structure.
     * @param field The field to test.
     * @return (false,true) if field (is not,is) an alarmLimit structure.
     */
    bool isAlarmLimit(FieldConstPtr const & field);
    /**
     * Is field a control structure.
     * @param field The field to test.
     * @return (false,true) if field (is not,is) a control structure.
     */
    bool isControl(FieldConstPtr const & field);

    /**
     * Create an enumerated structure.
     * @return an enumerated structure.
     */
    StructureConstPtr createEnumerated();
    /**
     * Create a timeStamp structure.
     * @return a timeStamp structure.
     */
    StructureConstPtr createTimeStamp();
    /**
     * Create an alarm structure.
     * @return an alarm structure.
     */
    StructureConstPtr createAlarm();
    /**
     * Create a display structure.
     * @return a displayalarm structure.
     */
    StructureConstPtr createDisplay();
    /**
     * Create a control structure.
     * @return a control structure.
     */
    StructureConstPtr createControl();

    /**
     * Create an array of enumerated structures.
     * @return an array of enumerated structures.
     */
    StructureArrayConstPtr createEnumeratedArray();
    /**
     * Create an array of timeStamp structures.
     * @return an array of timeStamp structures.
     */
    StructureArrayConstPtr createTimeStampArray();
    /**
     * Create an array of alarm structures.
     * @return an array of alarm structures.
     */
    StructureArrayConstPtr createAlarmArray();
private:
    NTField();
    FieldCreatePtr fieldCreate;
    StandardFieldPtr standardField;
};

/**
 * Convenience Class for data fields of a Normative Type
 * @author mrk
 * 
 */
class PVNTField {
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
    PVStructurePtr createEnumerated(
        StringArray const & choices);
    /**
     * Create a timeStamp PVStructure.
     * @return a timeStamp PVStructure..
     */
    PVStructurePtr createTimeStamp();
    /**
     * Create an alarm PVStructure.
     * @return an alarm PVStructure..
     */
    PVStructurePtr createAlarm();
    /**
     * Create a display PVStructure.
     * @return a display PVStructure..
     */
    PVStructurePtr createDisplay();
    /**
     * Create an alarmLimit PVStructure.
     * @return an alarmLimit PVStructure..
     */
    PVStructurePtr createAlarmLimit();
    /**
     * Create a control PVStructure.
     * @return a control PVStructure..
     */
    PVStructurePtr createControl();

    /**
     * Create an enumerated PVStructureArray.
     * @return an enumerated PVStructureArray..
     */
    PVStructureArrayPtr createEnumeratedArray();
    /**
     * Create a timeStamp PVStructureArray.
     * @return a timeStamp PVStructureArray
     */
    PVStructureArrayPtr createTimeStampArray();
    /**
     * Create an alarm PVStructureArray.
     * @return an alarm PVStructureArray..
     */
    PVStructureArrayPtr createAlarmArray();
private:
    PVNTField();
    PVDataCreatePtr pvDataCreate;
    StandardFieldPtr standardField;
    StandardPVFieldPtr standardPVField;
    NTFieldPtr ntstructureField;
};

}}
#endif  /* NTFIELD_H */
