/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 */

/* Author: Marty Kraimer */

#include <string>
#include <iostream>

#include <pv/exampleRPC.h>
#include <pv/channelProviderLocal.h>

using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvDatabase;
using namespace epics::exampleCPP::exampleRPC;

int main(int argc,char *argv[])
{
    PVDatabasePtr master = PVDatabase::getMaster();
    ChannelProviderLocalPtr channelProvider = getChannelProviderLocal();
    PVRecordPtr pvRecord;
    string recordName;

    recordName = "mydevice";
    pvRecord = ExampleRPC::create(recordName);
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
    return 0;
}

