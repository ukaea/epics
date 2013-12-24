/* util.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 */
#ifndef UTIL_H
#define UTIL_H
#include <string>
#include <stdexcept>
#include <memory>

#include <pv/pvIntrospect.h>
#include <pv/pvData.h>
#include <pv/noDefaultMethods.h>
#include <pv/pvEnumerated.h>
#include <pv/thread.h>

#include <pv/pvDatabase.h>
#include <pv/support.h>

namespace epics { namespace pvIOC { 

// from pvDatabase.h
class PVRecord;

enum RequestResult {
    requestResultZombie,
    requestResultSuccess,
    requestResultFailure
};

enum ScanType {
    scanPassive,
    scanEvent,
    scanPeriodic
};

namespace RequestResultFunc {
    const char *name(ScanType);
    void toString(
        epics::pvData::StringBuilder buf,const RequestResult requestResult);
};

namespace ScanTypeFunc {
    const char *name(ScanType);
    void toString(epics::pvData::StringBuilder buf,const ScanType scanType);
};
    
class EventAnnounce {
public:
    virtual ~EventAnnounce(){}
    virtual void announce() = 0;
};

class EventScanner :
    public epics::pvData::Requester,
    private epics::pvData::NoDefaultMethods
{
public:
    static EventScanner &getEventScanner();
    ~EventScanner();
    bool addRecord(PVRecord &pvRecord);
    bool removeRecord(
        PVRecord &pvRecord,
        epics::pvData::String eventName,
        epics::pvData::ThreadPriority scanPriority);
    std::auto_ptr<EventAnnounce> addEventAnnouncer(
        epics::pvData::String eventName,
        epics::pvData::String announcer);
    void removeEventAnnouncer(
        EventAnnounce &eventAnnounce,
        epics::pvData::String announcer);
    void toString(epics::pvData::StringBuilder buf);
    void show(
        epics::pvData::StringBuilder buf,
        epics::pvData::String eventName);
private:
    EventScanner();
};

class PeriodicScanner :
    public epics::pvData::Requester,
    private epics::pvData::NoDefaultMethods
{
public:
    static PeriodicScanner& getPeriodicScanner();
    ~PeriodicScanner();
    bool addRecord(PVRecord &pvRecord);
    bool removeRecord(
        PVRecord &pvRecord,
        double rate,
        epics::pvData::ThreadPriority threadPriority);
    void toString(epics::pvData::StringBuilder buf);
    void show(
        epics::pvData::StringBuilder buf,
        epics::pvData::ThreadPriority threadPriority);
    void show(
        epics::pvData::StringBuilder buf,
        double rate);
    void show(
        epics::pvData::StringBuilder buf,
        double rate,
        epics::pvData::ThreadPriority threadPriority);
private:
    PeriodicScanner();
};

class ScanField :
    public epics::pvData::Requester,
    private epics::pvData::NoDefaultMethods
{
public:
    ~ScanField();
    static std::auto_ptr<ScanField> create(PVRecord& pvRecord);
    epics::pvData::ThreadPriority getPriority();
    epics::pvData::PVInt &getPriorityIndexPV();
    ScanType getScanType();
    epics::pvData::PVInt &getScanTypeIndexPV();
    double getRate();
    epics::pvData::PVDouble &getRatePV();
    epics::pvData::String getEventName();
    epics::pvData::PVString &getEventNamePV();
    bool getSingleProcessRequester();
    epics::pvData::PVBoolean &getSingleProcessRequesterPV();
    bool getProcessAfterStart();
    epics::pvData::PVBoolean &getProcessAfterStartPV();
private:
    ScanField(PVRecord& pvRecord);
};

}}

#endif  /* UTIL_H */
