/* removeRecord.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 * @date 2013.04.18
 */
#ifndef REMOVERECORD_H
#define REMOVERECORD_H

#include <pv/channelProviderLocal.h>

#include <shareLib.h>

namespace epics { namespace pvDatabase { 


class RemoveRecord;
typedef std::tr1::shared_ptr<RemoveRecord> RemoveRecordPtr;

/**
 * @brief Remove another record in the same database.
 *
 * A record to remove another record
 * It is meant to be used via a channelPutGet request.
 * The argument has one field: recordName.
 * The result has a field named status.
 */
class epicsShareClass RemoveRecord :
    public PVRecord
{
public:
    POINTER_DEFINITIONS(RemoveRecord);
    /**
     * Factory methods to create RemoveRecord.
     * @param recordName The name for the RemoveRecord.
     * @return A shared pointer to RemoveRecord..
     */
    static RemoveRecordPtr create(
        std::string const & recordName);
    /**
     * standard init method required by PVRecord
     * @return true unless record name already exists.
     */
    virtual bool init();
    /**
     * @brief Remove the record specified by  recordName.
     */
    virtual void process();
private:
    RemoveRecord(
        std::string const & recordName,
        epics::pvData::PVStructurePtr const & pvStructure);
    epics::pvData::PVStringPtr pvRecordName;
    epics::pvData::PVStringPtr pvResult;
};

}}

#endif  /* REMOVERECORD_H */
