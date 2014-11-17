/* longArrayGet.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 * @date 2013.08.09
 */
#ifndef LONGARRAYGET_H
#define LONGARRAYGET_H


#ifdef epicsExportSharedSymbols
#   define longarraygetEpicsExportSharedSymbols
#   undef epicsExportSharedSymbols
#endif

#include <pv/event.h>
#include <pv/lock.h>
#include <pv/standardPVField.h>
#include <pv/timeStamp.h>
#include <pv/pvTimeStamp.h>
#include <pv/pvAccess.h>


#ifdef longarraygetEpicsExportSharedSymbols
#   define epicsExportSharedSymbols
#	undef longarraygetEpicsExportSharedSymbols
#   include <shareLib.h>
#endif

namespace epics { namespace pvDatabase { 


class LongArrayGet;
typedef std::tr1::shared_ptr<LongArrayGet> LongArrayGetPtr;


class LongArrayChannelGet;
typedef std::tr1::shared_ptr<LongArrayChannelGet> LongArrayChannelGetPtr;

class epicsShareClass  LongArrayGet :
    public std::tr1::enable_shared_from_this<LongArrayGet>
{
public:
    POINTER_DEFINITIONS(LongArrayGet);
    static LongArrayGetPtr create(
        std::string const & providerName,
        std::string const & channelName,
        int iterBetweenCreateChannel = 0,
        int iterBetweenCreateChannelGet = 0,
        double delayTime = 0.0);
    ~LongArrayGet();
    void destroy();
private:
    LongArrayGetPtr getPtrSelf()
    {
        return shared_from_this();
    }
    LongArrayGet(
        std::string const & providerName,
        std::string const & channelName,
        int iterBetweenCreateChannel = 0,
        int iterBetweenCreateChannelGet = 0,
        double delayTime = 0.0);
    bool init();

    std::string providerName;
    std::string channelName;
    int iterBetweenCreateChannel;
    int iterBetweenCreateChannelGet;
    double delayTime;
    LongArrayChannelGetPtr longArrayChannelGet;
};


}}

#endif  /* LONGARRAYGET_H */
