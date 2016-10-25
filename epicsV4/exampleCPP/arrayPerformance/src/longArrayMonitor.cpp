/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 * @date 2013.08.09
 */

#include <epicsThread.h>
#include <pv/pvaClient.h>

#define epicsExportSharedSymbols
#include <pv/longArrayMonitor.h>


namespace epics { namespace exampleCPP { namespace arrayPerformance { 

using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvaClient;
using namespace std;

LongArrayMonitor::LongArrayMonitor(
   string  const &providerName,
    string  const & channelName,
    int queueSize)
: 
  providerName(providerName),
  channelName(channelName),
  queueSize(queueSize)
{
     thread = std::auto_ptr<epicsThread>(new epicsThread(
        *this,
        "longArrayMonitor",
        epicsThreadGetStackSize(epicsThreadStackSmall),
        epicsThreadPriorityLow));
     thread->start();
}

void LongArrayMonitor::stop()
{
    runStop.signal();
    runReturn.wait();
}

void LongArrayMonitor::run()
{
    PvaClientPtr pva(PvaClient::get("pva"));
    string  request("record[queueSize=");
    char buff[20];
    sprintf(buff,"%d",queueSize);
    request += buff;
    request += "]field(value,timeStamp,alarm)";
    PvaClientMonitorPtr monitor = pva->channel(channelName,providerName,2.0)->monitor(request);
    TimeStamp timeStamp;
    TimeStamp timeStampLast;
    timeStampLast.getCurrent();
    long nElements = 0;
    long nSinceLastReport = 0;
    int64 first = 0;
    int64 last = 0;
    while(true) {   
        if(runStop.tryWait()) {
             runReturn.signal();
             return;
        }    
        if(!monitor->waitEvent(0.0)) {
	    cout << "waitEvent returned false. Why???" << endl;
            continue;
        }
        PvaClientMonitorDataPtr pvaData = monitor->getData();
    	PVStructurePtr pvStructure = pvaData->getPVStructure();
    	PVLongArrayPtr pvValue = pvStructure->getSubField<PVLongArray>("value");
        size_t len = pvValue->getLength();
        if(len>0) {
            shared_vector<const int64> data = pvValue->view();
            first = data[0];
            last = data[len-1];
            if(first!=last) cout << "error first=" << first << " last=" << last << endl;
       } else {
            cout << "len is 0" << endl;
       }
       nElements += len;
       timeStamp.getCurrent();
       double diff = TimeStamp::diff(timeStamp,timeStampLast);
       if(diff>=1.0) {
            ostringstream out;
            out << " monitors/sec " << nSinceLastReport << " ";
            if(len>0) out << "first " << first << " last " << last ;
            out << " changed " << *pvaData->getChangedBitSet();
            out << " overrun " << *pvaData->getOverrunBitSet();
            double elementsPerSec = nElements;
            elementsPerSec /= diff;
            if(elementsPerSec>10.0e9) {
                 elementsPerSec /= 1e9;
                 out << " gigaElements/sec " << elementsPerSec;
            } else if(elementsPerSec>10.0e6) {
                 elementsPerSec /= 1e6;
                 out << " megaElements/sec " << elementsPerSec;
            } else if(elementsPerSec>10.0e3) {
                 elementsPerSec /= 1e3;
                 out << " kiloElements/sec " << elementsPerSec;
            } else  {
                 out << " Elements/sec " << elementsPerSec;
            }
            cout << out.str() << endl;
            timeStampLast = timeStamp;
            nSinceLastReport = 0;
            nElements = 0;
        }
        ++nSinceLastReport;
        monitor->releaseEvent();
    }
}

}}}


