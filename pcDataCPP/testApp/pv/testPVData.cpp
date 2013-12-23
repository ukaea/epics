/* testPVdata.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/* Author:  Marty Kraimer Date: 2010.11 */

#include <cstddef>
#include <cstdlib>
#include <cstddef>
#include <string>
#include <cstdio>

#include <epicsAssert.h>
#include <epicsExit.h>

#include <pv/requester.h>
#include <pv/pvIntrospect.h>
#include <pv/pvData.h>
#include <pv/convert.h>
#include <pv/standardField.h>
#include <pv/standardPVField.h>

using namespace epics::pvData;
using std::tr1::static_pointer_cast;

static bool debug = false;

static FieldCreatePtr fieldCreate;
static PVDataCreatePtr pvDataCreate;
static StandardFieldPtr standardField;
static StandardPVFieldPtr standardPVField;
static ConvertPtr convert;
static String builder("");
static String alarmTimeStamp("alarm,timeStamp");
static String alarmTimeStampValueAlarm("alarm,timeStamp,valueAlarm");
static String allProperties("alarm,timeStamp,display,control,valueAlarm");

static void testAppend(FILE * fd)
{
    if(debug) fprintf(fd,"\ntestAppend\n");
    PVFieldPtrArray pvFields;
    StringArray fieldNames;
    PVStructurePtr pvParent = pvDataCreate->createPVStructure(
        fieldNames,pvFields);
    PVStringPtr pvStringField = static_pointer_cast<PVString>(
        pvDataCreate->createPVScalar(pvString));
    pvStringField->put(String("value,timeStamp"));
    PVFieldPtr pvField = pvStringField;
    pvParent->appendPVField("request",pvField);
    builder.clear();
    pvParent->toString(&builder);
    if(debug) fprintf(fd,"%s\n",builder.c_str());
    fprintf(fd,"testAppend PASSED\n");
}

static void testCreatePVStructure(FILE * fd)
{
    if(debug) fprintf(fd,"\ntestCreatePVStructure\n");
    PVStructurePtr pv0 = standardPVField->scalar(
         pvDouble,alarmTimeStampValueAlarm);
    PVScalarPtr pv1 = pvDataCreate->createPVScalar(pvString);
    PVFieldPtrArray pvFields;
    StringArray fieldNames;
    pvFields.reserve(2);
    fieldNames.reserve(2);
    fieldNames.push_back("value");
    fieldNames.push_back("extra");
    pvFields.push_back(pv0);
    pvFields.push_back(pv1);
    PVStructurePtr pvParent = pvDataCreate->createPVStructure(
        fieldNames,pvFields);
    builder.clear();
    pvParent->toString(&builder);
    if(debug) fprintf(fd,"%s\n",builder.c_str());
    fprintf(fd,"testCreatePVStructure PASSED\n");
}

static void testPVScalarCommon(FILE * fd,String /*fieldName*/,ScalarType stype)
{
    PVScalarPtr pvScalar = pvDataCreate->createPVScalar(stype);
    if(stype==pvBoolean) {
        convert->fromString(pvScalar,String("true"));
    } else {
        convert->fromString(pvScalar,String("10"));
    }
    builder.clear();
    pvScalar->toString(&builder);
    if(debug) fprintf(fd,"%s\n",builder.c_str());
}

