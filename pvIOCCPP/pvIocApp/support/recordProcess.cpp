/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 */
#include <string>
#include <stdexcept>
#include <memory>

#include <epicsTime.h>
#include <epicsThread.h>

#include <pv/pvIntrospect.h>
#include <pv/pvData.h>
#include <pv/noDefaultMethods.h>
#include <pv/pvDatabase.h>

#include <pv/support.h>

namespace epics { namespace pvIOC { 

using namespace epics::pvData;
using namespace epics::pvAccess;

class ProcessAfterStart :
    public AfterStartRequester,
    public RecordProcessRequester
{
public:
    ProcessAfterStart(RecordProcess &recordProcess,AfterStart &afterStart);
     ~ProcessAfterStart(){}
    void callback(AfterStartNode &node);
    String getRequesterName(){return requesterName;}
    void message(String message, MessageType messageType) {
         recordProcess.getRecord().message(message, messageType);
    }
    void becomeProcessor(){recordProcess.process(*processToken, false);}
    void canNotProcess(String reason) {
        recordProcess.getRecord().message(
           String("ProcessAfterStart canNotProcess ") + reason,
           warningMessage);
        delete this;
    }
    void lostRightToProcess() {}
    void recordProcessComplete() {
        recordProcess.releaseProcessToken(*processToken);
        afterStart.done(*afterStartNode);
        delete this;
    }
    void recordProcessResult(RequestResult requestResult) {}
private:
     String requesterName;
     RecordProcess &recordProcess;
     AfterStart &afterStart;
     std::auto_ptr<ProcessToken> processToken;
     std::auto_ptr<AfterStartNode> afterStartNode;
};

ProcessAfterStart::ProcessAfterStart(
    RecordProcess &recordProcess,AfterStart &afterStart)
: requesterName("processAfterStart"),
  recordProcess(recordProcess),
  afterStart(afterStart),
  processToken(0),
  afterStartNode(afterStart.createNode(*this))
{
    afterStart.requestCallback(*(afterStartNode.get()), true, middlePriority);
}

void ProcessAfterStart::callback(AfterStartNode &node)
{
    processToken = recordProcess.requestProcessToken(*this);
    if(processToken.get()==0) {
        recordProcess.getRecord().getPVRecordStructure().message(
            String("processAfterStart but requestProcessToken failed"),
             warningMessage);
        delete this;
        return;
    }
    recordProcess.queueProcessRequest(*processToken);
}


RecordProcess::RecordProcess(PVRecord &pvRecord)
: trace(false),
  pvRecord(pvRecord),
  enabled(true),
  fieldSupport(0),
  pvProcessAfterStart(0),
  scanSupport(0),
  pvRecordFieldSingleProcessRequester(0),
  pvSingleProcessRequester(0),
  singleProcessRequester(false),
  leaveActive(false),
  activeToken(0),
  recordProcessActive(false),
  callStopAfterActive(false),
  callUninitializeAfterActive(false),
  processIsComplete(false),
  processCompleteDone(false),
  callRecordProcessComplete(false),
  requestResult(),
  timeStamp(),
  pvTimeStamp()
{
}

RecordProcess::~RecordProcess()
{ }

bool RecordProcess::isEnabled()
{
    return enabled;
}

bool RecordProcess::setEnabled(bool value)
{
    pvRecord.lock();
    try {
        bool oldValue = enabled;
        enabled = value;
        bool result = (oldValue==value) ? false : true;
        pvRecord.unlock();
        return result;
    } catch(...) {
        pvRecord.unlock();
        throw;
    }
}

bool RecordProcess::isActive()
{
    return (activeToken==0) ? false : true;
}

PVRecord &RecordProcess::getRecord()
{
    return pvRecord;
}

bool RecordProcess::isTrace()
{
    return trace;
}

bool RecordProcess::setTrace(bool value)
{
    pvRecord.lock();
    try {
        bool oldValue = trace;
        trace = value;
        bool result = (oldValue==value) ? false : true;
        pvRecord.unlock();
        return result;
    } catch(...) {
        pvRecord.unlock();
        throw;
    }
}

SupportState RecordProcess::getSupportState()
{
    pvRecord.lock();
    try {
        SupportState state = fieldSupport->getSupportState();
        pvRecord.unlock();
        return state;
    } catch(...) {
        pvRecord.unlock();
        throw;
    }
}

void RecordProcess::initialize()
{
    pvRecord.lock();
    try {
        if(trace) traceMessage(" initialize");
        PVRecordStructure &pvRecordStructure = pvRecord.getPVRecordStructure();
        PVStructure &pvStructure = pvRecordStructure.getPVStructure();
        fieldSupport = const_cast<Support *>(pvRecordStructure.getSupport());
        if(fieldSupport==0) {
            throw std::logic_error(pvRecord.getRecordName() + " has no support");
        }
        PVFieldPtrArray pvFields = pvStructure.getPVFields();
        PVRecordFieldPtrArray pvRecordFields =
            pvRecordStructure.getPVRecordFields();
        StructureConstPtr structure = pvStructure.getStructure();
        int index;
        index = structure->getFieldIndex("timeStamp");
        if(index>=0) {
            pvTimeStamp.attach(pvFields[index]);
        }
        index = structure->getFieldIndex("scan");
        if(index>=0) {
            scanSupport = pvRecordFields[index]->getSupport();
            scanField = ScanField::create(pvRecord);
            if(scanField.get()!=0) {
            	pvSingleProcessRequester = 
                     &scanField->getSingleProcessRequesterPV();
            	pvRecordFieldSingleProcessRequester =
                     &pvRecord.findPVRecordField(*pvSingleProcessRequester);
                pvProcessAfterStart = &scanField->getProcessAfterStartPV();
            }
        }
        fieldSupport->initialize();
        pvRecord.unlock();
    } catch(...) {
        pvRecord.unlock();
        throw;
    }
}

void RecordProcess::start(AfterStart &afterStart)
{
    pvRecord.lock();
    try {
        if(trace) traceMessage(" start");
        fieldSupport->start(afterStart);
        if(scanSupport!=0) scanSupport->start(afterStart);
        if(pvSingleProcessRequester!=0) {
            singleProcessRequester = pvSingleProcessRequester->get();
            pvRecord.registerListener(*this);
            pvRecordFieldSingleProcessRequester->addListener(*this);
        }
        if(!singleProcessRequester && pvProcessAfterStart!=0) {
            if(pvProcessAfterStart!=0) {
                new ProcessAfterStart(*this,afterStart);
            }
        }
        pvRecord.unlock();
    } catch(...) {
        pvRecord.unlock();
        throw;
    }
}

void RecordProcess::stop()
{
    pvRecord.lock();
    try {
        if(activeToken!=0) {
            callStopAfterActive = true;
            if(trace) traceMessage("stop delayed because active");
            pvRecord.unlock();
            return;
        }
        if(trace) traceMessage("stop");
        if(scanSupport!=0) scanSupport->stop();
        fieldSupport->stop();
        pvRecord.removeEveryListener();
        pvRecord.unlock();
    } catch(...) {
        pvRecord.unlock();
        throw;
    }
}

void RecordProcess::uninitialize()
{
    pvRecord.lock();
    try {
         if(activeToken!=0) {
             callUninitializeAfterActive = true;
             if(trace) traceMessage("uninitialize delayed because active");
             pvRecord.unlock();
             return;
         }
         if(trace) traceMessage("uninitialize");
         if(scanSupport!=0) scanSupport->uninitialize();
         fieldSupport->uninitialize();
        pvRecord.unlock();
    } catch(...) {
        pvRecord.unlock();
        throw;
    }
}

std::auto_ptr<ProcessToken> RecordProcess::requestProcessToken(
        RecordProcessRequester &recordProcessRequester)
{
    pvRecord.lock();
    try {
        if(singleProcessRequester && tokenList.size()!=0) {
             return std::auto_ptr<ProcessToken>();
        }
        std::list<ProcessToken*>::iterator iter = tokenList.begin();
        while(iter!=tokenList.end()) {
            ProcessToken *token = *iter++;
            if(&token->recordProcessRequester == &recordProcessRequester) {
                throw std::invalid_argument("already have token");
            }
        }
        ProcessToken *processToken = new ProcessToken(recordProcessRequester);
        tokenList.push_back(processToken);
        std::auto_ptr<ProcessToken> token = std::auto_ptr<ProcessToken>(processToken);
        pvRecord.unlock();
        return token;
    } catch(...) {
        pvRecord.unlock();
        throw;
    }
}

void RecordProcess::releaseProcessToken(ProcessToken &processToken)
{
    pvRecord.lock();
    try {
        RecordProcessRequester &requester = processToken.recordProcessRequester;
        std::list<ProcessToken*>::iterator iter = tokenList.begin();
        while(iter!=tokenList.end()) {
            ProcessToken *token = *iter;
            if(&token->recordProcessRequester == &requester) {
                tokenList.erase(iter);
                pvRecord.unlock();
                return;
            }
            iter++;
        }
        pvRecord.unlock();
    } catch(...) {
        pvRecord.unlock();
        throw;
    }
}

void RecordProcess::forceInactive()
{
    pvRecord.lock();
    try {
        if(activeToken==0) return;
        String xxx("forceInactive recordProcessRequester");
        RecordProcessRequester *requester = &activeToken->recordProcessRequester;
        xxx += requester->getRequesterName();
        message(xxx,errorMessage);
        activeToken = 0;
        std::list<ProcessToken*>::iterator iter = tokenList.begin();
        while(iter!=tokenList.end()) {
            ProcessToken *token = *iter;
            if(&token->recordProcessRequester == requester) {
                tokenList.erase(iter);
                pvRecord.unlock();
                return;
            }
            iter++;
        }
        pvRecord.unlock();
    } catch(...) {
        pvRecord.unlock();
        throw;
    }
}

String RecordProcess::getRecordProcessRequesterName()
{
    pvRecord.lock();
    try {
        String string;
        if(activeToken!=0) string = activeToken->recordProcessRequester.getRequesterName();
        pvRecord.unlock();
        return string;
    } catch(...) {
        pvRecord.unlock();
        throw;
    }
}

void RecordProcess::queueProcessRequest(ProcessToken &token)
{
    RecordProcessRequester &recordProcessRequester = token.recordProcessRequester;
    pvRecord.lock();
    try {
        SupportState supportState = fieldSupport->getSupportState();
        if (supportState != ready) {
             recordProcessRequester.canNotProcess("record support is not ready");
             pvRecord.unlock();
             return;
        }
        if (!isEnabled()) {
            recordProcessRequester.canNotProcess("record is disabled");
            pvRecord.unlock();
            return;
        }
        if(&token==activeToken) {
            recordProcessRequester.canNotProcess("record already active");
            pvRecord.unlock();
            return;
        }
        if(activeToken!=0) {
            queueRequestList.push_back(&token);
            pvRecord.unlock();
            return;
        }
        activeToken = &token;
        processIsComplete = false;
        processCompleteDone = false;
        pvRecord.beginGroupPut();
        pvRecord.unlock();
    } catch(...) {
        pvRecord.unlock();
        throw;
    }
}

void RecordProcess::process(ProcessToken &processToken,bool leaveActive)
{
    timeStamp.getCurrent();
    process(processToken,leaveActive,timeStamp);
}

void RecordProcess::process(ProcessToken &processToken,
        bool leaveActive,TimeStamp timeStamp)
{
    if(activeToken!=&processToken) {
        throw std::logic_error("not the active process requester");
    }
    RecordProcessRequester *recordProcessRequester = 0;
    pvRecord.lock();
    try {
        recordProcessRequester = &activeToken->recordProcessRequester;
        if(pvTimeStamp.isAttached()) {
            pvTimeStamp.set(timeStamp);
            if(trace) {
                String xxx("process timeStamp ");
                xxx += recordProcessRequester->getRequesterName();
                traceMessage(xxx);
            }
        } else {
           if(trace) {
                String xxx("process no timeStamp ");
                xxx += recordProcessRequester->getRequesterName();
                traceMessage(xxx);
           }
           this->leaveActive = leaveActive;
           recordProcessActive = true;
           // NOTE: processContinue may be called before the following returns
           fieldSupport->process(*this);
           recordProcessActive = false;
           if(processIsComplete && !processCompleteDone) {
               completeProcessing();
           }
        }
        pvRecord.unlock();
    } catch(...) {
        pvRecord.unlock();
        throw;
    }
    if(callRecordProcessComplete) {
        callRecordProcessComplete = false;
        recordProcessRequester->recordProcessComplete();
        if(!leaveActive && activeToken!=0) {
            activeToken->recordProcessRequester.becomeProcessor();
        }
        return;
    }
    while(true) {
        ProcessCallbackRequester *processCallbackRequester = 0;
        {
            Lock xx(mutexPCRL);
            if(processCallbackRequesterList.size()<1) break;
            processCallbackRequester = processCallbackRequesterList.front();
            processCallbackRequesterList.pop_front();
        }
        processCallbackRequester->processCallback();
    }
    return;
}

void RecordProcess::setInactive(ProcessToken &processToken)
{
    if(activeToken!=&processToken) {
        throw std::logic_error("not the active process requester");
    }
    pvRecord.lock();
    try {
        RecordProcessRequester *recordProcessRequester
             = &activeToken->recordProcessRequester;
        if(trace) {
             String xxx("setInactive ");
             xxx += recordProcessRequester->getRequesterName();
             traceMessage(xxx);
        }
        if(!processIsComplete) {
            throw std::logic_error("processing is not finished");
        }
        if(!processCompleteDone) {
            throw std::logic_error("process complete is not done");
        }
        activeToken = 0;
        if(queueRequestList.size()>0) {
            activeToken = queueRequestList.front();
            queueRequestList.pop_front();
        }
        pvRecord.unlock();
    } catch(...) {
        pvRecord.unlock();
        throw;
    }
    if(activeToken!=0) activeToken->recordProcessRequester.becomeProcessor();
}

void RecordProcess::processContinue(
    ProcessContinueRequester &processContinueRequester)
{
    RecordProcessRequester *recordProcessRequester = 0;
    pvRecord.lock();
    try {
        if(activeToken==0) {
            String xxx("processContinue called but record");
            xxx += pvRecord.getRecordName();
            xxx += " is not active";
            throw std::logic_error(xxx);
        }
        recordProcessRequester = &activeToken->recordProcessRequester;
        if(trace) traceMessage("processContinue ");
        recordProcessActive = true;
        processContinueRequester.processContinue();
        recordProcessActive = false;
        if(processIsComplete && !processCompleteDone) completeProcessing();
        pvRecord.unlock();
    } catch(...) {
        pvRecord.unlock();
        throw;
    }
    if(callRecordProcessComplete) {
        callRecordProcessComplete = false;
        recordProcessRequester->recordProcessComplete();
        if(!leaveActive && activeToken!=0) {
            activeToken->recordProcessRequester.becomeProcessor();
        }
        return;
    }
    while(true) {
        ProcessCallbackRequester *processCallbackRequester = 0;
        {
            Lock xx(mutexPCRL);
            if(processCallbackRequesterList.size()<1) break;
            processCallbackRequester = processCallbackRequesterList.front();
            processCallbackRequesterList.pop_front();
        }
        processCallbackRequester->processCallback();
    }
}

void RecordProcess::supportProcessDone(RequestResult requestResult)
{
    if(!recordProcessActive) {
        throw std::logic_error("must be called from process or processContinue");
    }
    processIsComplete = true;
    this->requestResult = requestResult;
}

void RecordProcess::setTimeStamp(TimeStamp timeStamp)
{
    checkForIllegalRequest();
    if(trace) traceMessage("setTimeStamp");
    this->timeStamp = timeStamp;
}

TimeStamp RecordProcess::getTimeStamp()
{
    checkForIllegalRequest();
    return timeStamp;
}

void RecordProcess::dataPut(PVRecordField &pvRecordField)
{
    if(&pvRecordField!=pvRecordFieldSingleProcessRequester) {
        throw std::logic_error("processing is not finished");
    }
    pvRecord.lock();
    try {
        bool oldValue = singleProcessRequester;
        bool newValue = pvSingleProcessRequester->get();
        if(oldValue==newValue) return;
        singleProcessRequester = pvSingleProcessRequester->get();
        if(!singleProcessRequester) return;
        if(tokenList.size()<2) return;
        // remove all requesters.
        while(true) {
            if(tokenList.size()==0) break;
            ProcessToken *token = tokenList.back();
            tokenList.pop_back();
            token->recordProcessRequester.lostRightToProcess();
        }
        while(true) {
           int index = queueRequestList.size();
           if(index==0) break;
           queueRequestList.pop_back();
        }
        pvRecord.unlock();
    } catch(...) {
        pvRecord.unlock();
        throw;
    }
}

void RecordProcess::dataPut(
        PVRecordStructure &requested,
        PVRecordField &pvRecordField)
{ }

void RecordProcess::beginGroupPut(PVRecord &pvRecord)
{ }

void RecordProcess::endGroupPut(PVRecord &pvRecord)
{ }

void RecordProcess::unlisten(PVRecord &pvRecord)
{ }

void RecordProcess::completeProcessing()
{
    processCompleteDone = true;
    callRecordProcessComplete = true;
    if(callStopAfterActive) {
        if(trace) traceMessage("stop");
        if(scanSupport!=0) scanSupport->stop();
        fieldSupport->stop();
        pvRecord.removeEveryListener();
        callStopAfterActive = false;
    }
    if(callUninitializeAfterActive) {
        if(trace) traceMessage("uninitialize");
        if(scanSupport!=0) scanSupport->uninitialize();
        fieldSupport->uninitialize();
        callUninitializeAfterActive = false;
    }
    if(processCallbackRequesterList.size()>0){
        pvRecord.message(
            "completing processing but ProcessCallbackRequesters are still present",
            fatalErrorMessage);
    }
    pvRecord.endGroupPut();
    activeToken->recordProcessRequester.recordProcessResult(requestResult);
    if(!leaveActive) {
    	activeToken = 0;
    	if(queueRequestList.size()>0) {
                activeToken = queueRequestList.front();
                queueRequestList.pop_front();
        	processIsComplete = false;
    		processCompleteDone = false;
    		pvRecord.beginGroupPut();
        }
    }
    if(trace) traceMessage("process completion " + fieldSupport->getRequesterName());
}

void RecordProcess::traceMessage(epics::pvData::String mess)
{
    epicsTimeStamp timeStamp;
    epicsTimeGetCurrent(&timeStamp);
    struct tm tm;
    unsigned long nano;
    epicsTimeToTM(&tm,&nano,&timeStamp);
    char buffer[50];
    sprintf(buffer,"%d.%d.%d. %d:%d:%d ",
        tm.tm_year + 1900,
        tm.tm_mon,
        tm.tm_mday,
        tm.tm_hour,tm.tm_min,tm.tm_sec);
     String builder(buffer);
     builder +=  mess + " thread " + epicsThreadGetNameSelf();
    message(builder, infoMessage);
}

void RecordProcess::checkForIllegalRequest()
{
    if(activeToken!=0 && (recordProcessActive)) return;
    if(activeToken==0) {
        message("illegal request because record is not active", infoMessage);
        throw std::logic_error("record is not active");
    } else {
        message(
          "illegal request because neither process or processContinue is running",
          infoMessage);
        throw std::logic_error("neither process or processContinue is running");
    }
}


void RecordProcess::message(String message,MessageType messageType)
{
    pvRecord.message(pvRecord.getRecordName() + " " + message,messageType);
}

}}
