/* exampleClient.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 */
/* Author Marty Kraimer 2012.01 */

#include <cstddef>
#include <cstdlib>
#include <cstddef>
#include <string>
#include <cstdio>

#include <epicsThread.h>
#include <epicsAssert.h>
#include <epicsExit.h>

#include <pv/ezchannelRPC.h>

using namespace epics::pvData;
using namespace epics::pvAccess;
using std::tr1::static_pointer_cast;


static FILE *fd = 0;
static FILE *auxfd = 0;

static void dump(EZChannelRPC::shared_pointer const & channelRPC)
{
    printf("%s\n",channelRPC->getMessage().c_str());
}

void exampleClient()
{
    String builder;
    FieldCreatePtr fieldCreate = getFieldCreate();
    PVDataCreatePtr pvDataCreate = getPVDataCreate();

    // Create an argument to pass to service
    size_t n = 3;
    FieldConstPtrArray fields;
    StringArray fieldNames;
    fields.reserve(n);
    fieldNames.reserve(n);
    fieldNames.push_back("function");
    fieldNames.push_back("names");
    fieldNames.push_back("values");
    fields.push_back(fieldCreate->createScalar(pvString));
    fields.push_back(fieldCreate->createScalarArray(pvString));
    fields.push_back(fieldCreate->createScalarArray(pvString));
    StructureConstPtr structure = fieldCreate->createStructure(fieldNames,fields);
    PVStructurePtr pvArgument = pvDataCreate->createPVStructure(structure);
    PVStringPtr pvfunction = pvArgument->getStringField("function");
    PVStringArrayPtr pvnames = static_pointer_cast<PVStringArray>
        (pvArgument->getScalarArrayField("names",pvString));
    PVStringArrayPtr pvvalues = static_pointer_cast<PVStringArray>
        (pvArgument->getScalarArrayField("values",pvString));
    pvfunction->put("example");
    size_t  nnameValue = 2;
    StringArray names;
    names.reserve(nnameValue);
    StringArray values;
    values.reserve(nnameValue);
    names.push_back("name0");
    names.push_back("name1");
    values.push_back("value0");
    values.push_back("value1");
    pvnames->put(0,2,names,0);
    pvvalues->put(0,2,values,0);
    // create a channelRPC and connect
    EZChannelRPC::shared_pointer channelRPC(new EZChannelRPC("serviceRPC"));
printf("calling connect\n");
    bool result = channelRPC->connect(1.0);
    if(!result) {dump(channelRPC); return;}

    //make a request
printf("calling request\n");
    PVStructure::shared_pointer pvResponse
        = channelRPC->request(pvArgument,false);
    if(pvResponse.get()==0) {dump(channelRPC); return;}

builder.clear();
pvResponse->toString(&builder);
printf("\nresponse\n%s\n",builder.c_str());

    // now look for expected data from service
    PVIntPtr pvSeverity = pvResponse->getIntField("alarm.severity");
    if(pvSeverity==0) exit(1);
    if(pvSeverity->get()!=0) {
        printf("alarm severity not 0\n");
    }
    PVDoubleArrayPtr  pvposition = static_pointer_cast<PVDoubleArray>
        (pvResponse->getScalarArrayField("position",pvDouble));
    if(pvposition!=0) {
        builder.clear();
        pvposition->toString(&builder);
        printf("\nposition\n%s\n",builder.c_str());
      
    }
    // can look for other data
    channelRPC->destroy();
}

int main(int argc,char *argv[])
{
    char *fileName = 0;
    if(argc>1) fileName = argv[1];
    fd = stdout;
    if(fileName!=0 && fileName[0]!=0) {
        fd = fopen(fileName,"w+");
    }
    char *auxFileName = 0;
    if(argc>2) auxFileName = argv[2];
    auxfd = stdout;
    if(auxFileName!=0 && auxFileName[0]!=0) {
        auxfd = fopen(auxFileName,"w+");
    }
    ClientFactory::start();
    exampleClient();
    return (0);
}

