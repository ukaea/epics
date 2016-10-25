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


int main(int argc,char *argv[])
{
    PvaClientPtr pva = PvaClient::get("pva");
    try {
        PvaClientMonitorPtr monitor = pva->channel("powerSupply")->monitor("");
        while(true) {
            if(!monitor->waitEvent()) {
               cout << "waitEvent returned false. Why???";
               continue;
            }
            PvaClientMonitorDataPtr monitorData = monitor->getData();
            cout << " changed " << *monitorData->getChangedBitSet() << endl;
            monitorData->showChanged(cout);
            cout << " overrun " << *monitorData->getOverrunBitSet() << endl;
            monitorData->showOverrun(cout);
            monitor->releaseEvent();
        }
    } catch (std::runtime_error e) {
            cout << "exception " << e.what() << endl;
            exit(1);
    }
    return 0;
}
