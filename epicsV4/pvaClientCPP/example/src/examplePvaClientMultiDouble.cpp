/*examplePvaClientMultiDouble.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
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
    
    size_t num = channelNames.size();
cout << "num " << num << " names " << channelNames << endl;
    PvaClientMultiChannelPtr multiChannel(
        PvaClientMultiChannel::create(pva,channelNames,provider));
    PvaClientMultiGetDoublePtr multiGet(multiChannel->createGet());
    PvaClientMultiPutDoublePtr multiPut(multiChannel->createPut());
    PvaClientMultiMonitorDoublePtr multiMonitor(multiChannel->createMonitor());
    shared_vector<double> data(num,0);
    for(double value = 0.2; value< 2.3; value+= 1.0) {
        try {
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
        } catch (std::runtime_error e) {
            cout << "exception " << e.what() << endl;
        }
    }
}

int main(int argc,char *argv[])
{
    PvaClientPtr pva = PvaClient::create();
    size_t num = 5;
    shared_vector<string> channelNames(num);
    channelNames[0] = "double01";
    channelNames[1] = "int01";
    channelNames[2] = "double03";
    channelNames[3] = "double04";
    channelNames[4] = "double05";
    cout << "double pva\n";
    shared_vector<const string> names(freeze(channelNames));
    example(pva,"pva",names);
    cout << "double ca\n";
    example(pva,"ca",names);
    channelNames = shared_vector<string>(num);
    channelNames[0] = "exampleDouble01";
    channelNames[1] = "exampleInt";
    channelNames[2] = "exampleDouble03";
    channelNames[3] = "exampleDouble04";
    channelNames[4] = "exampleDouble05";
    names = freeze(channelNames);
    cout << "exampleDouble pva\n";
    example(pva,"pva",names);
    return 0;
}
