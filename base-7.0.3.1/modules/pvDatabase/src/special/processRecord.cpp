/* processRecord.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 * @date 2013.04.18
 */
#include <map>
#include <epicsThread.h>
#include <pv/event.h>
#include <shareLib.h>
#include <string>
#include <cstring>
#include <stdexcept>
#include <memory>
#include <set>

#include <pv/lock.h>
#include <pv/pvType.h>
#include <pv/pvData.h>
#include <pv/pvTimeStamp.h>
#include <pv/timeStamp.h>
#include <pv/rpcService.h>
#include <pv/pvAccess.h>
#include <pv/status.h>
#include <pv/serverContext.h>

#define epicsExportSharedSymbols
#include "pv/pvStructureCopy.h"
#include "pv/pvDatabase.h"
#include "pv/processRecord.h"

using std::tr1::static_pointer_cast;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace std;

namespace epics { namespace pvDatabase { 

ProcessRecordPtr ProcessRecord::create(
    std::string const & recordName,double delay)
{
    FieldCreatePtr fieldCreate = getFieldCreate();
    PVDataCreatePtr pvDataCreate = getPVDataCreate();
    StructureConstPtr  topStructure = fieldCreate->createFieldBuilder()->
        addNestedStructure("argument")->
            add("command",pvString)->
            add("recordName",pvString)->
            endNested()->
        addNestedStructure("result") ->
            add("status",pvString) ->
            endNested()->
        createStructure();
    PVStructurePtr pvStructure = pvDataCreate->createPVStructure(topStructure);
    ProcessRecordPtr pvRecord(
        new ProcessRecord(recordName,pvStructure,delay));
    if(!pvRecord->init()) pvRecord.reset();
    return pvRecord;
}

void ProcessRecord::startThread()
{
    thread = EpicsThreadPtr(new epicsThread(
        *this,
        "processRecord",
        epicsThreadGetStackSize(epicsThreadStackSmall),
        epicsThreadPriorityLow));
    thread->start();
}

void ProcessRecord::stop()
{
    runStop.signal();
    runReturn.wait();
}


ProcessRecord::ProcessRecord(
    std::string const & recordName,
    epics::pvData::PVStructurePtr const & pvStructure,double delay)
: PVRecord(recordName,pvStructure),
  delay(delay),
  pvDatabase(PVDatabase::getMaster())
{
}

bool ProcessRecord::init()
{
    initPVRecord();
    PVStructurePtr pvStructure = getPVStructure();
    pvCommand = pvStructure->getSubField<PVString>("argument.command");
    pvRecordName = pvStructure->getSubField<PVString>("argument.recordName");
    if(!pvRecordName) return false;
    pvResult = pvStructure->getSubField<PVString>("result.status");
    if(!pvResult) return false;
    startThread();
    return true;
}

void ProcessRecord::process()
{
    string recordName = pvRecordName->get();
    string command = pvCommand->get();
    if(command.compare("add")==0) {
        epicsGuard<epics::pvData::Mutex> guard(mutex);
        std::map<std::string,PVRecordPtr>::iterator iter = pvRecordMap.find(recordName);
        if(iter!=pvRecordMap.end()) {
             pvResult->put(recordName + " already present");
             return;
        }
        PVRecordPtr pvRecord = pvDatabase->findRecord(recordName);
        if(!pvRecord) {
             pvResult->put(recordName + " not in pvDatabase");
             return;
        }
        pvRecordMap.insert(PVRecordMap::value_type(recordName,pvRecord));
        pvResult->put("success");
        return;
    } else if(command.compare("remove")==0) {
        epicsGuard<epics::pvData::Mutex> guard(mutex);
        std::map<std::string,PVRecordPtr>::iterator iter = pvRecordMap.find(recordName);
        if(iter==pvRecordMap.end()) {
             pvResult->put(recordName + " not found");
             return;
        }
        pvRecordMap.erase(iter);
        pvResult->put("success");
        return;
    } else {
        pvResult->put(command  + " not a valid command: only add and remove are valid");
        return;
    }
}

void ProcessRecord::run()
{
    while(true) {
        if(runStop.tryWait()) {
             runReturn.signal();
             return;
        }    
        if(delay>0.0) epicsThreadSleep(delay);
        epicsGuard<epics::pvData::Mutex> guard(mutex);
        PVRecordMap::iterator iter;
        for(iter = pvRecordMap.begin(); iter!=pvRecordMap.end(); ++iter) {
           PVRecordPtr pvRecord = (*iter).second;
           pvRecord->lock();
           pvRecord->beginGroupPut();
           try {
               pvRecord->process();
           } catch (std::exception& ex) {
               std::cout << "record " << pvRecord->getRecordName() << "exception " << ex.what() << "\n";
           } catch (...) {
               std::cout<< "record " << pvRecord->getRecordName() << " process exception\n";
           }
           pvRecord->endGroupPut();
           pvRecord->unlock();
        }
    }
}


}}

