/* arrayPerformance.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 * @date 2013.08.08
 */

#include <pv/lock.h>

#define epicsExportSharedSymbols
#include <arrayPerformance.h>



namespace epics { namespace pvDatabase { 

using namespace epics::pvData;
using std::tr1::static_pointer_cast;
using std::tr1::dynamic_pointer_cast;
using std::cout;
using std::endl;
using std::ostringstream;

ArrayPerformancePtr ArrayPerformance::create(
    std::string const & recordName,
        size_t size,
        double delay)
{
    epics::pvData::PVStructurePtr pvStructure =
       epics::pvData::getStandardPVField()->scalarArray(epics::pvData::pvLong,"timeStamp,alarm");
    ArrayPerformancePtr pvRecord(
        new ArrayPerformance(recordName,pvStructure,size,delay));
    if(!pvRecord->init()) pvRecord.reset();
    return pvRecord;
}

ArrayPerformance::ArrayPerformance(
    std::string const & recordName,
    epics::pvData::PVStructurePtr const & pvStructure,
        size_t size,
        double delay)
: PVRecord(recordName,pvStructure),
  size(size),
  delay(delay),
  isDestroyed(false)
{
    pvTimeStamp.attach(pvStructure->getSubField("timeStamp"));
}

ArrayPerformance::~ArrayPerformance()
{
}

bool ArrayPerformance::init()
{
    
    initPVRecord();
    PVLongArrayPtr pvLongArray = getPVStructure()->getSubField<PVLongArray>("value");
    if(!pvLongArray) return false;
    pvValue = pvLongArray;
    ArrayPerformancePtr xxx = dynamic_pointer_cast<ArrayPerformance>(getPtrSelf());
    arrayPerformanceThread = ArrayPerformanceThreadPtr(new ArrayPerformanceThread(xxx));
    arrayPerformanceThread->init();
    return true;
}

void ArrayPerformance::start()
{
    arrayPerformanceThread->start();
}

void ArrayPerformance::process()
{
    timeStamp.getCurrent();
    pvTimeStamp.set(timeStamp);
}

void ArrayPerformance::destroy()
{
    if(isDestroyed) return;
    isDestroyed = true;
    arrayPerformanceThread->destroy();
    arrayPerformanceThread.reset();
    PVRecord::destroy();
}

ArrayPerformanceThread::ArrayPerformanceThread(ArrayPerformancePtr const & arrayPerformance)
: 
  arrayPerformance(arrayPerformance),
  isDestroyed(false),
  runReturned(false),
  threadName("arrayPerformance"),
  value(0)
{}

void ArrayPerformanceThread::init()
{
     thread = std::auto_ptr<epicsThread>(new epicsThread(
        *this,
        threadName.c_str(),
        epicsThreadGetStackSize(epicsThreadStackSmall),
        epicsThreadPriorityHigh));
}

void ArrayPerformanceThread::start()
{
    thread->start();
}

void ArrayPerformanceThread::destroy()
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
    arrayPerformance.reset();
}

void ArrayPerformanceThread::run()
{
    TimeStamp timeStamp;
    TimeStamp timeStampLast;
    timeStampLast.getCurrent();
    int nSinceLastReport = 0;
    while(true) {
        if(arrayPerformance->delay>0.0) epicsThreadSleep(arrayPerformance->delay);
        {
            Lock lock(mutex);
            if(isDestroyed) {
                runReturned = true;
                return;
            }
        }
        timeStamp.getCurrent();
        double diff = TimeStamp::diff(timeStamp,timeStampLast);
        if(diff>=1.0) {
            ostringstream out;
            out << "arrayPerformance value " << value;
            out << " time " << diff ;
            double iterations = nSinceLastReport;
            iterations /= diff;
            if(iterations>10.0e9) {
                 iterations /= 1e9;
                 out << " gigaIterations/sec " << iterations;
            } else if(iterations>10.0e6) {
                 iterations /= 1e6;
                 out << " megaIterations/sec " << iterations;
            } else if(iterations>10.0e3) {
                 iterations /= 1e3;
                 out << " kiloIterations/sec " << iterations;
            } else  {
                 out << " Iterations/sec " << iterations;
            }
            double elementSize = arrayPerformance->size;
            double elementsPerSecond = elementSize*nSinceLastReport;
            elementsPerSecond /= diff;
            if(elementsPerSecond>10.0e9) {
                 elementsPerSecond /= 1e9;
                 out << " gigaElements/sec " << elementsPerSecond;
            } else if(elementsPerSecond>10.0e6) {
                 elementsPerSecond /= 1e6;
                 out << " megaElements/sec " << elementsPerSecond;
            } else if(elementsPerSecond>10.0e3) {
                 elementsPerSecond /= 1e3;
                 out << " kiloElements/sec " << elementsPerSecond;
            } else  {
                 out << " Elements/sec " << elementsPerSecond;
            }
            cout << out.str() << endl;
            timeStampLast = timeStamp;
            nSinceLastReport = 0;
        }
        ++nSinceLastReport;
        arrayPerformance->lock();
        try {
            if(arrayPerformance->getTraceLevel()>1) {
                 cout << "arrayPerformance size " << arrayPerformance->size;
                 cout << " value " << value +1 << endl;
            }
            shared_vector<int64> xxx(arrayPerformance->size,value++);
            shared_vector<const int64> data(freeze(xxx));
                arrayPerformance->beginGroupPut();
                arrayPerformance->pvValue->replace(data);
                arrayPerformance->process();
                arrayPerformance->endGroupPut();
        } catch(...) {
           arrayPerformance->unlock();
           throw;
        }
        arrayPerformance->unlock();
    }
}

}}

