// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution

/*pvaClientTestPutData.cpp */

/**
 * @author mrk
 */

/* Author: Marty Kraimer */

#include <iostream>

#include <epicsUnitTest.h>
#include <testMain.h>

#include <pv/pvaClient.h>
#include <pv/bitSet.h>

using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvaClient;

static PvaClientPtr pvaClient;
static FieldCreatePtr fieldCreate;
static StandardFieldPtr standardField;
static PVDataCreatePtr pvDataCreate;

static void testPostPut()
{
    testDiag("== testPostPut ==");
    StructureConstPtr structure =
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
            createStructure();

    PvaClientPutDataPtr pvaData = PvaClientPutData::create(structure);
    PVStructurePtr pvStructure = pvaData->getPVStructure();

    BitSetPtr change = pvaData->getChangedBitSet();
    PVDoublePtr powerValue = pvStructure->getSubField<PVDouble>("power.value");
    PVDoublePtr voltageValue = pvStructure->getSubField<PVDouble>("voltage.value");
    PVDoublePtr currentValue = pvStructure->getSubField<PVDouble>("current.value");
    size_t powerOffset = powerValue->getFieldOffset();
    size_t voltageOffset = voltageValue->getFieldOffset();
    size_t currentOffset = currentValue->getFieldOffset();

    change->clear();
    powerValue->put(1.0);
    voltageValue->put(2.0);
    currentValue->put(.5);

    testOk(change->cardinality()==3,"3 fields changed");
    testOk(change->get(powerOffset),"power changed");
    testOk(change->get(voltageOffset),"voltage changed");
    testOk(change->get(currentOffset),"current changed");
}

static void testDouble()
{
    testDiag("== testDouble ==");
    StructureConstPtr structure =
        fieldCreate->createFieldBuilder() ->
            add("alarm",standardField->alarm()) ->
            add("timeStamp",standardField->timeStamp()) ->
            add("value",pvDouble) ->
            createStructure();

    PvaClientPutDataPtr pvaData = PvaClientPutData::create(structure);
    PVDoublePtr pvDouble = pvaData->getPVStructure() ->
        getSubField<PVDouble>("value");
    pvDouble->put(5.0);

    BitSetPtr change = pvaData->getChangedBitSet();
    size_t valueOffset = pvDouble->getFieldOffset();
    testOk(change->cardinality()==1,"1 field changed");
    testOk(change->get(valueOffset),"value changed");

    testOk(pvaData->hasValue(),"hasValue");
    testOk(pvaData->isValueScalar(),"isValueScalar");
    testOk(!pvaData->isValueScalarArray(),"!isValueScalarArray");

    try {
        testOk(!!pvaData->getValue(), "getValue");
    } catch (std::runtime_error e) {
        testFail("getValue exception '%s'", e.what());
    }

    try {
        testOk(!!pvaData->getScalarValue(), "getScalarValue");
    } catch (std::runtime_error e) {
        testFail("getScalarValue exception '%s'", e.what());
    }
    try {
        testOk(!pvaData->getArrayValue(), "!getArrayValue");
    } catch (std::runtime_error e) {
        testPass("getArrayValue exception '%s'", e.what());
    }
    try {
        testOk(!pvaData->getScalarArrayValue(), "!getScalarArrayValue");
    } catch (std::runtime_error e) {
        testPass("getScalarArrayValue exception '%s'", e.what());
    }

    try {
        testOk(pvaData->getDouble() == 5.0, "getDouble value");
    } catch (std::runtime_error e) {
        testFail("getDouble exception '%s'", e.what());
    }
    try {
        testOk(pvaData->getString() == "5", "getString value");
    } catch (std::runtime_error e) {
        testFail("getString exception '%s'", e.what());
    }

    try {
        shared_vector<const double> value = pvaData->getDoubleArray();
        testFail("getDoubleArray");
    } catch (std::runtime_error e) {
        testPass("getDoubleArray exception '%s'", e.what());
    }
    try {
        shared_vector<const string> value = pvaData->getStringArray();
        testFail("getStringArray");
    } catch (std::runtime_error e) {
        testPass("getStringArray exception '%s'", e.what());
    }

    try {
        pvaData->putDouble(5.0);
        testPass("putDouble");
    } catch (std::runtime_error e) {
        testFail("putDouble exception '%s'", e.what());
    }
    try {
        pvaData->putString("1e5");
        testPass("putString");
    } catch (std::runtime_error e) {
        testFail("putString exception '%s'", e.what());
    }

    try {
        size_t len = 2;
        shared_vector<double> val(len);
        for (size_t i=0; i<len; ++i)
            val[i] = (i+1) * 10.0;
        pvaData->putDoubleArray(freeze(val));
        testFail("putDoubleArray");
    } catch (std::runtime_error e) {
        testPass("putDoubleArray exception '%s'", e.what());
    }
    try {
        size_t len = 2;
        shared_vector<string> val(len);
        val[0] = "one"; val[1] = "two";
        pvaData->putStringArray(freeze(val));
        testFail("putStringArray");
    } catch (std::runtime_error e) {
        testPass("putStringArray exception '%s'", e.what());
    }
}

