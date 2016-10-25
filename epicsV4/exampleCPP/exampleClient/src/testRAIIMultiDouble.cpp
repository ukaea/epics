/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 */

#include <iostream>

#include <pv/pvaClientMultiChannel.h>
#include <epicsThread.h>

using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvaClient;

static void example(
     PvaClientPtr const &pva,
     string provider,
     shared_vector<const string> const &channelNames)
{
    cout << "_example provider " << provider << " channels " << channelNames << endl;
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
    string provider("pva");
    size_t nelements(5);
    size_t ntimes(1);
    bool debug(false);
    if(argc==2 && string(argv[1])==string("-help")) {
        cout << "provider nelements ntimes debug" << endl;
        cout << "default" << endl;
        cout << provider << " " <<  nelements << " " << ntimes  << " " << (debug ? "true" : "false") << endl;
        return 0;
    }
    if(argc>1) provider = argv[1];
    if(argc>2) nelements = strtoul(argv[2],0,0);
    if(argc>3) ntimes = strtoul(argv[3],0,0);
    if(argc>4) {
        string value(argv[4]);
        if(value=="true") debug = true;
    }
    if(nelements<1) nelements = 1;
    if(nelements>5) nelements = 5;
    bool pvaSrv((provider.find("pva")==string::npos ? false : true));
    bool caSrv((provider.find("ca")==string::npos ? false : true));
    cout << "provider \"" << provider << "\""
         << " pvaSrv " << (pvaSrv ? "true" : "false")
         << " caSrv " << (caSrv ? "true" : "false")
         << " nelements " <<  nelements
         << " ntimes " << ntimes
         << " debug " << (debug ? "true" : "false") << endl;
    cout << "_____testRAIIMultiDouble.cpp starting_______\n";
    try {

        PvaClientPtr pva = PvaClient::get(provider);
        if(debug) PvaClient::setDebug(true);
        size_t num = nelements;
        shared_vector<string> channelNames(num);
        channelNames[0] = "DBRdouble01";
        if(nelements>1) channelNames[1] = "DBRint01";
        if(nelements>2)channelNames[2] = "DBRdouble03";
        if(nelements>3)channelNames[3] = "DBRdouble04";
        if(nelements>4)channelNames[4] = "DBRdouble05";
        shared_vector<const string> names(freeze(channelNames));
        if(pvaSrv) {
            for(size_t i=0; i<ntimes ; ++i) {
                example(pva,"pva",names);
            }
        }
        if(caSrv) {
            for(size_t i=0; i<ntimes ; ++i) {
                example(pva,"ca",names);
            }
        }
        cout << "_____testRAIIMultiDouble.cpp done_______\n";
     } catch (std::runtime_error e) {
        cout << "exception " << e.what() << endl;
        return 1;
    }
    return 0;
}
