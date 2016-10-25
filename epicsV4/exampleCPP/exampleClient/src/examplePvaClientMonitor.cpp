/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 */

/* Author: Marty Kraimer */

#include <epicsThread.h>

#include <iostream>

#include <pv/pvaClient.h>

using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvaClient;


static void exampleMonitor(PvaClientPtr const &pva,string const & recordName,string const& provider)
{
    cout << "__exampleDouble recordName " << recordName << " provider " << provider << "__\n";
    PvaClientMonitorPtr monitor = pva->channel(recordName,provider,2.0)->monitor("");
    PvaClientMonitorDataPtr monitorData = monitor->getData();
    PvaClientPutPtr put = pva->channel(recordName,provider,2.0)->put("");
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
}


int main(int argc,char *argv[])
{
    cout << "_____examplePvaClientMonitor starting_______\n";
    try {
        PvaClientPtr pva = PvaClient::get("pva ca");
//PvaClient::setDebug(true);
        exampleMonitor(pva,"PVRdouble","pva");
        PvaClientChannelPtr pvaChannel = pva->createChannel("DBRdouble00","ca");
        pvaChannel->issueConnect();
        Status status = pvaChannel->waitConnect(1.0);
        if(status.isOK()) {
            cout << "exampleMonitor pva\n";
            exampleMonitor(pva,"DBRdouble00","pva");
            cout << "exampleMonitor ca\n";
            exampleMonitor(pva,"DBRdouble00","ca");
        } else {
             cout << "DBRdouble00 not found\n";
        }
        cout << "_____examplePvaClientMonitor done_______\n";
    } catch (std::runtime_error e) {
            cerr << "exception " << e.what() << endl;
            return 1;
    }
    return 0;
}
