/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
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

#include <epicsExit.h>

#include <pv/standardField.h>
#include <pv/standardPVField.h>
#include <pv/traceRecord.h>
#include <pv/channelProviderLocal.h>
#include <pv/serverContext.h>
#include <pv/clientFactory.h>

#include <pv/arrayPerformance.h>
#include <pv/longArrayMonitor.h>

using namespace std;
using std::tr1::static_pointer_cast;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvDatabase;
using namespace epics::exampleCPP::arrayPerformance;


int main(int argc,char *argv[])
{
    string channelName("arrayPerformance");
    int queueSize = 2;
    if(argc==2 && string(argv[1])==string("-help")) {
        cout << "channelName queueSize" << endl;
        cout << "default" << endl;
        cout << channelName << " ";
        cout << queueSize  << endl;
        return 0;
    }
    if(argc>1) channelName = argv[1];
    if(argc>2) queueSize = strtol(argv[2],0,0);
    try {
        LongArrayMonitorPtr longArrayMonitor(new LongArrayMonitor("pva",channelName,queueSize));
        string str;
        while(true) {
            cout << "Type exit to stop: \n";
            getline(cin,str);
            if(str.compare("exit")==0) {
                 longArrayMonitor->stop();
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

