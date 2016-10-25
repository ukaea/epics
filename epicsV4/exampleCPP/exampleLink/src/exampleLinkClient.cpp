/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */


/**
 * @author mrk
 */


#include <iostream>

#include <pv/pvaClient.h>

using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvaClient;

int main(int argc,char *argv[])
{
    string provider("pva");
    if(argc==2 && string(argv[1])==string("-help")) {
        cout << "provider" << endl;
        cout << "default" << endl;
        cout << provider  << endl;
        return 0;
    }
    if(argc>1) provider = argv[1];
    cout << "_____exampleLinkClient starting_______\n";
    string providers("pva");
    if(provider=="ca") providers = "pva ca";
    PvaClientPtr pva = PvaClient::get(providers);
    try {
        PvaClientPutPtr put = pva->channel("doubleArray",provider,5.0)->put();
        PvaClientPutDataPtr putData = put->getData();
        PvaClientMonitorPtr monitor = pva->channel("exampleMonitorLink")->monitor("");
        PvaClientMonitorDataPtr pvaData = monitor->getData();
        if(!monitor->waitEvent(0.0)) {
            cout << "waitEvent returned false. Why???\n";
        } else {
            cout << "exampleMonitorLink\n" << pvaData->getPVStructure() << endl;
            monitor->releaseEvent();
        }
        {
             size_t nElements = 5;
             shared_vector<double> value(nElements);
             for(size_t i=0; i< nElements; ++i) value[i] = 0.0;
             shared_vector<const double> data(freeze(value));
             putData->putDoubleArray(data); put->put();
        }
        if(!monitor->waitEvent(0.0)) {
            cout<< "waitEvent returned false. Why???\n";
        } else {
            cout << "exampleMonitorLink\n" << pvaData->getPVStructure() << endl;
            monitor->releaseEvent();
        }
        {
             size_t nElements = 5;
             shared_vector<double> value(nElements);
             for(size_t i=0; i< nElements; ++i) value[i] = .1*(i+1);
             shared_vector<const double> data(freeze(value));
             putData->putDoubleArray(data); put->put();
        }
        if(!monitor->waitEvent(0.0)) {
            cout<< "waitEvent returned false. Why???\n";
        } else {
            cout << "exampleMonitorLink\n" << pvaData->getPVStructure() << endl;
            monitor->releaseEvent();
        }
    } catch (std::runtime_error e) {
        cout << "exception " << e.what() << endl;
        return 1;
    }
    cout << "_____exampleLinkClient done_______\n";
    return 0;
}

