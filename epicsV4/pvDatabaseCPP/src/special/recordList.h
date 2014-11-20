/* recordListTest.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 * @date 2013.04.18
 */
#ifndef RECORDLIST_H
#define RECORDLIST_H

#include <shareLib.h>

#include <pv/pvDatabase.h>

namespace epics { namespace pvDatabase { 

class RecordListRecord;
typedef std::tr1::shared_ptr<RecordListRecord> RecordListRecordPtr;

/**
 * This is a record that provides a PVStringArray that
 * has the record names of all records in the local PVDatabase.
 * It is meant to be used by a channelPutGet request.
 */
class epicsShareClass RecordListRecord :
    public PVRecord
{
public:
    POINTER_DEFINITIONS(RecordListRecord);
    /**
     * Factory methods to create RecordListRecord.
     * @param recordName The name for the RecordListRecord.
     * @return A shared pointer to RecordListRecord..
     */
    static RecordListRecordPtr create(
        std::string const & recordName);
    /**
     * destructor
     */
    virtual ~RecordListRecord();
    /**
     * Clean up any resources used.
     */
    virtual void destroy();
    /**
     * standard init method required by PVRecord
     * @return true unless record name already exists.
     */
    virtual bool init();
    /* 
     * Generated the list of record names.
     */
    virtual void process();
private:
    RecordListRecord(std::string const & recordName,
        epics::pvData::PVStructurePtr const & pvStructure);
    epics::pvData::PVStringPtr database;
    epics::pvData::PVStringPtr regularExpression;
    epics::pvData::PVStringPtr status;
    epics::pvData::PVStringArrayPtr name;
};

}}

#endif  /* RECORDLIST_H */
