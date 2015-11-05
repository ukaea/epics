/* exampleLink.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 * @date 2013.08.02
 */
#ifndef EXAMPLEPVADOUBLEARRAYGET_H
#define EXAMPLEPVADOUBLEARRAYGET_H

#ifdef epicsExportSharedSymbols
#   define exampleLinkEpicsExportSharedSymbols
#   undef epicsExportSharedSymbols
#endif

#include <pv/timeStamp.h>
#include <pv/pvTimeStamp.h>
#include <pv/alarm.h>
#include <pv/pvAlarm.h>
#include <pv/pvDatabase.h>
#include <pv/pvCopy.h>
#include <pv/pvAccess.h>
#include <pv/serverContext.h>

#ifdef exampleLinkEpicsExportSharedSymbols
#   define epicsExportSharedSymbols
#	undef exampleLinkEpicsExportSharedSymbols
#endif

#include <shareLib.h>

namespace epics { namespace pvDatabase { 


class ExampleLink;
typedef std::tr1::shared_ptr<ExampleLink> ExampleLinkPtr;

class epicsShareClass ExampleLink :
    public PVRecord,
    public epics::pvAccess::ChannelRequester,
    public epics::pvAccess::ChannelGetRequester
{
public:
    POINTER_DEFINITIONS(ExampleLink);
    static ExampleLinkPtr create(
        std::string const & recordName,
        std::string const & providerName,
        std::string const & channelName
        );
    virtual ~ExampleLink() {}
    virtual void destroy();
    virtual bool init();
    virtual void process();
    virtual void channelCreated(
        const epics::pvData::Status& status,
        epics::pvAccess::Channel::shared_pointer const & channel);
    virtual void channelStateChange(
        epics::pvAccess::Channel::shared_pointer const & channel,
        epics::pvAccess::Channel::ConnectionState connectionState);
    virtual void channelGetConnect(
        const epics::pvData::Status& status,
        epics::pvAccess::ChannelGet::shared_pointer const & channelGet,
        epics::pvData::StructureConstPtr const & structure);
    virtual void getDone(
        const epics::pvData::Status& status,
        epics::pvAccess::ChannelGet::shared_pointer const & channelGet,
        epics::pvData::PVStructurePtr const & pvStructure,
        epics::pvData::BitSetPtr const &bitSet);
    virtual std::string getRequesterName() {return channelName;}
    virtual void message(
        std::string const & message,
        epics::pvData::MessageType messageType)
        {
           std::cout << "Why is ExampleLink::message called\n";
        }
private:
    ExampleLink(std::string const & recordName,
        std::string providerName,
        std::string channelName,
        epics::pvData::PVStructurePtr const & pvStructure);
    std::string providerName;
    std::string channelName;
    epics::pvData::PVDoubleArrayPtr pvValue;
    epics::pvData::PVTimeStamp pvTimeStamp;
    epics::pvData::TimeStamp timeStamp;
    epics::pvData::PVAlarm pvAlarm;
    epics::pvData::Alarm alarm;
    epics::pvAccess::Channel::shared_pointer channel;
    epics::pvAccess::ChannelGet::shared_pointer channelGet;
    epics::pvData::Event event;
    epics::pvData::Status status;
    epics::pvData::PVStructurePtr getPVStructure;
    epics::pvData::BitSetPtr bitSet;
    epics::pvData::PVDoubleArrayPtr getPVValue;
};

}}

#endif  /* EXAMPLEPVADOUBLEARRAYGET_H */
