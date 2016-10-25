/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 * @date 2013.08.09
 */
#ifndef LONGARRAYPUT_H
#define LONGARRAYPUT_H


#ifdef epicsExportSharedSymbols
#   define longarrayputEpicsExportSharedSymbols
#   undef epicsExportSharedSymbols
#endif

#include <epicsThread.h>
#include <pv/pvaClient.h>

#ifdef longarrayputEpicsExportSharedSymbols
#   define epicsExportSharedSymbols
#	undef longarrayputEpicsExportSharedSymbols
#   include <shareLib.h>
#endif

namespace epics { namespace exampleCPP { namespace arrayPerformance {


class LongArrayPut;
typedef std::tr1::shared_ptr<LongArrayPut> LongArrayPutPtr;


class epicsShareClass  LongArrayPut :
    public epicsThreadRunable
{
public:
    LongArrayPut(
        std::string const & providerName,
        std::string const & channelName,
        size_t arraySize = 100,
        int iterBetweenCreateChannel = 0,
        int iterBetweenCreateChannelPut = 0,
        double delayTime = 0.0);
    virtual void run();
    void stop();
private:
    std::string providerName;
    std::string channelName;
    size_t arraySize;
    int iterBetweenCreateChannel;
    int iterBetweenCreateChannelPut;
    double delayTime;
    std::auto_ptr<epicsThread> thread;
    epics::pvData::Event runStop;
    epics::pvData::Event runReturn;
};


}}}

#endif  /* LONGARRAYPUT_H */
