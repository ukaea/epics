/* pvDatabase.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 * @date 2012.11.21
 */

#include <epicsGuard.h>

#define epicsExportSharedSymbols

#include <pv/pvDatabase.h>

using std::tr1::static_pointer_cast;
using namespace epics::pvData;
using namespace std;

namespace epics { namespace pvDatabase {

PVDatabasePtr PVDatabase::getMaster()
{
    static PVDatabasePtr master;
    static Mutex mutex;
    Lock xx(mutex);
    if(master.get()==NULL) {
        master = PVDatabasePtr(new PVDatabase());
    }
    return master;
}

PVDatabase::PVDatabase()
: isDestroyed(false)
{
}

PVDatabase::~PVDatabase()
{
    destroy();
}

void PVDatabase::destroy()
{
    {
        epicsGuard<epics::pvData::Mutex> guard(mutex);
        if(isDestroyed) {
            return;
        }
        isDestroyed = true;
    }
    for(PVRecordMap::iterator iter = recordMap.begin(); iter != recordMap.end(); iter++) {
        PVRecordPtr pvRecord = (*iter).second;
        if(pvRecord) {
             pvRecord->destroy();
        }
    }
    recordMap.clear();
}

void PVDatabase::lock() {
    mutex.lock();
}

void PVDatabase::unlock() {
    mutex.unlock();
}

PVRecordPtr PVDatabase::findRecord(string const& recordName)
{
    epicsGuard<epics::pvData::Mutex> guard(mutex);
    PVRecordPtr xxx;
    if(isDestroyed) {
        return xxx;
    }
    PVRecordMap::iterator iter = recordMap.find(recordName);
    if(iter!=recordMap.end()) {
         return (*iter).second;
    }
    return xxx;
}

bool PVDatabase::addRecord(PVRecordPtr const & record)
{
    epicsGuard<epics::pvData::Mutex> guard(mutex);
    if(isDestroyed) {
        return false;
    }
    string recordName = record->getRecordName();
    PVRecordMap::iterator iter = recordMap.find(recordName);
    if(iter!=recordMap.end()) {
         return false;
    }
    record->start();
    recordMap.insert(PVRecordMap::value_type(recordName,record));
    return true;
}

bool PVDatabase::removeRecord(PVRecordPtr const & record)
{
    epicsGuard<epics::pvData::Mutex> guard(mutex);
    if(isDestroyed) {
        return false;
    }
    string recordName = record->getRecordName();
    PVRecordMap::iterator iter = recordMap.find(recordName);
    if(iter!=recordMap.end())  {
        PVRecordPtr pvRecord = (*iter).second;
        recordMap.erase(iter);
        if(pvRecord) pvRecord->destroy();
        return true;
    }
    return false;
}

PVStringArrayPtr PVDatabase::getRecordNames()
{
    epicsGuard<epics::pvData::Mutex> guard(mutex);
    PVStringArrayPtr xxx;
    if(isDestroyed) {
        return xxx;
    }
    PVStringArrayPtr pvStringArray = static_pointer_cast<PVStringArray>
        (getPVDataCreate()->createPVScalarArray(pvString));
    size_t len = recordMap.size();
    shared_vector<string> names(len);
    PVRecordMap::iterator iter;
    size_t i = 0;
    for(iter = recordMap.begin(); iter!=recordMap.end(); ++iter) {
        names[i++] = (*iter).first;
    }
    shared_vector<const string> temp(freeze(names));
    pvStringArray->replace(temp);
    return pvStringArray;
}

}}
