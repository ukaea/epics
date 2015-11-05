/*examplePvaClientMonitor.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
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


static void exampleMonitor(PvaClientPtr const &pva,string provider)
{
    PvaClientMonitorPtr monitor = pva->channel("double00",provider,2.0)->monitor("");
    PvaClientMonitorDataPtr monitorData = monitor->getData();
    PvaClientPutPtr put = pva->channel("double00",provider,2.0)->put("");
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
    PvaClientPtr pva = PvaClient::create();
    cout << "exampleMonitor pva\n";
    exampleMonitor(pva,"pva");
    cout << "exampleMonitor ca\n";
    exampleMonitor(pva,"ca");
    cout << "done\n";
    return 0;
}
