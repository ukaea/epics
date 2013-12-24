/* testPVAppend.cpp */
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
using std::size_t;

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

static void checkNameAndParent(
    FILE *fd,PVStructurePtr & pvStructure, int indentLevel)
{
    builder.clear();
    if(debug) {
        convert->newLine(&builder,indentLevel);
        fprintf(fd,"%s this %p indentLevel %d",
            builder.c_str(),pvStructure.get(),indentLevel);
    }
    PVFieldPtrArray pvFields = pvStructure->getPVFields();
    StringArray fieldNames = pvStructure->getStructure()->getFieldNames();
    for(size_t i = 0; i<pvFields.size(); i++) {
        PVFieldPtr pvField = pvFields[i];
        assert(pvField->getParent()==pvStructure.get());
        assert(pvField->getFieldName().compare(fieldNames[i])==0);
        builder.clear();
        if(debug) {
            convert->newLine(&builder,indentLevel);
            fprintf(fd,"%s this %p name %s parent %p",
                builder.c_str(),
                pvField.get(),
                pvField->getFieldName().c_str(),
                pvField->getParent());
        }
        if(pvField->getField()->getType()==structure) {
            PVStructurePtr xxx = static_pointer_cast<PVStructure>(pvField);
            checkNameAndParent(fd,xxx,indentLevel+1);
        }
    }
    builder.clear();
}

static void testAppendSimple(FILE * fd)
{
    if(debug) fprintf(fd,"\ntestAppendSimple\n");
    PVFieldPtrArray fields;
    StringArray names;
    PVStructurePtr pvParent = pvDataCreate->createPVStructure(names,fields);
    PVStringPtr pvStringField = static_pointer_cast<PVString>(
        pvDataCreate->createPVScalar(pvString));
    pvStringField->put("value,timeStamp");
    PVFieldPtr pvField = pvStringField;
    pvParent->appendPVField("fieldlist",pvField);
    pvStringField = static_pointer_cast<PVString>(
        pvDataCreate->createPVScalar(pvString));
    pvStringField->put("junk");
    pvField = pvStringField;
    pvParent->appendPVField("extra",pvField);
    builder.clear();
    pvParent->toString(&builder);
    if(debug) fprintf(fd,"%s\n",builder.c_str());
    fprintf(fd,"testAppendSimple PASSED\n");
}

static void testAppendMore(FILE * fd)
{
    if(debug) fprintf(fd,"\ntestAppendMore\n");
    PVFieldPtrArray fields;
    StringArray names;
    PVStructurePtr pvStructure = pvDataCreate->createPVStructure(names,fields);
    PVStructurePtr pvChild1 = pvDataCreate->createPVStructure(names,fields);
    PVStringPtr pvStringField = static_pointer_cast<PVString>(
        pvDataCreate->createPVScalar(pvString));
    pvStringField->put("bla");
    PVFieldPtr pvField = pvStringField;
    pvChild1->appendPVField("value",pvField);
    pvField = pvChild1;
    pvStructure->appendPVField("child1",pvField);
    PVStructurePtr pvChild2 = pvDataCreate->createPVStructure(names,fields);
    pvStringField = static_pointer_cast<PVString>(
        pvDataCreate->createPVScalar(pvString));
    pvStringField->put("blabla");
    pvField = pvStringField;
    pvChild2->appendPVField("value",pvField);
    pvField = pvChild2;
    pvStructure->appendPVField("child2",pvField);
    builder.clear();
    pvStructure->toString(&builder);
    if(debug) fprintf(fd,"%s\n",builder.c_str());
    checkNameAndParent(fd,pvStructure,0);
    fprintf(fd,"testAppendMore PASSED\n");
}

static void testAppendStructures(FILE * fd)
{
    if(debug) fprintf(fd,"\ntestAppendStructures\n");
    PVFieldPtrArray fields;
    StringArray names;
    PVStructurePtr pvParent = pvDataCreate->createPVStructure(names,fields);
    PVFieldPtrArray topFields;
    StringArray topNames;
    PVStructurePtr pvTop = pvDataCreate->createPVStructure(topNames,topFields);
    pvParent->appendPVField("top",pvTop);
    PVFieldPtrArray valueFields;
    StringArray valueNames;
    PVStructurePtr pvValue = pvDataCreate->createPVStructure(valueNames,valueFields);
    pvTop->appendPVField("value",pvValue);
    PVFieldPtrArray indexFields;
    StringArray indexNames;
    PVStructurePtr pvIndex = pvDataCreate->createPVStructure(indexNames,indexFields);
    pvValue->appendPVField("index",pvIndex);
    builder.clear();
    pvParent->toString(&builder);
    if(debug) fprintf(fd,"%s\n",builder.c_str());
    builder.clear();
    pvParent->getField()->toString(&builder);
    if(debug) fprintf(fd,"field\n%s\n",builder.c_str());
    fprintf(fd,"testAppendStructures PASSED\n");
}

static void append2(PVStructurePtr &pvStructure,
    const char *oneName,const char *twoName,
    const char *oneValue,const char *twoValue)
{
    PVFieldPtrArray pvFields;
    pvFields.reserve(2);
    StringArray names;
    names.reserve(2);
    PVStringPtr pvStringField = static_pointer_cast<PVString>(
        pvDataCreate->createPVScalar(pvString));
    pvStringField->put(oneValue);
    names.push_back(oneName);
    pvFields.push_back(pvStringField);
    pvStringField = static_pointer_cast<PVString>(
        pvDataCreate->createPVScalar(pvString));
    pvStringField->put(twoValue);
    names.push_back(twoName);
    pvFields.push_back(pvStringField);
    pvStructure->appendPVFields(names,pvFields);
}
static void testAppends(FILE * fd)
{
    if(debug) fprintf(fd,"\ntestAppends\n");
    PVFieldPtrArray emptyPVFields;
    StringArray emptyNames;
    PVFieldPtrArray pvFields;
    pvFields.reserve(2);
    StringArray names;
    names.reserve(2);
    names.push_back("child1");
    PVStructurePtr pvChild = pvDataCreate->createPVStructure(
        emptyNames,emptyPVFields);
    append2(pvChild,"Joe","Mary","Good Guy","Good Girl");
    pvFields.push_back(pvChild);
    names.push_back("child2");
    pvChild = pvDataCreate->createPVStructure(
        emptyNames,emptyPVFields);
    append2(pvChild,"Bill","Jane","Bad Guy","Bad Girl");
    pvFields.push_back(pvChild);
    PVStructurePtr pvStructure = pvDataCreate->createPVStructure(
        names,pvFields);
    builder.clear();
    pvStructure->toString(&builder);
    if(debug) fprintf(fd,"%s\n",builder.c_str());
    checkNameAndParent(fd,pvStructure,0);
    PVFieldPtr pvField = pvStructure->getSubField("child2.Bill");
    assert(pvField.get()!=NULL);
    pvField->renameField("Joe");
    builder.clear();
    pvStructure->toString(&builder);
    if(debug) fprintf(fd,"%s\n",builder.c_str());
    pvField->getParent()->removePVField("Joe");
    builder.clear();
    pvStructure->toString(&builder);
    if(debug) fprintf(fd,"%s\n",builder.c_str());
    checkNameAndParent(fd,pvStructure,0);
    fprintf(fd,"testAppends PASSED\n");
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
    testAppendStructures(fd);
    testAppendSimple(fd);
    testAppendMore(fd);
    testAppends(fd);
    return(0);
}