static void testPVScalarWithProperties(
    FILE * fd,String /*fieldName*/,ScalarType stype)
{
    PVStructurePtr pvStructure;
    bool hasValueAlarm = false;
    bool hasBooleanAlarm = false;
    bool hasDisplayControl = false;
    switch(stype) {
        case pvBoolean: {
             pvStructure = standardPVField->scalar(
                 stype,alarmTimeStampValueAlarm);
             hasBooleanAlarm = true;
             PVBooleanPtr pvField = pvStructure->getBooleanField("value");
             pvField->put(true);
             break;
        }
        case pvByte: {
             pvStructure = standardPVField->scalar(
                 stype,allProperties);
             hasValueAlarm = true;
             hasDisplayControl = true;
             PVBytePtr pvField = pvStructure->getByteField("value");
             pvField->put(127);
             break;
        }
        case pvShort: {
             pvStructure = standardPVField->scalar(
                 stype,allProperties);
             hasValueAlarm = true;
             hasDisplayControl = true;
             PVShortPtr pvField = pvStructure->getShortField("value");
             pvField->put(32767);
             break;
        }
        case pvInt: {
             pvStructure = standardPVField->scalar(
                 stype,allProperties);
             hasValueAlarm = true;
             hasDisplayControl = true;
             PVIntPtr pvField = pvStructure->getIntField("value");
             pvField->put((int32)0x80000000);
             break;
        }
        case pvLong: {
             pvStructure = standardPVField->scalar(
                 stype,allProperties);
             hasValueAlarm = true;
             hasDisplayControl = true;
             PVLongPtr pvField = pvStructure->getLongField("value");
             int64 value = 0x80000000;
             value <<= 32;
             value |= 0xffffffff;
             pvField->put(value);
             break;
        }
        case pvUByte: {
             pvStructure = standardPVField->scalar(
                 stype,allProperties);
             hasValueAlarm = true;
             hasDisplayControl = true;
             PVUBytePtr pvField = pvStructure->getUByteField("value");
             pvField->put(255);
             break;
        }
        case pvUShort: {
             pvStructure = standardPVField->scalar(
                 stype,allProperties);
             hasValueAlarm = true;
             hasDisplayControl = true;
             PVUShortPtr pvField = pvStructure->getUShortField("value");
             pvField->put(65535);
             break;
        }
        case pvUInt: {
             pvStructure = standardPVField->scalar(
                 stype,allProperties);
             hasValueAlarm = true;
             hasDisplayControl = true;
             PVUIntPtr pvField = pvStructure->getUIntField("value");
             pvField->put((uint32)0x80000000);
             break;
        }
        case pvULong: {
             pvStructure = standardPVField->scalar(
                 stype,allProperties);
             hasValueAlarm = true;
             hasDisplayControl = true;
             PVULongPtr pvField = pvStructure->getULongField("value");
             int64 value = 0x80000000;
             value <<= 32;
             value |= 0xffffffff;
             pvField->put(value);
             break;
        }
        case pvFloat: {
             pvStructure = standardPVField->scalar(
                 stype,allProperties);
             hasValueAlarm = true;
             hasDisplayControl = true;
             PVFloatPtr pvField = pvStructure->getFloatField("value");
             pvField->put(1.123e8);
             break;
        }
        case pvDouble: {
             pvStructure = standardPVField->scalar(
                 stype,allProperties);
             hasValueAlarm = true;
             hasDisplayControl = true;
             PVDoublePtr pvField = pvStructure->getDoubleField("value");
             pvField->put(1.123e35);
             break;
        }
        case pvString: {
             pvStructure = standardPVField->scalar(
                 stype,alarmTimeStamp);
             PVStringPtr pvField = pvStructure->getStringField("value");
             pvField->put(String("this is a string"));
             break;
        }
    }
    PVLongPtr seconds = pvStructure->getLongField(
        String("timeStamp.secondsPastEpoch"));
    assert(seconds!=0);
    seconds->put(123456789);
    PVIntPtr nano = pvStructure->getIntField(String("timeStamp.nanoSeconds"));
    assert(nano!=0);
    nano->put(1000000);
    PVIntPtr severity = pvStructure->getIntField(String("alarm.severity"));
    assert(severity!=0);
    severity->put(2);
    PVStringPtr message = pvStructure->getStringField(String("alarm.message"));
    assert(message!=0);
    message->put(String("messageForAlarm"));
    if(hasDisplayControl) {
        PVStringPtr desc = pvStructure->getStringField(
            String("display.description"));
        assert(desc!=0);
        desc->put(String("this is a description"));
        PVStringPtr format = pvStructure->getStringField(
            String("display.format"));
        assert(format!=0);
        format->put(String("f10.2"));
        PVStringPtr units = pvStructure->getStringField(
            String("display.units"));
        assert(units!=0);
        units->put(String("SomeUnits"));
        PVDoublePtr limit = pvStructure->getDoubleField(
            String("display.limitLow"));
        assert(limit!=0);
        limit->put(0.0);
        limit = pvStructure->getDoubleField(
            String("display.limitHigh"));
        assert(limit!=0);
        limit->put(10.0);
        limit = pvStructure->getDoubleField(
            String("control.limitLow"));
        assert(limit!=0);
        limit->put(1.0);
        limit = pvStructure->getDoubleField(
            String("control.limitHigh"));
        assert(limit!=0);
        limit->put(9.0);
    }
    if(hasValueAlarm) {
        PVFieldPtr  pvField = pvStructure->getSubField(
            String("valueAlarm.active"));
        PVBooleanPtr pvBoolean = static_pointer_cast<PVBoolean>(pvField);
        pvBoolean->put(true);
        pvField = pvStructure->getSubField(
            String("valueAlarm.lowAlarmLimit"));
        PVScalarPtr pvtemp = static_pointer_cast<PVScalar>(pvField);
        assert(pvtemp.get()!=0);
        convert->fromDouble(pvtemp,1.0);
        pvField = pvStructure->getSubField(
            String("valueAlarm.highAlarmLimit"));
        pvtemp = static_pointer_cast<PVScalar>(pvField);
        assert(pvtemp.get()!=0);
        convert->fromDouble(pvtemp,9.0);
        severity = pvStructure->getIntField(
            String("valueAlarm.lowAlarmSeverity"));
        assert(severity!=0);
        severity->put(2);
        severity = pvStructure->getIntField(
            String("valueAlarm.highAlarmSeverity"));
        assert(severity!=0);
        severity->put(2);
        PVBooleanPtr active = pvStructure->getBooleanField(
            String("valueAlarm.active"));
        assert(active!=0);
        active->put(true);
    }
    if(hasBooleanAlarm) {
        PVFieldPtr  pvField = pvStructure->getSubField(
            String("valueAlarm.active"));
        PVBooleanPtr pvBoolean = static_pointer_cast<PVBoolean>(pvField);
        pvBoolean->put(true);
        severity = pvStructure->getIntField(
            String("valueAlarm.falseSeverity"));
        assert(severity!=0);
        severity->put(0);
        severity = pvStructure->getIntField(
            String("valueAlarm.trueSeverity"));
        assert(severity!=0);
        severity->put(2);
        severity = pvStructure->getIntField(
            String("valueAlarm.changeStateSeverity"));
        assert(severity!=0);
        severity->put(1);
    }
    builder.clear();
    pvStructure->toString(&builder);
    if(debug) fprintf(fd,"%s\n",builder.c_str());
}

