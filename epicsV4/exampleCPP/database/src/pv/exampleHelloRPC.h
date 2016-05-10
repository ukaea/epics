// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution

/* exampleHelloRPC.h */
/**
 * @author mrk
 * @date 2013.04.02
 */
#ifndef EXAMPLEHELLORPC_H
#define EXAMPLEHELLORPC_H

#ifdef epicsExportSharedSymbols
#   define exampleHelloRPCEpicsExportSharedSymbols
#   undef epicsExportSharedSymbols
#endif

#include <pv/pvData.h>
#include <pv/pvDatabase.h>
#include <pv/timeStamp.h>
#include <pv/pvTimeStamp.h>

#ifdef exampleHelloRPCEpicsExportSharedSymbols
#   define epicsExportSharedSymbols
#	undef exampleHelloRPCEpicsExportSharedSymbols
#endif

#include <shareLib.h>


namespace epics { namespace exampleCPP { namespace database {


class ExampleHelloRPC;
typedef std::tr1::shared_ptr<ExampleHelloRPC> ExampleHelloRPCPtr;

class ExampleHelloRPCService;
typedef std::tr1::shared_ptr<ExampleHelloRPCService> ExampleHelloRPCServicePtr;

class epicsShareClass ExampleHelloRPCService:
    public virtual epics::pvAccess::RPCService
{
public:
    POINTER_DEFINITIONS(ExampleHelloRPCService);

    static ExampleHelloRPCServicePtr create(ExampleHelloRPCPtr const & pvRecord)
    {
        return ExampleHelloRPCServicePtr(new ExampleHelloRPCService(pvRecord));
    } 
    ~ExampleHelloRPCService() {};

    epics::pvData::PVStructurePtr request(
        epics::pvData::PVStructurePtr const & args
    );
private:
    ExampleHelloRPCService(ExampleHelloRPCPtr const & pvRecord)
    : pvRecord(pvRecord) {}

    ExampleHelloRPCPtr pvRecord;
};

class epicsShareClass ExampleHelloRPC :
    public epics::pvDatabase::PVRecord
{
public:
    POINTER_DEFINITIONS(ExampleHelloRPC);
    static  ExampleHelloRPCPtr create(std::string const & recordName);
    virtual ~ExampleHelloRPC() {}
    virtual void destroy() {PVRecord::destroy();}
    virtual bool init();
    virtual void process() {PVRecord::process();}
    virtual epics::pvAccess::Service::shared_pointer getService(
        epics::pvData::PVStructurePtr const & pvRequest);
    void put(epics::pvData::PVStringPtr const & pvFrom);
private :
    ExampleHelloRPC(
        std::string const & recordName,
        epics::pvData::PVStructurePtr const & pvResult);

    epics::pvData::PVStructurePtr pvResult;
    epics::pvAccess::Service::shared_pointer service;
    friend class ExampleHelloRPCService;
};

}}}

#endif  /* EXAMPLEHELLORPC_H */
