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
}

void PVDatabase::destroy()
{
    lock();
    try {
        if(isDestroyed) {
            unlock();
            return;
        }
        isDestroyed = true;
        PVRecordMap::iterator iter;
        while(true) {
            iter = recordMap.begin();
            if(iter==recordMap.end()) break;
            PVRecordPtr pvRecord = (*iter).second;
            recordMap.erase(iter);
            if(pvRecord.get()!=NULL) pvRecord->destroy();
        }
        unlock();
    } catch (...) {
        unlock();
        throw;
    }
}

void PVDatabase::lock() {
    mutex.lock();
}

void PVDatabase::unlock() {
    mutex.unlock();
}

PVRecordPtr PVDatabase::findRecord(string const& recordName)
{
    lock();
    try {
        PVRecordPtr xxx;
        if(isDestroyed) {
            unlock();
            return xxx;
        }
        PVRecordMap::iterator iter = recordMap.find(recordName);
        if(iter!=recordMap.end()) {
             unlock();
             return (*iter).second;
        }
        unlock();
        return xxx;
    } catch(...) {
        unlock();
        throw;
    }
}

PVStringArrayPtr PVDatabase::getRecordNames()
{
    lock();
    try {
        PVStringArrayPtr xxx;
        if(isDestroyed) {
            unlock();
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
        unlock();
        return pvStringArray;
    } catch(...) {
        unlock();
        throw;
    }
}

bool PVDatabase::addRecord(PVRecordPtr const & record)
{
    lock();
    try {
        if(isDestroyed) {
            unlock();
            return false;
        }
        string recordName = record->getRecordName();
        PVRecordMap::iterator iter = recordMap.find(recordName);
        if(iter!=recordMap.end()) {
             unlock();
             return false;
        }
        record->start();
        recordMap.insert(PVRecordMap::value_type(recordName,record));
        unlock();
        return true;
    } catch(...) {
        unlock();
        throw;
    }
}

bool PVDatabase::removeRecord(PVRecordPtr const & record)
{
    lock();
    try {
        if(isDestroyed) {
            unlock();
            return false;
        }
        string recordName = record->getRecordName();
        PVRecordMap::iterator iter = recordMap.find(recordName);
        if(iter!=recordMap.end())  {
            PVRecordPtr pvRecord = (*iter).second;
            recordMap.erase(iter);
            if(pvRecord.get()!=NULL) pvRecord->destroy();
            unlock();
            return true;
        }
        unlock();
        return false;
    } catch(...) {
        unlock();
        throw;
    }
}

}}
