/*ExampleServerMain.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 */

/* Author: Marty Kraimer */

#include <string>
#include <iostream>

#include <pv/exampleServer.h>
#include <pv/traceRecord.h>
#include <pv/channelProviderLocal.h>

using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvDatabase;
using namespace epics::exampleServer;

int main(int argc,char *argv[])
{
    PVDatabasePtr master = PVDatabase::getMaster();
    PVRecordPtr pvRecord;
    bool result = false;
    string recordName;

    recordName = "exampleServer";
    pvRecord = ExampleServer::create(recordName);
    result = master->addRecord(pvRecord);
    if(!result) cout<< "record " << recordName << " not added" << endl;

    recordName = "traceRecordPGRPC";
    pvRecord = TraceRecord::create(recordName);
    result = master->addRecord(pvRecord);
    if(!result) cout<< "record " << recordName << " not added" << endl;

  
    ContextLocal::shared_pointer contextLocal = ContextLocal::create();
    contextLocal->start();

    PVStringArrayPtr pvNames = master->getRecordNames();
    shared_vector<const string> names = pvNames->view();
    for(size_t i=0; i<names.size(); ++i) cout << names[i] << endl;

    contextLocal->waitForExit();

    return 0;
}

