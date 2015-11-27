/* traceRecord.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 * @date 2013.04.18
 */
#ifndef TRACERECORD_H
#define TRACERECORD_H

#include <shareLib.h>

#include <pv/channelProviderLocal.h>

namespace epics { namespace pvDatabase { 


class TraceRecord;
typedef std::tr1::shared_ptr<TraceRecord> TraceRecordPtr;

/**
 * @brief Trace activity of  PVRecord.
 *
 * A record to set the trace value for another record
 * It is meant to be used via a channelPutGet request.
 * The argument has two fields: recordName and  level.
 * The result has a field named status.
 */
class epicsShareClass TraceRecord :
    public PVRecord
{
public:
    POINTER_DEFINITIONS(TraceRecord);
    /**
     * Factory methods to create TraceRecord.
     * @param recordName The name for the TraceRecord.
     * @return A shared pointer to TraceRecord..
     */
    static TraceRecordPtr create(
        std::string const & recordName);
    /**
     * destructor
     */
    virtual ~TraceRecord();
    /**
     * Clean up any resources used.
     */
    virtual void destroy();
    /**
     * standard init method required by PVRecord
     * @return true unless record name already exists.
     */
    virtual bool init();
    /**
     * Set the trace level.
     */
    virtual void process();
private:
    TraceRecord(
        std::string const & recordName,
        epics::pvData::PVStructurePtr const & pvStructure);
    PVDatabasePtr pvDatabase;
    epics::pvData::PVStringPtr pvRecordName;
    epics::pvData::PVIntPtr pvLevel;
    epics::pvData::PVStringPtr pvResult;
    bool isDestroyed;
};

}}

#endif  /* TRACERECORD_H */
