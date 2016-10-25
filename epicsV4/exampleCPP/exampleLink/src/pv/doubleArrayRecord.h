/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 * @date 2016.06.17
 */
#ifndef DOUBLEARRAYRECORD_H
#define DOUBLEARRAYRECORD_H

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


class DoubleArrayRecord;
typedef std::tr1::shared_ptr<DoubleArrayRecord> DoubleArrayRecordPtr;
typedef std::tr1::weak_ptr<DoubleArrayRecord> DoubleArrayRecordWPtr;


class epicsShareClass DoubleArrayRecord :
    public epics::pvDatabase::PVRecord
{
public:
    POINTER_DEFINITIONS(DoubleArrayRecord);
    static DoubleArrayRecordPtr create(
        epics::pvaClient::PvaClientPtr const &pva,
        std::string const & recordName,
        std::string const & providerName,
        std::string const & channelName
        );
    virtual ~DoubleArrayRecord() {}
    virtual bool init() {return false;}
    bool init(
        epics::pvaClient::PvaClientPtr const & pva,
        std::string const & channelName,
        std::string const & providerName
        );
private:
    DoubleArrayRecord(
        std::string const & recordName,
        epics::pvData::PVStructurePtr const & pvStructure);
};

}}}

#endif  /* DOUBLEARRAYRECORD_H */
