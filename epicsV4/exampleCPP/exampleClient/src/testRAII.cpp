/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 */

/* Author: Marty Kraimer */
#include <iostream>
#include <epicsThread.h>

#include <pv/pvaClient.h>

using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvaClient;

static void getQuick(PvaClientPtr const &pva,string const & channelName,string const & providerName )
{
    cout << "_____getQuick starting_______\n";
    double value =  pva->channel(channelName,providerName)->get()->getData()->getDouble();
    cout << " get value " << value << endl;
    cout << "__getQuick__ returning\n";
}

static void getLongWay(PvaClientPtr const &pva,string const & channelName,string const & providerName )
{
    cout << "_____getLongWay starting_______\n";
    PvaClientChannelPtr pvaChannel = pva->createChannel(channelName,providerName);
    pvaChannel->issueConnect();
    Status status = pvaChannel->waitConnect(2.0);
    if(!status.isOK()) {cout << " connect failed\n"; return;}
    PvaClientGetPtr pvaGet = pvaChannel->createGet();
    pvaGet->issueConnect();
    status = pvaGet->waitConnect();
    if(!status.isOK()) {cout << " createGet failed\n"; return;}
    PvaClientGetDataPtr pvaData = pvaGet->getData();
    double value = pvaData->getDouble();
    cout << "get value " << value << endl;
    cout << "__getLongWay__ returning\n";
}



static void put(PvaClientPtr const &pva,string const & channelName,string const & providerName )
{
    cout << "_____put starting_______\n";
    double value =  pva->channel(channelName,providerName)->get()->getData()->getDouble();
    value += 1.0;
    PvaClientChannelPtr channel = pva->channel(channelName,providerName);
    PvaClientPutPtr put = channel->put();
    PvaClientPutDataPtr putData = put->getData();
    putData->putDouble(value); put->put();
    cout << "__put__ returning\n";
}

static void monitor(PvaClientPtr const &pva,string const & channelName,string const& providerName)
{
    cout << "__monitor channelName " << channelName << " provider " << providerName << "__\n";
    PvaClientChannelPtr pvaChannel = pva->channel(channelName,providerName);
    PvaClientMonitorPtr monitor = pvaChannel->createMonitor("value,timeStamp");
    monitor->connect();
    monitor->start();
    PvaClientMonitorDataPtr monitorData = monitor->getData();
    PvaClientPutPtr put = pva->channel(channelName,providerName,2.0)->put("");
    PvaClientPutDataPtr putData = put->getData();
    for(size_t ntimes=0; ntimes<5; ++ntimes)
    {
         double value = ntimes;
         cout << "put " << value << endl;
         putData->putDouble(value); put->put();
         if(!monitor->waitEvent(.1)) {
               cout << "waitEvent returned false. Why???";
               continue;
         } else while(true) {
             cout << "monitor " << monitorData->getDouble() << endl;
             cout << "changed\n";
             monitorData->showChanged(cout);
             cout << "overrun\n";
             monitorData->showOverrun(cout);
             monitor->releaseEvent();
             if(!monitor->poll()) break;
         }
     }
     cout << "__monitor returning\n";
}

static void putGet(PvaClientPtr const &pva)
{

    cout << "helloWorldPutGet\n";
    PvaClientChannelPtr channel = pva->channel("PVRhelloPutGet");
    PvaClientPutGetPtr putGet = channel->createPutGet();
    putGet->connect();
    PvaClientPutDataPtr putData = putGet->getPutData();
    PVStructurePtr arg = putData->getPVStructure();
    PVStringPtr pvValue = arg->getSubField<PVString>("argument.value");
    pvValue->put("World");
    putGet->putGet();
    PvaClientGetDataPtr getData = putGet->getGetData();
    cout << getData->getPVStructure() << endl;
}


int main(int argc,char *argv[])
{
    string provider("pva");
    string channelName("DBRdouble00");
    size_t ntimes(1);
    bool debug(false);
    if(argc==2 && string(argv[1])==string("-help")) {
        cout << "provider channelName ntimes debug" << endl;
        cout << "default" << endl;
        cout << provider << " " <<  channelName << " " << ntimes  << "  " 
             << (debug ? "true" : "false") << endl;
        return 0;
    }
    if(argc>1) provider = argv[1];
    if(argc>2) channelName = argv[2];
    if(argc>3) ntimes = strtoul(argv[3],0,0);
    if(argc>4) {
        string value(argv[4]);
        if(value=="true") debug = true;
    }
    bool pvaSrv(((provider.find("pva")==string::npos) ? false : true));
    bool caSrv(((provider.find("ca")==string::npos) ? false : true));
    cout << "provider \"" << provider << "\""
         << " pvaSrv " << (pvaSrv ? "true" : "false")
         << " caSrv " << (caSrv ? "true" : "false")
         << " channelName " <<  channelName
         << " ntimes " << ntimes
         << " debug " << (debug ? "true" : "false") << endl;

    cout << "_____testRAII starting_______\n";
    try {
        PvaClientPtr pva= PvaClient::get(provider);
        if(debug) PvaClient::setDebug(true);
        for(size_t i=0; i<ntimes ; ++i) {
            if(pvaSrv) {
                getQuick(pva,channelName,provider);
                put(pva,channelName,provider);
                getLongWay(pva,channelName,provider);
                monitor(pva,channelName,provider);
                putGet(pva);
            }
            if(caSrv) {
                getQuick(pva,channelName,"ca");
                put(pva,channelName,"ca");
                getLongWay(pva,channelName,"ca");
                monitor(pva,channelName,"ca");
            }
        }
        cout << "_____testRAII done_______\n";
    } catch (std::runtime_error e) {
        cerr << "exception " << e.what() << endl;
        return 1;
    }
    cout << "after pvaClient looses context\n";
    return 0;
}
