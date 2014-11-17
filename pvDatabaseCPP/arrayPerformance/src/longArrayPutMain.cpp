/*longArrayPutMain.cpp */
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
#include <longArrayPut.h>

using namespace std;
using std::tr1::static_pointer_cast;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvDatabase;

int main(int argc,char *argv[])
{
    string channelName("arrayPerformance");
    size_t arraySize = 10;
    int iterBetweenCreateChannel = 0;
    int iterBetweenCreateChannelPut = 0;
    double delayTime = 1.0;
    if(argc==2 && string(argv[1])==string("-help")) {
        cout << "longArrayPutMain channelName arraySize ";
        cout << "iterBetweenCreateChannel iterBetweenCreateChannelPut delayTime" << endl;
        cout << "default" << endl;
        cout << "longArrayPutMain " << channelName << " ";
        cout << arraySize << " ";
        cout << iterBetweenCreateChannel  << " ";
        cout << iterBetweenCreateChannelPut  << " ";
        cout << delayTime  << endl;
        return 0;
    }
    ClientFactory::start();
    if(argc>1) channelName = argv[1];
    if(argc>2) arraySize = strtoul(argv[2],0,0);
    if(argc>3) iterBetweenCreateChannel = strtol(argv[3],0,0);
    if(argc>4) iterBetweenCreateChannelPut = strtol(argv[4],0,0);
    if(argc>5) delayTime = atof(argv[5]);
    cout << "longArrayPutMain " << channelName << " ";
    cout << arraySize << " ";
    cout << iterBetweenCreateChannel  << " ";
    cout << iterBetweenCreateChannelPut  << " ";
    cout << delayTime << endl;
    LongArrayPutPtr longArrayPut
         = LongArrayPut::create(
              "pvAccess",
              channelName,
              arraySize,
              iterBetweenCreateChannel,
              iterBetweenCreateChannelPut,
              delayTime);
    cout << "longArrayPut\n";
    string str;
    while(true) {
        cout << "Type exit to stop: \n";
        getline(cin,str);
        if(str.compare("exit")==0) break;

    }
    longArrayPut->destroy();
    longArrayPut.reset();
    double xxx = 1.0;
    if(xxx<delayTime) xxx = delayTime;
    epicsThreadSleep(xxx);
    ClientFactory::stop();
    epicsThreadSleep(1.0);
    return 0;
}

