/* support.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 */
#ifndef SUPPORT_H
#define SUPPORT_H
#include <string>
#include <stdexcept>
#include <memory>
#include <list>

#include <pv/pvIntrospect.h>
#include <pv/pvData.h>
#include <pv/noDefaultMethods.h>
#include <pv/pvEnumerated.h>
#include <pv/timeStamp.h>
#include <pv/pvTimeStamp.h>
#include <pv/lock.h>

#include <pv/pvDatabase.h>
#include <pv/install.h>
#include <pv/util.h>

namespace epics { namespace pvIOC { 

enum SupportState{
    readyForInitialize,
    readyForStart,
    ready,
    zombie
};

class SupportStateFunc;
class ProcessCallbackRequester;
class ProcessContinueRequester;
class ProcessToken;
class RecordProcess;
class RecordProcessRequester;
class Support;
class SupportProcessRequester;

// from pvDatabase.h
class PVDatabase;
class PVListener;
class PVRecordClient;
class PVRecord;
class PVRecordField;
class PVRecordStructure;
class PVReplace;
class PVRecordCreate;
class PVDatabaseFactory;

typedef std::tr1::shared_ptr<Support> SupportPtr;

class SupportStateFunc {
public:
    static SupportState getSupportState(int value);
    static epics::pvData::PVEnumeratedPtr getSupportState(epics::pvData::PVField Ptr const &pvField);
        epics::pvData::PVField &pvField);
    static const char * name(SupportState state);
    static epics::pvData::String supportStateNames[];
};

class ProcessCallbackRequester : public epics::pvData::Requester {
public:
    POINTER_DEFINITIONS(ProcessCallbackRequester);
    virtual ~ProcessCallbackRequester(){}
    virtual void processCallback() = 0;
};

class ProcessContinueRequester {
public:
    POINTER_DEFINITIONS(ProcessContinueRequester);
    virtual ~ProcessContinueRequester(){}
    virtual void processContinue() = 0;

};

class RecordProcessRequester : public epics::pvData::Requester {
public:
    POINTER_DEFINITIONS(RecordProcessRequester);
    virtual ~RecordProcessRequester(){}
    virtual void becomeProcessor() = 0;
    virtual void canNotProcess(epics::pvData::String reason) = 0;
    virtual void lostRightToProcess() = 0;
    virtual void recordProcessResult(RequestResult requestResult) = 0;
    virtual void recordProcessComplete() = 0;
}; 

class SupportProcessRequester {
public:
    POINTER_DEFINITIONS(SupportProcessRequester);
    virtual ~SupportProcessRequester() {}
    virtual void supportProcessDone(RequestResult requestResult) = 0;
};

class ProcessToken : private epics::pvData::NoDefaultMethods {
public:
    ~ProcessToken() {}
private:
    ProcessToken(RecordProcessRequesterPtr const &recordProcessRequester)
    : recordProcessRequester(recordProcessRequester) {}
    RecordProcessRequesterPtr recordProcessRequester;
    friend class RecordProcess;
};

class RecordProcess :
    public SupportProcessRequester,
    public PVListener,
    private epics::pvData::NoDefaultMethods {
public:
    RecordProcess(PVRecord &pvRecord);
    ~RecordProcess();
    bool isEnabled();
    bool setEnabled(bool value);
    bool isActive();
    PVRecord &getRecord();
    bool isTrace();
    bool setTrace(bool value);
    SupportState getSupportState();
    void initialize();
    void start(AfterStart &afterStart);
    void stop();
    void uninitialize();
    std::auto_ptr<ProcessToken> requestProcessToken(
        RecordProcessRequester &recordProcessRequester);
    void releaseProcessToken(ProcessToken &processToken);
    void forceInactive();
    epics::pvData::String getRecordProcessRequesterName();
    void queueProcessRequest(ProcessToken &processToken);
    void process(ProcessToken &processToken,bool leaveActive);
    void process(ProcessToken &processToken,
        bool leaveActive,epics::pvData::TimeStamp timeStamp);
    void setInactive(ProcessToken &processToken);
    void processContinue(ProcessContinueRequester &processContinueRequester);
    void requestProcessCallback(
        ProcessCallbackRequester &processCallbackRequester);
    virtual void supportProcessDone(RequestResult requestResult);
    void setTimeStamp(epics::pvData::TimeStamp timeStamp);
    epics::pvData::TimeStamp  getTimeStamp();
    virtual void dataPut(PVRecordField &pvRecordField);
    virtual void dataPut(
        PVRecordStructure &requested,
        PVRecordField &pvRecordField);
    virtual void beginGroupPut(PVRecord &pvRecord);
    virtual void endGroupPut(PVRecord &pvRecord);
    virtual void unlisten(PVRecord &pvRecord);
private:
    void completeProcessing();
    void traceMessage(epics::pvData::String message);
    void checkForIllegalRequest();
    void message(
        epics::pvData::String message,
        epics::pvData::MessageType messageType);
    bool trace;
    PVRecord &pvRecord;
    bool enabled;
    Support * fieldSupport;
    std::auto_ptr<ScanField> scanField;
    epics::pvData::PVBoolean *pvProcessAfterStart;
    Support * scanSupport;
    PVRecordField *pvRecordFieldSingleProcessRequester;
    epics::pvData::PVBoolean *pvSingleProcessRequester;
    bool singleProcessRequester;
    std::list<ProcessToken*> tokenList;
    std::list<ProcessToken*> queueRequestList;
    bool leaveActive;
    ProcessToken *activeToken;
    bool recordProcessActive;
    std::list<ProcessCallbackRequester*> processCallbackRequesterList;
    epics::pvData::Mutex mutexPCRL;
    bool callStopAfterActive;
    bool callUninitializeAfterActive;
    bool processIsComplete;
    bool processCompleteDone;
    bool callRecordProcessComplete;
    RequestResult requestResult;
    epics::pvData::TimeStamp timeStamp;
    epics::pvData::PVTimeStamp pvTimeStamp;
};

class Support
 : public epics::pvData::Requester , private epics::pvData::NoDefaultMethods
{
public:
    Support(epics::pvData::String name,PVRecordField &pvRecordField);
    ~Support();
    virtual epics::pvData::String getRequesterName();
    virtual void message(epics::pvData::String,epics::pvData::MessageType);
    epics::pvData::String getSupportName();
    SupportState getSupportState();
    PVRecordField &getPVRecordField();
    virtual void initialize();
    virtual void start(AfterStart &afterStart);
    virtual void stop();
    virtual void uninitialize();
    virtual void process(SupportProcessRequester &supportProcessRequester);
protected:
    void setSupportState(SupportState state);
    bool checkSupportState(
        SupportState expectedState,
        epics::pvData::String message);
private:
    epics::pvData::String supportName;
    PVRecordField &pvRecordField;
    SupportState supportState;
};

typedef std::auto_ptr<Support> (*SupportCreate)
    (epics::pvData::String name, PVRecordField &pvRecordField);

class SupportRegistry
 : public epics::pvData::Requester , private epics::pvData::NoDefaultMethods {
public:
    ~SupportRegistry();
    static SupportRegistry &getSupportRegistry();
    static void registerCreate(
        epics::pvData::String name,SupportCreate supportCreate);
    static SupportCreate find(epics::pvData::String name);
private:
    SupportRegistry();
};


}}

#endif  /* SUPPORT_H */
