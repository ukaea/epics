/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 * @date 2013.04.02
 */
#ifndef EXAMPLEHELLORPCRECORD_H
#define EXAMPLEHELLORPCRECORD_H

#ifdef epicsExportSharedSymbols
#   define exampleHelloRPCRecordEpicsExportSharedSymbols
#   undef epicsExportSharedSymbols
#endif

#include <pv/pvData.h>
#include <pv/pvDatabase.h>
#include <pv/timeStamp.h>
#include <pv/pvTimeStamp.h>

#ifdef exampleHelloRPCRecordEpicsExportSharedSymbols
#   define epicsExportSharedSymbols
#	undef exampleHelloRPCRecordEpicsExportSharedSymbols
#endif

#include <shareLib.h>


namespace epics { namespace exampleCPP { namespace database {


class ExampleHelloRPCRecord;
typedef std::tr1::shared_ptr<ExampleHelloRPCRecord> ExampleHelloRPCRecordPtr;

/**
 * @brief A PVRecord that implements a hello service accessed via a channelRPC request.
 *
 */
class epicsShareClass ExampleHelloRPCRecord :
    public epics::pvDatabase::PVRecord,
    public virtual epics::pvAccess::RPCService
{
public:
    POINTER_DEFINITIONS(ExampleHelloRPCRecord);
    /**
     * @brief Create an instance of ExampleHelloRecord.
     *
     * @param recordName The name of the record.
     * @return The new instance.
     */
    static  ExampleHelloRPCRecordPtr create(std::string const & recordName);

     /**
      * @brief Get the ExampleRPC service.
      */
    virtual epics::pvAccess::Service::shared_pointer getService(
        epics::pvData::PVStructurePtr const & pvRequest);
    /**
      * @brief Process a request from the client
      *
      * @param args The request from the client
      * @return The result.
      */
    epics::pvData::PVStructurePtr request(
        epics::pvData::PVStructurePtr const & args
    );
    virtual ~ExampleHelloRPCRecord() {}
    virtual bool init() {return false;}
private :
    ExampleHelloRPCRecord(
        std::string const & recordName,
        epics::pvData::PVStructurePtr const & pvTop);

    epics::pvData::PVStructurePtr put(
        epics::pvData::PVStringPtr const & pvFrom);

    epics::pvData::PVStructurePtr pvTop;
    epics::pvAccess::Service::shared_pointer service;
};

}}}

#endif  /* EXAMPLEHELLORPCRECORD_H */
