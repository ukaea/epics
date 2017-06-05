/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 */

/* Author: Marty Kraimer */

#include <iostream>

#include <pv/pvaClient.h>

using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvaClient;


static void exampleProcess(PvaClientPtr const &pva)
{
    cout << "example process\n";
    PvaClientChannelPtr channel = pva->channel("PVRdouble");
    PvaClientProcessPtr process = channel->createProcess();
    process->process();
    channel->get("field()")->getData()->showChanged(cout) << endl;
    process->process();
    channel->get("field()")->getData()->showChanged(cout) << endl;
}


int main(int argc,char *argv[])
{
    
    try {
        PvaClientPtr pva = PvaClient::get("pva");
        exampleProcess(pva);
    } catch (std::runtime_error e) {
        cerr << "exception " << e.what() << endl;
        return 1;
    }
    return 0;
}
