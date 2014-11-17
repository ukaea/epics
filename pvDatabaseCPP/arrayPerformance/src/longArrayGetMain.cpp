/*longArrayGetMain.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
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

#include <arrayPerformance.h>
#include <longArrayGet.h>

using namespace std;
using std::tr1::static_pointer_cast;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvDatabase;

int main(int argc,char *argv[])
{
    string channelName("arrayPerformance");
    int iterBetweenCreateChannel = 0;
    int iterBetweenCreateChannelGet = 0;
    double delayTime = 1.0;
    if(argc==2 && string(argv[1])==string("-help")) {
        cout << "longArrayGetMain channelName ";
        cout << "iterBetweenCreateChannel iterBetweenCreateChannelGet delayTime" << endl;
        cout << "default" << endl;
        cout << "longArrayGetMain " << channelName << " ";
        cout << iterBetweenCreateChannel  << " ";
        cout << iterBetweenCreateChannelGet  << " ";
        cout << delayTime  << endl;
        return 0;
    }
    ClientFactory::start();
    if(argc>1) channelName = argv[1];
    if(argc>2) iterBetweenCreateChannel = strtol(argv[2],0,0);
    if(argc>3) iterBetweenCreateChannelGet = strtol(argv[3],0,0);
    if(argc>4) delayTime = atof(argv[4]);
    cout << "longArrayGetMain " << channelName << " ";
    cout << iterBetweenCreateChannel  << " ";
    cout << iterBetweenCreateChannelGet  << " ";
    cout << delayTime << endl;
    LongArrayGetPtr longArrayGet
         = LongArrayGet::create(
              "pvAccess",
              channelName,
              iterBetweenCreateChannel,
              iterBetweenCreateChannelGet,
              delayTime);
    cout << "longArrayGet\n";
    string str;
    while(true) {
        cout << "Type exit to stop: \n";
        getline(cin,str);
        if(str.compare("exit")==0) break;

    }
    longArrayGet->destroy();
    longArrayGet.reset();
    double xxx = 1.0;
    if(xxx<delayTime) xxx = delayTime;
    ClientFactory::stop();
    epicsThreadSleep(xxx);
    return 0;
}

