// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution

/*longArrayGetMain.cpp */

/**
 * @author mrk
 * @date 2013.08.10
 */

/* Author: Marty Kraimer */

#include <cstddef>
#include <cstdlib>
#include <cstddef>
#include <string>
#include <cstdio>
#include <memory>
#include <iostream>
#include <vector>

#include <pv/standardField.h>
#include <pv/standardPVField.h>
#include <pv/traceRecord.h>
#include <pv/channelProviderLocal.h>
#include <pv/serverContext.h>
#include <pv/clientFactory.h>

#include <pv/arrayPerformance.h>
#include <pv/longArrayGet.h>

using namespace std;
using std::tr1::static_pointer_cast;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvDatabase;
using namespace epics::exampleCPP::arrayPerformance;


int main(int argc,char *argv[])
{
    string channelName("arrayPerformance");
    int iterBetweenCreateChannel = 0;
    int iterBetweenCreateChannelGet = 0;
    double delayTime = 1.0;
    if(argc==2 && string(argv[1])==string("-help")) {
        cout << "channelName iterBetweenCreateChannel iterBetweenCreateChannelGet delayTime" << endl;
        cout << "default" << endl;
        cout << channelName << " ";
        cout << iterBetweenCreateChannel  << " ";
        cout << iterBetweenCreateChannelGet  << " ";
        cout << delayTime  << endl;
        return 0;
    }
    if(argc>1) channelName = argv[1];
    if(argc>2) iterBetweenCreateChannel = strtol(argv[2],0,0);
    if(argc>3) iterBetweenCreateChannelGet = strtol(argv[3],0,0);
    if(argc>4) delayTime = atof(argv[4]);
    cout << channelName << " ";
    cout << iterBetweenCreateChannel  << " ";
    cout << iterBetweenCreateChannelGet  << " ";
    cout << delayTime << endl;
    try {
        LongArrayGetPtr longArrayGet(new LongArrayGet("pva",
              channelName,
              iterBetweenCreateChannel,
              iterBetweenCreateChannelGet,
              delayTime));
        string str;
        while(true) {
            cout << "Type exit to stop: \n";
            getline(cin,str);
            if(str.compare("exit")==0) {
                 longArrayGet->stop();
epicsThreadSleep(2.0); // should not be necessary
                 exit(0);
            }
        }
    } catch (std::runtime_error e) {
        cout << "exception " << e.what() << endl;
        exit(1);
    }
    return 0;
}

