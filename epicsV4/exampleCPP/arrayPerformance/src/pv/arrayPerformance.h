/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
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
#include <pv/event.h>
#include <pv/standardPVField.h>
#include <pv/timeStamp.h>
#include <pv/pvTimeStamp.h>
#include <pv/pvDatabase.h>

#ifdef arrayperformanceEpicsExportSharedSymbols
#   define epicsExportSharedSymbols
#	undef arrayperformanceEpicsExportSharedSymbols
#   include <shareLib.h>
#endif

namespace epics { namespace exampleCPP { namespace arrayPerformance { 

class ArrayPerformance;
typedef std::tr1::shared_ptr<ArrayPerformance> ArrayPerformancePtr;

class epicsShareClass  ArrayPerformance :
    public epics::pvDatabase::PVRecord,
    public epicsThreadRunable
{
public:
    static ArrayPerformancePtr create(
        std::string const & recordName,
        size_t size,
        double delay);
    virtual void process();
    virtual bool init() { return false;}
    virtual void run();
    void startThread();
    void stop();
private:
     ArrayPerformance(std::string const & recordName,
        epics::pvData::PVStructurePtr const & pvStructure,
        size_t size,
        double delay);
    size_t size;
    double delay;
    std::auto_ptr<epicsThread> thread;
    epics::pvData::Event runStop;
    epics::pvData::Event runReturn;
};

}}}

#endif  /* ARRAYPERFORMANCE_H */
