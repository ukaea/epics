// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution

/* exampleLinkRecord.h */

/**
 * @author mrk
 * @date 2013.08.02
 */
#ifndef EXAMPLELINKRECORD_H
#define EXAMPLELINKRECORD_H

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
#include <pv/pvaClient.h>

#ifdef exampleLinkEpicsExportSharedSymbols
#   define epicsExportSharedSymbols
#	undef exampleLinkEpicsExportSharedSymbols
#endif

#include <shareLib.h>

namespace epics { namespace exampleCPP { namespace exampleLink {


class ExampleLinkRecord;
typedef std::tr1::shared_ptr<ExampleLinkRecord> ExampleLinkRecordPtr;
typedef std::tr1::weak_ptr<ExampleLinkRecord> ExampleLinkRecordWPtr;


class epicsShareClass ExampleLinkRecord :
    public epics::pvDatabase::PVRecord,
    public epics::pvaClient::PvaClientMonitorRequester
{
public:
    POINTER_DEFINITIONS(ExampleLinkRecord);
    static ExampleLinkRecordPtr create(
        epics::pvaClient::PvaClientPtr const &pva,
        std::string const & recordName,
        std::string const & providerName,
        std::string const & channelName
        );
    virtual ~ExampleLinkRecord() {}
    virtual void process();
    void event(epics::pvaClient::PvaClientMonitorPtr const & monitor);
private:
    ExampleLinkRecord(
        std::string const & recordName,
        epics::pvData::PVStructurePtr const & pvStructure);
    bool init(
        epics::pvaClient::PvaClientPtr const & pva,
        std::string const & channelName,
        std::string const & providerName
        );
    epics::pvData::PVDoubleArrayPtr pvValue;
    epics::pvaClient::PvaClientMonitorRequester::shared_pointer  monitorRequester;
    epics::pvaClient::PvaClientMonitorPtr pvaClientMonitor;
};

}}}

#endif  /* EXAMPLELINKRECORD_H */
