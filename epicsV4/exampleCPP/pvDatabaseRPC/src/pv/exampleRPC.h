// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution

/* exampleRPC.h */

/**
 * @author dgh
 * @date 2015.12.08
 */
#ifndef EXAMPLERPC_H
#define EXAMPLERPC_H

#ifdef epicsExportSharedSymbols
#   define exampleRPCEpicsExportSharedSymbols
#   undef epicsExportSharedSymbols
#endif

#include <pv/pvDatabase.h>
#include <pv/timeStamp.h>
#include <pv/pvTimeStamp.h>

#ifdef exampleRPCEpicsExportSharedSymbols
#   define epicsExportSharedSymbols
#	undef exampleRPCEpicsExportSharedSymbols
#endif

#include <shareLib.h>


namespace epics { namespace exampleCPP { namespace exampleRPC { 

class ExampleRPCService;
typedef std::tr1::shared_ptr<ExampleRPCService> ExampleRPCServicePtr;

class ExampleRPCServiceAsync;
typedef std::tr1::shared_ptr<ExampleRPCServiceAsync> ExampleRPCServiceAsyncPtr;


class ExampleRPC;
typedef std::tr1::shared_ptr<ExampleRPC> ExampleRPCPtr;



class epicsShareClass ExampleRPCService :
    public virtual epics::pvAccess::RPCService
{
public:
    POINTER_DEFINITIONS(ExampleRPCService);

    static ExampleRPCService::shared_pointer create(ExampleRPCPtr const & pvRecord)
    {
        return ExampleRPCServicePtr(new ExampleRPCService(pvRecord));
    }
    ~ExampleRPCService() {};
 
    epics::pvData::PVStructurePtr request(
        epics::pvData::PVStructure::shared_pointer const & args
    ) throw (epics::pvAccess::RPCRequestException);
private:
    ExampleRPCService(ExampleRPCPtr const & pvRecord)
    : pvRecord(pvRecord)
    {
    }

    ExampleRPCPtr pvRecord;
};


class ExampleRPCServiceAsync :
    public epics::pvAccess::RPCServiceAsync
{
public:
    POINTER_DEFINITIONS(ExampleRPCServiceAsync);

    static ExampleRPCServiceAsync::shared_pointer create(ExampleRPCPtr const & pvRecord)
    {
        return ExampleRPCServiceAsyncPtr(new ExampleRPCServiceAsync(pvRecord));
    }

   void request(epics::pvData::PVStructurePtr const & args,
                epics::pvAccess::RPCResponseCallback::shared_pointer const & callback);
private:
    ExampleRPCServiceAsync(ExampleRPCPtr const & pvRecord)
    : pvRecord(pvRecord)
    {
    }

    ExampleRPCPtr pvRecord;
};

class ExampleRPC;
typedef std::tr1::shared_ptr<ExampleRPC> ExampleRPCPtr;

class epicsShareClass ExampleRPC :
    public epics::pvDatabase::PVRecord
{
public:
    POINTER_DEFINITIONS(ExampleRPC);
    static ExampleRPCPtr create(
        std::string const & recordName);
    virtual ~ExampleRPC();
    virtual void destroy();
    virtual bool init();
    virtual void process();
    virtual epics::pvAccess::Service::shared_pointer getService(
        epics::pvData::PVStructurePtr const & pvRequest);
    void put(double x, double y);

    bool takeControl();
    void releaseControl();
private:

    ExampleRPC(std::string const & recordName,
        epics::pvData::PVStructurePtr const & pvStructure);

    epics::pvData::PVTimeStamp pvTimeStamp;
    epics::pvData::TimeStamp timeStamp;
    epics::pvData::Mutex taskMutex;
    epics::pvAccess::Service::shared_pointer service;
};


}}}

#endif  /* EXAMPLERPC_H */
