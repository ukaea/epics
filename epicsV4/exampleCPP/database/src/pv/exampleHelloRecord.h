/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 * @date 2013.04.02
 */
#ifndef EXAMPLEHELLORECORD_H
#define EXAMPLEHELLORECORD_H

#ifdef epicsExportSharedSymbols
#   define exampleHelloEpicsExportSharedSymbols
#   undef epicsExportSharedSymbols
#endif

#include <pv/pvDatabase.h>
#include <pv/timeStamp.h>
#include <pv/pvTimeStamp.h>

#ifdef exampleHelloEpicsExportSharedSymbols
#   define epicsExportSharedSymbols
#	undef exampleHelloEpicsExportSharedSymbols
#endif

#include <shareLib.h>


namespace epics { namespace exampleCPP { namespace database {


class ExampleHelloRecord;
typedef std::tr1::shared_ptr<ExampleHelloRecord> ExampleHelloRecordPtr;

/**
 * @brief A PVRecord that implements a hello service accessed via a channelPutGet request.
 *
 */
class epicsShareClass ExampleHelloRecord :
    public epics::pvDatabase::PVRecord
{
public:
    POINTER_DEFINITIONS(ExampleHelloRecord);
    /**
     * @brief Create an instance of ExampleHelloRecord.
     *
     * @param recordName The name of the record.
     * @return The new instance.
     */
    static ExampleHelloRecordPtr create(
        std::string const & recordName);
    /**
     *  @brief Implement hello semantics.
     */
    virtual void process();
    virtual ~ExampleHelloRecord() {}
    virtual bool init() {return false;}
private:
    ExampleHelloRecord(std::string const & recordName,
        epics::pvData::PVStructurePtr const & pvStructure);

    epics::pvData::PVStringPtr pvArgumentValue;
    epics::pvData::PVStringPtr pvResultValue;
    epics::pvData::PVTimeStamp pvTimeStamp;
    epics::pvData::TimeStamp timeStamp;
};


}}}

#endif  /* EXAMPLEHELLORECORD_H */
