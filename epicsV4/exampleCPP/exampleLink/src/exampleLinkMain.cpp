/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
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

#include <pv/channelProviderLocal.h>
#include <pv/serverContext.h>
#include <pv/ntscalarArray.h>

#include <pv/exampleMonitorLinkRecord.h>
#include <pv/exampleGetLinkRecord.h>
#include <pv/examplePutLinkRecord.h>

using namespace std;
using std::tr1::static_pointer_cast;
using namespace epics::pvData;
using namespace epics::nt;
using namespace epics::pvaClient;
using namespace epics::pvAccess;
using namespace epics::pvDatabase;
using namespace epics::exampleCPP::exampleLink;

int main(int argc,char *argv[])
{
    string provider("pva");
    string linkedRecordName("doubleArray");
    bool generateLinkedRecord(true);
    if(argc==2 && string(argv[1])==string("-help")) {
        cout << "provider  linkedRecordName generateLinkedRecord" << endl;
        cout << "default" << endl;
        cout << provider << " " << " " << linkedRecordName << " true" << endl;
        return 0;
    }
    if(argc>1) provider = argv[1];
    if(argc>2) linkedRecordName = argv[2];
    if(argc>3) {
        string val = argv[3];
        if(val=="false") generateLinkedRecord = false;
    }
    try {
        PVDatabasePtr master = PVDatabase::getMaster();
        ChannelProviderLocalPtr channelProvider = getChannelProviderLocal();
        PvaClientPtr pva= PvaClient::get(provider);
        ServerContext::shared_pointer ctx =
        startPVAServer("local",0,true,true);
        
        if(generateLinkedRecord) {
            NTScalarArrayBuilderPtr ntScalarArrayBuilder = NTScalarArray::createBuilder();
            PVStructurePtr pvStructure = ntScalarArrayBuilder->
                value(pvDouble)->
                addAlarm()->
                addTimeStamp()->
                createPVStructure();
            PVRecordPtr pvRecord(PVRecord::create(linkedRecordName,pvStructure));
            master->addRecord(pvRecord);
        }
        ExampleMonitorLinkRecordPtr pvMonitorRecord(
            ExampleMonitorLinkRecord::create(
                 pva, "exampleMonitorLink",provider,linkedRecordName));
        master->addRecord(pvMonitorRecord);
        ExampleGetLinkRecordPtr pvGetRecord(
            ExampleGetLinkRecord::create(
                 pva,"exampleGetLink",provider,linkedRecordName));
        master->addRecord(pvGetRecord);
        ExamplePutLinkRecordPtr pvPutRecord(
            ExamplePutLinkRecord::create(
                 pva,"examplePutLink",provider,linkedRecordName));
        master->addRecord(pvPutRecord);
        cout << "exampleMonitorLink\n";
        string str;
        while(true) {
            cout << "Type exit to stop: \n";
            getline(cin,str);
            if(str.compare("exit")==0) break;
        }
        ctx->destroy();
    } catch (std::runtime_error e) {
        cerr << "exception " << e.what() << endl;
        return 1;
    }
    return 0;
}
