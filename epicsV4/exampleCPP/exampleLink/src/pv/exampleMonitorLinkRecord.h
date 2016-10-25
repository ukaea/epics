/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 * @date 2016.06.17
 */
#ifndef EXAMPLEMONITORLINKRECORD_H
#define EXAMPLEMONITORLINKRECORD_H

#ifdef epicsExportSharedSymbols
#   define exampleMonitorLinkEpicsExportSharedSymbols
#   undef epicsExportSharedSymbols
#endif

#include <pv/timeStamp.h>
#include <pv/pvTimeStamp.h>
#include <pv/alarm.h>
#include <pv/pvAlarm.h>
#include <pv/pvDatabase.h>
#include <pv/pvCopy.h>
#include <pv/pvaClient.h>

#ifdef exampleMonitorLinkEpicsExportSharedSymbols
#   define epicsExportSharedSymbols
#	undef exampleMonitorLinkEpicsExportSharedSymbols
#endif

#include <shareLib.h>

namespace epics { namespace exampleCPP { namespace exampleLink {


class ExampleMonitorLinkRecord;
typedef std::tr1::shared_ptr<ExampleMonitorLinkRecord> ExampleMonitorLinkRecordPtr;
typedef std::tr1::weak_ptr<ExampleMonitorLinkRecord> ExampleMonitorLinkRecordWPtr;


class epicsShareClass ExampleMonitorLinkRecord :
    public epics::pvDatabase::PVRecord,
    public epics::pvaClient::PvaClientMonitorRequester
{
public:
    POINTER_DEFINITIONS(ExampleMonitorLinkRecord);
    static ExampleMonitorLinkRecordPtr create(
        epics::pvaClient::PvaClientPtr const &pva,
        std::string const & recordName,
        std::string const & providerName,
        std::string const & channelName
        );
    virtual ~ExampleMonitorLinkRecord() {}
    virtual void process();
    void event(epics::pvaClient::PvaClientMonitorPtr const & monitor);
    virtual bool init() {return false;}
    bool init(
        epics::pvaClient::PvaClientPtr const & pva,
        std::string const & channelName,
        std::string const & providerName
        );
private:
    ExampleMonitorLinkRecord(
        std::string const & recordName,
        epics::pvData::PVStructurePtr const & pvStructure);
    epics::pvData::PVDoubleArrayPtr pvValue;
    epics::pvaClient::PvaClientMonitorRequester::shared_pointer  monitorRequester;
    epics::pvaClient::PvaClientMonitorPtr pvaClientMonitor;
};

}}}

#endif  /* EXAMPLEMONITORLINKRECORD_H */
