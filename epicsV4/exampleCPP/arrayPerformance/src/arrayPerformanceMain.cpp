// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution

/*arrayPerformanceMain.cpp */

/**
 * @author mrk
 * @date 2013.08.08
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
#include <pv/longArrayMonitor.h>

using namespace std;
using std::tr1::static_pointer_cast;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvaClient;
using namespace epics::pvDatabase;
using namespace epics::exampleCPP::arrayPerformance;

int main(int argc,char *argv[])
{
    string recordName;
    recordName = "arrayPerformance";
    size_t size = 10000000;
    double delay = .0001;
    string providerName("local");
    size_t nMonitor = 1;
    int queueSize = 2;

    if(argc==2 && string(argv[1])==string("-help")) {
        cout << "recordName size";
        cout << " delay providerName nMonitor queueSize" << endl;
        cout << "default" << endl;
        cout << recordName << " ";
        cout << size << " ";
        cout << delay << " ";
        cout << providerName << " ";
        cout << nMonitor << " ";
        cout << queueSize << endl;
        return 0;
    }
    if(argc>1) recordName = argv[1];
    if(argc>2) size = strtoul(argv[2],0,0);
    if(argc>3) delay = atof(argv[3]);
    if(argc>4) providerName = argv[4];
    if(argc>5) nMonitor = strtoul(argv[5],0,0);
    if(argc>6) queueSize = strtol(argv[6],0,0);
    cout << recordName << " ";
    cout << size << " ";
    cout << delay << " ";
    cout << providerName << " ";
    cout << nMonitor << " ";
    cout << queueSize << endl;
    
    try {
        PVDatabasePtr master = PVDatabase::getMaster();
        
        ChannelProviderLocalPtr channelProvider = getChannelProviderLocal();
        ServerContext::shared_pointer ctx = 
        startPVAServer("local",0,true,true);
        ArrayPerformancePtr arrayPerformance =ArrayPerformance::create(recordName,size,delay);
        master->addRecord(arrayPerformance);
        arrayPerformance->startThread();
        std::vector<LongArrayMonitorPtr> longArrayMonitor(nMonitor);
        for(size_t i=0; i<nMonitor; ++i) {
           longArrayMonitor[i] = LongArrayMonitorPtr(
               new LongArrayMonitor(providerName,recordName,queueSize));
        }
        
        cout << "arrayPerformance\n";
        string str;
        while(true) {
            cout << "Type exit to stop: \n";
            getline(cin,str);
            if(str.compare("exit")==0) break;
        }
        for(size_t i=0; i<nMonitor; ++i) {
            longArrayMonitor[i]->stop();
        }
        arrayPerformance->stop();
        ctx->destroy();
    } catch (std::runtime_error e) {
        cerr << "exception " << e.what() << endl;
        return 1;
    }
    return 0;
}