static void testDoubleArray()
{
    testDiag("== testDoubleArray ==");
    StructureConstPtr structure =
       fieldCreate->createFieldBuilder() ->
            add("alarm",standardField->alarm()) ->
            add("timeStamp",standardField->timeStamp()) ->
            addArray("value",pvDouble) ->
            createStructure();

    PvaClientPutDataPtr pvaData = PvaClientPutData::create(structure);
    PVDoubleArrayPtr pvalue = pvaData->getPVStructure() ->
        getSubField<PVDoubleArray>("value");

    size_t len = 5;
    shared_vector<double> value(len);
    for (size_t i=0; i<len; ++i)
        value[i] = i * 10.0;
    pvalue->replace(freeze(value));

    BitSetPtr change = pvaData->getChangedBitSet();
    size_t valueOffset = pvalue->getFieldOffset();
    testOk(change->cardinality()==1,"1 field changed");
    testOk(change->get(valueOffset),"value changed");

    testOk(pvaData->hasValue(),"hasValue");
    testOk(!pvaData->isValueScalar(),"!isValueScalar");
    testOk(pvaData->isValueScalarArray(),"isValueScalarArray");

    try {
        testOk(!!pvaData->getValue(), "getValue");
    } catch (std::runtime_error e) {
        testFail("getValue exception '%s'", e.what());
    }

    try {
        testOk(!pvaData->getScalarValue(), "!getScalarValue");
    } catch (std::runtime_error e) {
        testPass("getScalarValue exception '%s'", e.what());
    }
    try {
        testOk(!!pvaData->getArrayValue(), "getArrayValue");
    } catch (std::runtime_error e) {
        testFail("getArrayValue exception '%s'", e.what());
    }
    try {
        testOk(!!pvaData->getScalarArrayValue(), "getScalarArrayValue");
    } catch (std::runtime_error e) {
        testPass("getScalarArrayValue exception '%s'", e.what());
    }

    try {
        testFail("getDouble %g", pvaData->getDouble());
    } catch (std::runtime_error e) {
        testPass("getDouble exception '%s'", e.what());
    }
    try {
        testFail("getString %s", pvaData->getString().c_str());
    } catch (std::runtime_error e) {
        testPass("getString exception '%s'", e.what());
    }

    try {
        shared_vector<const double> value = pvaData->getDoubleArray();
        testPass("getDoubleArray");
    } catch (std::runtime_error e) {
        testFail("getDoubleArray exception '%s'", e.what());
    }
    try {
        shared_vector<const string> value = pvaData->getStringArray();
    } catch (std::runtime_error e) {
        testPass("getStringArray exception '%s'", e.what());
    }

    try {
        pvaData->putDouble(5.0);
        testFail("putDouble");
    } catch (std::runtime_error e) {
        testPass("putDouble exception '%s'", e.what());
    }
    try {
        pvaData->putString("1e5");
        testFail("putString");
    } catch (std::runtime_error e) {
        testPass("putString exception '%s'", e.what());
    }

    try {
        size_t len = 2;
        shared_vector<double> val(len);
        for (size_t i=0; i<len; ++i)
            val[i] = (i+1) * 2.0;
        pvaData->putDoubleArray(freeze(val));
        testPass("putDoubleArray");
    } catch (std::runtime_error e) {
        testFail("putDoubleArray exception '%s'", e.what());
    }
    try {
        size_t len = 2;
        shared_vector<string> val(len);
        val[0] = "one"; val[1] = "two";
        pvaData->putStringArray(freeze(val));
        testFail("putStringArray");
    } catch (std::runtime_error e) {
        testPass("putStringArray exception '%s'", e.what());
    }
}

MAIN(pvaClientTestPutData)
{
    pvaClient = PvaClient::get("pva");
    fieldCreate = getFieldCreate();
    standardField = getStandardField();
    pvDataCreate = getPVDataCreate();

    testPlan(38);
    testDiag("=== pvaClientTestPutData ===");

    testPostPut();
    testDouble();
    testDoubleArray();

    testDone();
pvaClient.reset();
fieldCreate.reset();
standardField.reset();
pvDataCreate.reset();
    return 0;
}
