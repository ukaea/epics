/* arrayPerformance.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 * @date 2013.08.08
 */
#ifndef ARRAYPERFORMANCE_H
#define ARRAYPERFORMANCE_H


#ifdef epicsExportSharedSymbols
#   define arrayperformanceEpicsExportSharedSymbols
#   undef epicsExportSharedSymbols
#endif

#include <epicsThread.h>
#include <pv/standardPVField.h>
#include <pv/timeStamp.h>
#include <pv/pvTimeStamp.h>
#include <pv/pvDatabase.h>

#ifdef arrayperformanceEpicsExportSharedSymbols
#   define epicsExportSharedSymbols
#	undef arrayperformanceEpicsExportSharedSymbols
#   include <shareLib.h>
#endif

namespace epics { namespace pvDatabase { 

class ArrayPerformance;
typedef std::tr1::shared_ptr<ArrayPerformance> ArrayPerformancePtr;

class ArrayPerformanceThread;
typedef std::tr1::shared_ptr<ArrayPerformanceThread> ArrayPerformanceThreadPtr;

class epicsShareClass  ArrayPerformance :
    public PVRecord
{
public:
    POINTER_DEFINITIONS(ArrayPerformance);
    static ArrayPerformancePtr create(
        std::string const & recordName,
        size_t size,
        double delay);
    virtual ~ArrayPerformance();
    virtual bool init();
    virtual void start();
    virtual void process();
    virtual void destroy();
private:
    ArrayPerformance(std::string const & recordName,
        epics::pvData::PVStructurePtr const & pvStructure,
        size_t size,
        double delay);
    size_t size;
    double delay;
    bool isDestroyed;
    epics::pvData::PVLongArrayPtr pvValue;
    epics::pvData::PVTimeStamp pvTimeStamp;
    epics::pvData::TimeStamp timeStamp;
    ArrayPerformanceThreadPtr arrayPerformanceThread;
    friend class ArrayPerformanceThread;
};

class epicsShareClass  ArrayPerformanceThread :
   public epicsThreadRunable
{
public:
    ArrayPerformanceThread(ArrayPerformancePtr const & arrayPerformance);
    virtual ~ArrayPerformanceThread(){};
    void init();
    void start();
    virtual void run();
    void destroy();
private:
    ArrayPerformancePtr arrayPerformance;
    bool isDestroyed;
    bool runReturned;
    std::string threadName;
    epics::pvData::Mutex mutex;
    epics::pvData::int64 value;
    std::auto_ptr<epicsThread> thread;
};


}}

#endif  /* ARRAYPERFORMANCE_H */
