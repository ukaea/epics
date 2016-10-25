/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
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

#include <epicsThread.h>
#include <pv/pvaClient.h>

#ifdef longarraygetEpicsExportSharedSymbols
#   define epicsExportSharedSymbols
#	undef longarraygetEpicsExportSharedSymbols
#   include <shareLib.h>
#endif

namespace epics { namespace exampleCPP { namespace arrayPerformance {


class LongArrayGet;
typedef std::tr1::shared_ptr<LongArrayGet> LongArrayGetPtr;


class epicsShareClass  LongArrayGet :
    public epicsThreadRunable
{
public:
    LongArrayGet(
        std::string  providerName,
        std::string  channelName,
        int iterBetweenCreateChannel,
        int iterBetweenCreateChannelGet,
        double delayTime);
    virtual ~LongArrayGet(){}
    virtual void run();
    void stop();
private:
    std::string  providerName;
    std::string  channelName;
    int iterBetweenCreateChannel;
    int iterBetweenCreateChannelGet;
    double delayTime;
    std::auto_ptr<epicsThread> thread;
    epics::pvData::Event runStop;
    epics::pvData::Event runReturn;
};


}}}

#endif  /* LONGARRAYGET_H */
