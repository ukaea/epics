// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution

/* helloPutGetRecord.h */
/**
 * @author mrk
 * @date 2013.04.02
 */
#ifndef HELLOPUTGETRECORD_H
#define HELLOPUTGETRECORD_H

#ifdef epicsExportSharedSymbols
#   define helloPutGetEpicsExportSharedSymbols
#   undef epicsExportSharedSymbols
#endif

#include <pv/pvDatabase.h>
#include <pv/timeStamp.h>
#include <pv/pvTimeStamp.h>

#ifdef helloPutGetEpicsExportSharedSymbols
#   define epicsExportSharedSymbols
#	undef helloPutGetEpicsExportSharedSymbols
#endif

#include <shareLib.h>


namespace epics { namespace exampleCPP { namespace helloPutGet { 


class HelloPutGetRecord;
typedef std::tr1::shared_ptr<HelloPutGetRecord> HelloPutGetRecordPtr;

class epicsShareClass HelloPutGetRecord :
    public epics::pvDatabase::PVRecord
{
public:
    POINTER_DEFINITIONS(HelloPutGetRecord);
    static HelloPutGetRecordPtr create(
        std::string const & recordName);
    virtual ~HelloPutGetRecord();
    virtual void destroy();
    virtual bool init();
    virtual void process();
private:
    HelloPutGetRecord(std::string const & recordName,
        epics::pvData::PVStructurePtr const & pvStructure);

    epics::pvData::PVStringPtr pvArgumentValue;
    epics::pvData::PVStringPtr pvResultValue;
};


}}}

#endif  /* HELLOPUTGETRECORD_H */
