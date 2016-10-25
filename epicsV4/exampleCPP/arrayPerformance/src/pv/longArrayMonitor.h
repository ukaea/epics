/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 * @date 2013.08.09
 */
#ifndef LONGARRAYMONITOR_H
#define LONGARRAYMONITOR_H

#ifdef epicsExportSharedSymbols
#   define longarraymonitorEpicsExportSharedSymbols
#   undef epicsExportSharedSymbols
#endif

#include <epicsThread.h>
#include <pv/pvaClient.h>


#ifdef longarraymonitorEpicsExportSharedSymbols
#   define epicsExportSharedSymbols
#	undef longarraymonitorEpicsExportSharedSymbols
#   include <shareLib.h>
#endif


namespace epics { namespace exampleCPP { namespace arrayPerformance {


class LongArrayMonitor;
typedef std::tr1::shared_ptr<LongArrayMonitor> LongArrayMonitorPtr;

class epicsShareClass  LongArrayMonitor :
    public epicsThreadRunable
{
public:
    LongArrayMonitor(
        std::string const & providerName,
        std::string const & channelName,
        int queueSize = 2);
    virtual void run();
    void stop();
private:
    std::string providerName;
    std::string channelName;
    int queueSize;
    std::auto_ptr<epicsThread> thread;
    epics::pvData::Event runStop;
    epics::pvData::Event runReturn;
};


}}}

#endif  /* LONGARRAYMONITOR_H */
