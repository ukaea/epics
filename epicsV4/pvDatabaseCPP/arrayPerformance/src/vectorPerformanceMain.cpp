/*vectorPerformanceMain.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 * @date 2013.09.02
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

#include <epicsThread.h>
#include <pv/timeStamp.h>


using namespace std;
using std::tr1::static_pointer_cast;
using namespace epics::pvData;

class VectorPerformanceThread;
typedef std::tr1::shared_ptr<VectorPerformanceThread> VectorPerformanceThreadPtr;

class VectorPerformanceThread :
   public epicsThreadRunable
{
public:
    VectorPerformanceThread(int threadNumber,size_t size,double delay);
    virtual ~VectorPerformanceThread(){};
    void init();
    void start();
    virtual void run();
    void destroy();
private:
    bool isDestroyed;
    bool runReturned;
    int threadNumber;
    size_t size;
    epics::pvData::int64 value;
    double delay;
    Mutex mutex;
    std::vector<int64> vector;
    std::auto_ptr<epicsThread> thread;
};

VectorPerformanceThread::VectorPerformanceThread(
    int threadNumber,size_t size,double delay)
:
  isDestroyed(false),
  runReturned(false),
  threadNumber(threadNumber),
  size(size),
  value(0),
  delay(delay)
{}

void VectorPerformanceThread::init()
{
     vector.resize(size);
     thread = std::auto_ptr<epicsThread>(new epicsThread(
        *this,
        "vectorPerform",
        epicsThreadGetStackSize(epicsThreadStackSmall),
        epicsThreadPriorityHigh));
}

void VectorPerformanceThread::start()
{
     thread->start();
}

void VectorPerformanceThread::destroy()
{
    Lock lock(mutex);
    if(isDestroyed) return;
    isDestroyed = true;
    while(true) {
        if(runReturned) break;
        lock.unlock();
        epicsThreadSleep(.01);
        lock.lock();
    }
    thread->exitWait();
    thread.reset();
}


void VectorPerformanceThread::run()
{
    TimeStamp timeStamp;
    TimeStamp timeStampLast;
    timeStampLast.getCurrent();
    int nSinceLastReport = 0;
    while(true) {
        if(delay>0.0) epicsThreadSleep(delay);
        {
            Lock lock(mutex);
            if(isDestroyed) {
cout << " found isDestroyed " << threadNumber << endl;
                runReturned = true;
                return;
            }
        }
        timeStamp.getCurrent();
        double diff = TimeStamp::diff(timeStamp,timeStampLast);
        if(diff>=1.0) {
            cout << "thread" << threadNumber;
            cout << " value " << value;
            cout << " time " << diff;
            double iterations = nSinceLastReport;
            iterations /= diff;
            cout << " iterations/sec " << iterations;
            double elementSize = size;
            double elementsPerSecond = elementSize*nSinceLastReport;
            elementsPerSecond /= diff;
            elementsPerSecond /= 1e6;
            cout << " elements/sec " << elementsPerSecond << "million" << endl;
            cout.flush();
            timeStampLast = timeStamp;
            nSinceLastReport = 0;
        }
        ++nSinceLastReport;
        ++value;
        for(size_t i=0; i<size; ++i) vector[i] = value;
    }
}

int main(int argc,char *argv[])
{
    size_t size = 50000000;
    double delay = .01;
    size_t nThread = 1;
    if(argc==2 && string(argv[1])==string("-help")) {
        cout << "vectorPerformanceMain size delay nThread" << endl;
        cout << "default" << endl;
        cout << "vectorPerformance ";
        cout << size << " ";
        cout << delay << " ";
        cout << nThread << " ";
        cout << endl;
        return 0;
    }
    if(argc>1) size = strtoul(argv[1],0,0);
    if(argc>2) delay = atof(argv[2]);
    if(argc>3) nThread = strtoul(argv[3],0,0);
    cout << "vectorPerformance ";
    cout << size << " ";
    cout << delay << " ";
    cout << nThread << " ";
    cout << endl;
    cout << "vectorPerformance\n";
    std::vector<VectorPerformanceThreadPtr> threads;
    for(size_t i=0; i<nThread; ++i) {
        threads.push_back(
           VectorPerformanceThreadPtr(
               new VectorPerformanceThread(i,size,delay)));
    }
    epicsThreadSleep(.1);
    for(size_t i=0; i<nThread; ++i) {
        threads[i]->init();
    }
    for(size_t i=0; i<nThread; ++i) {
        threads[i]->start();
    }
    string str;
    while(true) {
        cout << "Type exit to stop: \n";
        getline(cin,str);
        if(str.compare("exit")==0) break;

    }
    for(size_t i=0; i<nThread; ++i) {
cout << "" << i << " calling destroy" << endl;
        threads[i]->destroy();
        threads[i].reset();
    }
    return 0;
}