static void testPVScalar(FILE * fd) {
    if(debug) fprintf(fd,"\ntestScalar\n");
    testPVScalarCommon(fd,String("boolean"),pvByte);
    testPVScalarCommon(fd,String("byte"),pvByte);
    testPVScalarCommon(fd,String("short"),pvShort);
    testPVScalarCommon(fd,String("int"),pvInt);
    testPVScalarCommon(fd,String("long"),pvLong);
    testPVScalarCommon(fd,String("ubyte"),pvUByte);
    testPVScalarCommon(fd,String("ushort"),pvUShort);
    testPVScalarCommon(fd,String("uint"),pvUInt);
    testPVScalarCommon(fd,String("ulong"),pvULong);
    testPVScalarCommon(fd,String("float"),pvFloat);
    testPVScalarCommon(fd,String("double"),pvDouble);
    testPVScalarCommon(fd,String("string"),pvString);

    testPVScalarWithProperties(fd,String("boolean"),pvBoolean);
    testPVScalarWithProperties(fd,String("byte"),pvByte);
    testPVScalarWithProperties(fd,String("short"),pvShort);
    testPVScalarWithProperties(fd,String("int"),pvInt);
    testPVScalarWithProperties(fd,String("long"),pvLong);
    testPVScalarWithProperties(fd,String("ubyte"),pvUByte);
    testPVScalarWithProperties(fd,String("ushort"),pvUShort);
    testPVScalarWithProperties(fd,String("uint"),pvUInt);
    testPVScalarWithProperties(fd,String("ulong"),pvULong);
    testPVScalarWithProperties(fd,String("float"),pvFloat);
    testPVScalarWithProperties(fd,String("double"),pvDouble);
    testPVScalarWithProperties(fd,String("string"),pvString);
    fprintf(fd,"testScalar PASSED\n");
}


static void testScalarArrayCommon(FILE * fd,String /*fieldName*/,ScalarType stype)
{
    PVStructurePtr pvStructure = standardPVField->scalarArray(
        stype,alarmTimeStamp);
    PVScalarArrayPtr scalarArray = pvStructure->getScalarArrayField(
        "value",stype);
    assert(scalarArray.get()!=0);
    if(stype==pvBoolean) {
        StringArray values(3);
        values[0] = "true";
        values[1] = "false";
        values[2] = "true";
        convert->fromStringArray(scalarArray, 0,3,values,0);
    } else {
        StringArray values(3);
        values[0] = "0";
        values[1] = "1";
        values[2] = "2";
        convert->fromStringArray(scalarArray, 0,3,values,0);
    }
    builder.clear();
    pvStructure->toString(&builder);
    if(debug) fprintf(fd,"%s\n",builder.c_str());
    PVFieldPtr pvField = pvStructure->getSubField("alarm.status");
    pvField->message("this is a test",infoMessage);
}

static void testScalarArray(FILE * fd) {
    if(debug) fprintf(fd,"\ntestScalarArray\n");
    testScalarArrayCommon(fd,String("boolean"),pvBoolean);
    testScalarArrayCommon(fd,String("byte"),pvByte);
    testScalarArrayCommon(fd,String("short"),pvShort);
    testScalarArrayCommon(fd,String("int"),pvInt);
    testScalarArrayCommon(fd,String("long"),pvLong);
    testScalarArrayCommon(fd,String("float"),pvFloat);
    testScalarArrayCommon(fd,String("double"),pvDouble);
    testScalarArrayCommon(fd,String("string"),pvString);
    fprintf(fd,"testScalarArray PASSED\n");
}

int main(int argc,char *argv[])
{
    char *fileName = 0;
    if(argc>1) fileName = argv[1];
    FILE * fd = stdout;
    if(fileName!=0 && fileName[0]!=0) {
        fd = fopen(fileName,"w+");
    }
    fieldCreate = getFieldCreate();
    pvDataCreate = getPVDataCreate();
    standardField = getStandardField();
    standardPVField = getStandardPVField();
    convert = getConvert();
    testAppend(fd);
    testCreatePVStructure(fd);
    testPVScalar(fd);
    testScalarArray(fd);
    return(0);
}
