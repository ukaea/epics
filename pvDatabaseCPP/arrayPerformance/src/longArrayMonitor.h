/* longArrayMonitor.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
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

#include <pv/event.h>
#include <pv/lock.h>
#include <pv/standardPVField.h>
#include <pv/timeStamp.h>
#include <pv/pvTimeStamp.h>
#include <pv/pvAccess.h>

#ifdef longarraymonitorEpicsExportSharedSymbols
#   define epicsExportSharedSymbols
#	undef longarraymonitorEpicsExportSharedSymbols
#   include <shareLib.h>
#endif


namespace epics { namespace pvDatabase { 


class LongArrayMonitor;
typedef std::tr1::shared_ptr<LongArrayMonitor> LongArrayMonitorPtr;

class LAMChannelRequester;
typedef std::tr1::shared_ptr<LAMChannelRequester> LAMChannelRequesterPtr;

class LAMMonitorRequester;
typedef std::tr1::shared_ptr<LAMMonitorRequester> LAMMonitorRequesterPtr;

class epicsShareClass  LongArrayMonitor :
    public std::tr1::enable_shared_from_this<LongArrayMonitor>
{
public:
    POINTER_DEFINITIONS(LongArrayMonitor);
    static LongArrayMonitorPtr create(
        std::string const & providerName,
        std::string const & channelName,
        int queueSize = 1,
        double waitTime = 0.0);
    ~LongArrayMonitor();
    void start();
    void stop();
    void destroy();
private:
    static epics::pvData::Mutex printMutex;
    bool init(
        std::string const & providerName,
        std::string const & channelName,
        int queueSize,
        double waitTime);
    LongArrayMonitorPtr getPtrSelf()
    {
        return shared_from_this();
    }
    LongArrayMonitor();

    LAMChannelRequesterPtr channelRequester;
    LAMMonitorRequesterPtr monitorRequester;
    epics::pvAccess::Channel::shared_pointer channel;
    epics::pvData::Monitor::shared_pointer monitor;
    epics::pvData::Event event;
    epics::pvData::Status status;
    friend class LAMChannelRequester;
    friend class LAMMonitorRequester;
};


}}

#endif  /* LONGARRAYMONITOR_H */
