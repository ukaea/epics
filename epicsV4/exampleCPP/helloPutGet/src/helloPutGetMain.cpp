// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution

/*HelloPutGetMain.cpp */

/**
 * @author mrk
 */

/* Author: Marty Kraimer */

#include <string>
#include <iostream>

#include <pv/helloPutGetRecord.h>
#include <pv/channelProviderLocal.h>
#include <pv/serverContext.h>

using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvDatabase;
using namespace epics::exampleCPP::helloPutGet;

int main(int argc,char *argv[])
{
    PVDatabasePtr master = PVDatabase::getMaster();
    ChannelProviderLocalPtr channelProvider = getChannelProviderLocal();
    PVRecordPtr pvRecord;
    string recordName;

    recordName = "helloPutGet";
    pvRecord = HelloPutGetRecord::create(recordName);
    master->addRecord(pvRecord);

    ServerContext::shared_pointer ctx =
        startPVAServer("local",0,true,true);
    string str;
    while(true) {
        cout << "Type exit to stop: \n";
        getline(cin,str);
        if(str.compare("exit")==0) break;

    }
    ctx->destroy();
epicsThreadSleep(3.0);  // should not be necessary
    return 0;
}

