/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 */

/* Author: Marty Kraimer */

#include <iostream>

#include <pv/pvaClientMultiChannel.h>

using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvaClient;

static void example(
     PvaClientPtr const &pva,
     string provider,
     shared_vector<const string> const &channelNames)
{
    cout << "_example provider " << provider << " channels " << channelNames << "_\n";
    size_t num = channelNames.size();
    PvaClientMultiChannelPtr multiChannel(
        PvaClientMultiChannel::create(pva,channelNames,provider));
    Status status = multiChannel->connect();
    if(!status.isSuccess()) {
         cout << "Did not connect: ";
         shared_vector<epics::pvData::boolean> isConnected = multiChannel->getIsConnected();
         for(size_t i=0; i<num; ++i) {
             if(!isConnected[i]) cout << channelNames[i] << " ";
         }
         cout << endl;
         return;
    }
    PvaClientMultiGetDoublePtr multiGet(multiChannel->createGet());
    PvaClientMultiPutDoublePtr multiPut(multiChannel->createPut());
    PvaClientMultiMonitorDoublePtr multiMonitor(multiChannel->createMonitor());
    shared_vector<double> data(num,0);
    for(double value = 0.0; value< 1.0; value+= .2) {
        for(size_t i=0; i<num; ++i) data[i] = value + i;
        cout << "put " << data << endl;
        multiPut->put(data);
        data =  multiGet->get();
        cout << "get " << data << endl;
        bool result = multiMonitor->waitEvent(.1);
        while(result) {
             cout << "monitor  data " << multiMonitor->get() << endl;
             result = multiMonitor->poll();
        }
    }
}

int main(int argc,char *argv[])
{
    cout << "_____examplePvaClientMultiDouble starting_______\n";
    try {
        PvaClientPtr pva = PvaClient::get("pva ca");
//PvaClient::setDebug(true);
        size_t num = 5;
        shared_vector<string> channelNames(num);
        channelNames[0] = "PVRdouble01";
        channelNames[1] = "PVRint";
        channelNames[2] = "PVRdouble03";
        channelNames[3] = "PVRdouble04";
        channelNames[4] = "PVRdouble05";
        shared_vector<const string> names(freeze(channelNames));
        example(pva,"pva",names);
        PvaClientChannelPtr pvaChannel = pva->createChannel("DBRdouble00","pva");
        pvaChannel->issueConnect();
        Status status = pvaChannel->waitConnect(1.0);
        if(status.isOK()) {
            channelNames = shared_vector<string>(num);
            channelNames[0] = "DBRdouble01";
            channelNames[1] = "DBRint01";
            channelNames[2] = "DBRdouble03";
            channelNames[3] = "DBRdouble04";
            channelNames[4] = "DBRdouble05";
            names = freeze(channelNames);
            example(pva,"pva",names);
            example(pva,"ca",names);
        } else {
            cout << "DBRdouble00 not found\n";
        }
        cout << "_____examplePvaClientMultiDouble done_______\n";
     } catch (std::runtime_error e) {
        cout << "exception " << e.what() << endl;
        return 1;
    }
    return 0;
}
