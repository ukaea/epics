/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */
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

#include <pv/device.h>

#ifdef exampleRPCEpicsExportSharedSymbols
#   define epicsExportSharedSymbols
#	undef exampleRPCEpicsExportSharedSymbols
#endif

#include <shareLib.h>


namespace epics { namespace exampleCPP { namespace exampleRPC { 


class AbortService;
typedef std::tr1::shared_ptr<AbortService> AbortServicePtr;

class ConfigureService;
typedef std::tr1::shared_ptr<ConfigureService> ConfigureServicePtr;

class RunService;
typedef std::tr1::shared_ptr<RunService> RunServicePtr;

class PauseService;
typedef std::tr1::shared_ptr<PauseService> PauseServicePtr;

class ResumeService;
typedef std::tr1::shared_ptr<ResumeService> ResumeServicePtr;

class StopService;
typedef std::tr1::shared_ptr<StopService> StopServicePtr;

class RewindService;
typedef std::tr1::shared_ptr<RewindService> RewindServicePtr;

class ScanService;
typedef std::tr1::shared_ptr<ScanService> ScanServicePtr;



class ExampleRPC;
typedef std::tr1::shared_ptr<ExampleRPC> ExampleRPCPtr;



class epicsShareClass AbortService :
    public virtual epics::pvAccess::RPCService
{
public:
    POINTER_DEFINITIONS(AbortService);

    static AbortService::shared_pointer create(ExampleRPCPtr const & pvRecord)
    {
        return AbortServicePtr(new AbortService(pvRecord));
    }
    ~AbortService() {};
 
    epics::pvData::PVStructurePtr request(
        epics::pvData::PVStructure::shared_pointer const & args
    ) throw (epics::pvAccess::RPCRequestException);
private:
    AbortService(ExampleRPCPtr const & pvRecord)
    : pvRecord(pvRecord)
    {
    }

    ExampleRPCPtr pvRecord;
};


class epicsShareClass ConfigureService :
    public virtual epics::pvAccess::RPCService
{
public:
    POINTER_DEFINITIONS(ConfigureService);

    static ConfigureService::shared_pointer create(ExampleRPCPtr const & pvRecord)
    {
        return ConfigureServicePtr(new ConfigureService(pvRecord));
    }
    ~ConfigureService() {};
 
    epics::pvData::PVStructurePtr request(
        epics::pvData::PVStructure::shared_pointer const & args
    ) throw (epics::pvAccess::RPCRequestException);
private:
    ConfigureService(ExampleRPCPtr const & pvRecord)
    : pvRecord(pvRecord)
    {
    }

    ExampleRPCPtr pvRecord;
};


class epicsShareClass RunService :
    public virtual epics::pvAccess::RPCService
{
public:
    POINTER_DEFINITIONS(RunService);

    static RunService::shared_pointer create(ExampleRPCPtr const & pvRecord)
    {
        return RunServicePtr(new RunService(pvRecord));
    }
    ~RunService() {};
 
    epics::pvData::PVStructurePtr request(
        epics::pvData::PVStructure::shared_pointer const & args
    ) throw (epics::pvAccess::RPCRequestException);
private:
    RunService(ExampleRPCPtr const & pvRecord)
    : pvRecord(pvRecord)
    {
    }

    ExampleRPCPtr pvRecord;
};

class PauseService :
    public virtual epics::pvAccess::RPCService
{
public:
    POINTER_DEFINITIONS(PauseService);

    static PauseService::shared_pointer create(ExampleRPCPtr const & pvRecord)
    {
        return PauseServicePtr(new PauseService(pvRecord));
    }
    ~PauseService() {};
 
    epics::pvData::PVStructurePtr request(
        epics::pvData::PVStructure::shared_pointer const & args
    ) throw (epics::pvAccess::RPCRequestException);
private:
    PauseService(ExampleRPCPtr const & pvRecord)
    : pvRecord(pvRecord)
    {
    }

    ExampleRPCPtr pvRecord;
};

class ResumeService :
    public virtual epics::pvAccess::RPCService
{
public:
    POINTER_DEFINITIONS(ResumeService);

    static ResumeService::shared_pointer create(ExampleRPCPtr const & pvRecord)
    {
        return ResumeServicePtr(new ResumeService(pvRecord));
    }
    ~ResumeService() {};
 
    epics::pvData::PVStructurePtr request(
        epics::pvData::PVStructure::shared_pointer const & args
    ) throw (epics::pvAccess::RPCRequestException);
private:
    ResumeService(ExampleRPCPtr const & pvRecord)
    : pvRecord(pvRecord)
    {
    }

    ExampleRPCPtr pvRecord;
};

class StopService :
    public virtual epics::pvAccess::RPCService
{
public:
    POINTER_DEFINITIONS(StopService);

    static StopService::shared_pointer create(ExampleRPCPtr const & pvRecord)
    {
        return StopServicePtr(new StopService(pvRecord));
    }
    ~StopService() {};
 
    epics::pvData::PVStructurePtr request(
        epics::pvData::PVStructure::shared_pointer const & args
    ) throw (epics::pvAccess::RPCRequestException);
private:
    StopService(ExampleRPCPtr const & pvRecord)
    : pvRecord(pvRecord)
    {
    }

    ExampleRPCPtr pvRecord;
};

class RewindService :
    public virtual epics::pvAccess::RPCService
{
public:
    POINTER_DEFINITIONS(RewindService);

    static RewindService::shared_pointer create(ExampleRPCPtr const & pvRecord)
    {
        return RewindServicePtr(new RewindService(pvRecord));
    }
    ~RewindService() {};
 
    epics::pvData::PVStructurePtr request(
        epics::pvData::PVStructure::shared_pointer const & args
    ) throw (epics::pvAccess::RPCRequestException);

private:
    int getRequestedSteps(epics::pvData::PVStructurePtr const & args);

private:
    RewindService(ExampleRPCPtr const & pvRecord)
    : pvRecord(pvRecord)
    {
    }

    ExampleRPCPtr pvRecord;
};

class ScanService :
    public epics::pvAccess::RPCServiceAsync,
    public std::tr1::enable_shared_from_this<ScanService>
{
public:
    POINTER_DEFINITIONS(ScanService);

    virtual void update(int flags);

    class Callback : public Device::Callback
    {
    public:
        POINTER_DEFINITIONS(Callback);
        static Callback::shared_pointer create(ScanServicePtr const & record);

        virtual void update(int flags);

    private:
        Callback(ScanServicePtr service)
        : service(service)
        {}

        ScanServicePtr service;
    };

    static ScanService::shared_pointer create(ExampleRPCPtr const & pvRecord)
    {
        return ScanServicePtr(new ScanService(pvRecord));
    }

   void request(epics::pvData::PVStructurePtr const & args,
                epics::pvAccess::RPCResponseCallback::shared_pointer const & callback);
private:
    ScanService(ExampleRPCPtr const & pvRecord)
    : pvRecord(pvRecord)
    {
    }

    virtual void stateChanged(Device::State state);

    virtual void scanComplete();

    void handleError(const std::string & message);

    epics::pvAccess::RPCResponseCallback::shared_pointer rpcCallback;

    Callback::shared_pointer deviceCallback;

    ExampleRPCPtr pvRecord;
};



class epicsShareClass ExampleRPC :
    public epics::pvDatabase::PVRecord
{
public:
    POINTER_DEFINITIONS(ExampleRPC);
    static ExampleRPCPtr create(
        std::string const & recordName);
    virtual ~ExampleRPC() {}
    virtual bool init() {return false;}
    virtual void process();
    virtual epics::pvAccess::Service::shared_pointer getService(
        epics::pvData::PVStructurePtr const & pvRequest);

    class Callback : public Device::Callback
    {
    public:
        POINTER_DEFINITIONS(Callback);
        static Callback::shared_pointer create(ExampleRPCPtr const & record);

        virtual void update(int flags);

    private:
        Callback(ExampleRPCPtr record)
        : record(record)
        {}
        ExampleRPCPtr record;
    };

    virtual void update(int flags);

    DevicePtr getDevice() { return device; }

private:

    ExampleRPC(std::string const & recordName,
        epics::pvData::PVStructurePtr const & pvStructure);
    void initPvt();

    epics::pvData::PVDoublePtr      pvx;
    epics::pvData::PVDoublePtr      pvy;
    epics::pvData::PVDoublePtr      pvx_rb;
    epics::pvData::PVDoublePtr      pvy_rb;

    epics::pvData::PVIntPtr         pvStateIndex;
    epics::pvData::PVStringArrayPtr pvStateChoices;

    epics::pvData::PVTimeStamp pvTimeStamp;
    epics::pvData::PVTimeStamp pvTimeStamp_sp;
    epics::pvData::PVTimeStamp pvTimeStamp_rb;
    epics::pvData::PVTimeStamp pvTimeStamp_st;

    bool firstTime;

    DevicePtr device;
};


}}}

#endif  /* EXAMPLERPC_H */
