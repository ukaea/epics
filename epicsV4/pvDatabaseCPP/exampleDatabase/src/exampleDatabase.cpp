/*exampleDatabase.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 * @date 2013.07.24
 */

/* Author: Marty Kraimer */

#include <cstddef>
#include <cstdlib>
#include <cstddef>
#include <string>
#include <cstdio>
#include <memory>
#include <vector>
#include <iostream>

#include <pv/standardField.h>
#include <pv/standardPVField.h>
#include <pv/channelProviderLocal.h>
#include <pv/traceRecord.h>

#include <pv/powerSupply.h>

#define epicsExportSharedSymbols
#include <pv/exampleDatabase.h>

using namespace std;
using std::tr1::static_pointer_cast;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvDatabase;

static FieldCreatePtr fieldCreate = getFieldCreate();
static StandardFieldPtr standardField = getStandardField();
static PVDataCreatePtr pvDataCreate = getPVDataCreate();
static StandardPVFieldPtr standardPVField = getStandardPVField();

static void createStructureArrayRecord(
    PVDatabasePtr const &master,
    string const &recordName)
{
    StringArray names(2);
    FieldConstPtrArray fields(2);
    names[0] = "name";
    names[1] = "value";
    fields[0] = fieldCreate->createScalar(pvString);
    fields[1] = fieldCreate->createScalar(pvString);

    StringArray topNames(1);
    FieldConstPtrArray topFields(1);
    topNames[0] = "value";
    topFields[0] = fieldCreate->createStructureArray(
        fieldCreate->createStructure(names, fields));
    StructureConstPtr top = fieldCreate->createStructure(topNames,topFields);
    PVStructurePtr pvStructure = pvDataCreate->createPVStructure(top);
    PVRecordPtr pvRecord = PVRecord::create(recordName,pvStructure);
    bool result = master->addRecord(pvRecord);
    if(!result) cout<< "record " << recordName << " not added" << endl;
}

static void createRegularUnionArrayRecord(
    PVDatabasePtr const &master,
    string const &recordName)
{
    StringArray unionNames(2);
    FieldConstPtrArray unionFields(2);
    unionNames[0] = "string";
    unionNames[1] = "stringArray";
    unionFields[0] = fieldCreate->createScalar(pvString);
    unionFields[1] = fieldCreate->createScalarArray(pvString);

    StringArray names(1);
    FieldConstPtrArray fields(1);
    fields[0] = fieldCreate->createUnionArray(fieldCreate->createUnion(unionNames,unionFields));
    names[0] = "value";
    StructureConstPtr top = fieldCreate->createStructure(names,fields);
    PVStructurePtr pvStructure = pvDataCreate->createPVStructure(top);
    PVRecordPtr pvRecord = PVRecord::create(recordName,pvStructure);
    bool result = master->addRecord(pvRecord);
    if(!result) cout<< "record " << recordName << " not added" << endl;
}

static void createVariantUnionArrayRecord(
    PVDatabasePtr const &master,
    string const &recordName)
{
    StringArray names(1);
    FieldConstPtrArray fields(1);
    fields[0] = fieldCreate->createVariantUnionArray();
    names[0] = "value";
    StructureConstPtr top = fieldCreate->createStructure(names,fields);
    PVStructurePtr pvStructure = pvDataCreate->createPVStructure(top);
    PVRecordPtr pvRecord = PVRecord::create(recordName,pvStructure);
    bool result = master->addRecord(pvRecord);
    if(!result) cout<< "record " << recordName << " not added" << endl;
}

static void createRecords(
    PVDatabasePtr const &master,
    ScalarType scalarType,
    string const &recordNamePrefix,
    string const &properties)
{
    string recordName = recordNamePrefix;
    PVStructurePtr pvStructure = standardPVField->scalar(scalarType,properties);
    PVRecordPtr pvRecord = PVRecord::create(recordName,pvStructure);
    bool result = master->addRecord(pvRecord);
    if(!result) cout<< "record " << recordName << " not added" << endl;
    recordName += "Array";
    pvStructure = standardPVField->scalarArray(scalarType,properties);
    pvRecord = PVRecord::create(recordName,pvStructure);
    result = master->addRecord(pvRecord);
}

void ExampleDatabase::create()
{
    PVDatabasePtr master = PVDatabase::getMaster();
    PVRecordPtr pvRecord;
    string recordName;
    bool result(false);
    recordName = "traceRecordPGRPC";
    pvRecord = TraceRecord::create(recordName);
    result = master->addRecord(pvRecord);
    if(!result) cout<< "record " << recordName << " not added" << endl;
    string properties;
    properties = "alarm,timeStamp";
    createRecords(master,pvBoolean,"exampleBoolean",properties);
    createRecords(master,pvByte,"exampleByte",properties);
    createRecords(master,pvShort,"exampleShort",properties);
    createRecords(master,pvInt,"exampleInt",properties);
    createRecords(master,pvLong,"exampleLong",properties);
    createRecords(master,pvFloat,"exampleFloat",properties);
    createRecords(master,pvDouble,"exampleDouble",properties);
    createRecords(master,pvDouble,"exampleDouble01",properties);
    createRecords(master,pvDouble,"exampleDouble02",properties);
    createRecords(master,pvDouble,"exampleDouble03",properties);
    createRecords(master,pvDouble,"exampleDouble04",properties);
    createRecords(master,pvDouble,"exampleDouble05",properties);
    createRecords(master,pvString,"exampleString",properties);
    createStructureArrayRecord(master,"exampleStructureArray");
    createRegularUnionArrayRecord(master,"exampleRegularUnionArray");
    createVariantUnionArrayRecord(master,"exampleVariantUnionArray");
    recordName = "examplePowerSupply";
    PVStructurePtr pvStructure = createPowerSupply();
    PowerSupplyPtr psr = PowerSupply::create(recordName,pvStructure);
    if(!psr) {
          cout << "PowerSupply::create failed" << endl;
    } else {
        result = master->addRecord(psr);
        if(!result) cout<< "record " << recordName << " not added" << endl;
    }
}

