/* addRecord.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 * @date 2013.04.18
 */
#ifndef ADDRECORD_H
#define ADDRECORD_H

#include <pv/channelProviderLocal.h>

#include <shareLib.h>

namespace epics { namespace pvDatabase { 


class AddRecord;
typedef std::tr1::shared_ptr<AddRecord> AddRecordPtr;

/**
 * @brief Add another record in the same database.
 *
 * A record to add another record
 * It is meant to be used via a channelPutGet request.
 * The argument has one field: recordName.
 * The result has a field named status.
 */
class epicsShareClass AddRecord :
    public PVRecord
{
public:
    POINTER_DEFINITIONS(AddRecord);
    /**
     * Factory methods to create AddRecord.
     * @param recordName The name for the AddRecord.
     * @return A shared pointer to AddRecord..
     */
    static AddRecordPtr create(
        std::string const & recordName);
    /**
     * standard init method required by PVRecord
     * @return true unless record name already exists.
     */
    virtual bool init();
    /**
     * @brief Add the record specified by  recordName.
     */
    virtual void process();
private:
    AddRecord(
        std::string const & recordName,
        epics::pvData::PVStructurePtr const & pvStructure);
    epics::pvData::PVStringPtr pvRecordName;
    epics::pvData::PVStringPtr pvResult;
};

}}

#endif  /* ADDRECORD_H */
