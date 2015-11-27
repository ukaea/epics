/* exampleServer.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 * @date 2013.04.02
 */
#ifndef EXAMPLESERVER_H
#define EXAMPLESERVER_H

#ifdef epicsExportSharedSymbols
#   define exampleServerEpicsExportSharedSymbols
#   undef epicsExportSharedSymbols
#endif

#include <pv/pvDatabase.h>
#include <pv/timeStamp.h>
#include <pv/pvTimeStamp.h>

#ifdef exampleServerEpicsExportSharedSymbols
#   define epicsExportSharedSymbols
#	undef exampleServerEpicsExportSharedSymbols
#endif

#include <shareLib.h>


namespace epics { namespace exampleServer { 


class ExampleServer;
typedef std::tr1::shared_ptr<ExampleServer> ExampleServerPtr;

class epicsShareClass ExampleServer :
    public epics::pvDatabase::PVRecord
{
public:
    POINTER_DEFINITIONS(ExampleServer);
    static ExampleServerPtr create(
        std::string const & recordName);
    virtual ~ExampleServer();
    virtual void destroy();
    virtual bool init();
    virtual void process();
private:
    ExampleServer(std::string const & recordName,
        epics::pvData::PVStructurePtr const & pvStructure);

    epics::pvData::PVStringPtr pvArgumentValue;
    epics::pvData::PVStringPtr pvResultValue;
    epics::pvData::PVTimeStamp pvTimeStamp;
    epics::pvData::TimeStamp timeStamp;
};


}}

#endif  /* EXAMPLESERVER_H */
