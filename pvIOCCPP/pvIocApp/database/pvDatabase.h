/* pvDatabase.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/* Marty Kraimer 2011.03 */
#ifndef PVDATABASE_H
#define PVDATABASE_H
#include <string>
#include <stdexcept>
#include <memory>
#include <map>
#include <list>

#include <pv/pvIntrospect.h>
#include <pv/pvData.h>
#include <pv/pvAccess.h>
#include <pv/convert.h>
#include <pv/linkedList.h>
#include <pv/lock.h>
#include <pv/noDefaultMethods.h>
#include <pv/executor.h>
#include <pv/messageQueue.h>

namespace epics { namespace pvIOC { 

class PVListener;
class PVDatabase;
class PVListener;
class PVRecordClient;
class PVRecord;
class PVRecordField;
class PVRecordStructure;
class PVDatabaseFactory;

typedef std::tr1::shared_ptr<PVListener> PVListenerPtr;
typedef std::tr1::shared_ptr<PVDatabase> PVDatabasePtr;
typedef std::tr1::shared_ptr<PVListener> PVListenerPtr;
typedef std::tr1::shared_ptr<PVRecordClient> PVRecordClientPtr;
typedef std::tr1::shared_ptr<PVRecord> PVRecordPtr;
typedef std::tr1::shared_ptr<PVRecordField> PVRecordFieldPtr;
typedef std::tr1::shared_ptr<PVRecordStructure> PVRecordStructurePtr;
typedef std::tr1::shared_ptr<PVDatabaseFactory> PVDatabaseFactoryPtr;
typedef std::vector<PVRecordPtr> PVRecordPtrArray;
typedef std::vector<PVRecordPtr>::iterator PVRecordPtrArray_iterator;
typedef std::vector<PVRecordPtr>::const_iterator PVRecordPtrArray_const__iterator;
typedef std::vector<PVRecordStructurePtr> PVRecordStructurePtrArray;
typedef std::vector<PVRecordStructurePtr>::iterator PVRecordStructurePtrArray_iterator;
typedef std::vector<PVRecordStructurePtr>::const_iterator PVRecordStructurePtrArray_const__iterator;
typedef std::vector<PVRecordFieldPtr> PVRecordFieldPtrArray;
typedef std::vector<PVRecordFieldPtr>::iterator PVRecordFieldPtrArray_iterator;
typedef std::vector<PVRecordFieldPtr>::const_iterator PVRecordFieldPtrArray_const__iterator;

//from support
class SupportStateFunc;
class ProcessCallbackRequester;
class ProcessContinueRequester;
class ProcessToken;
class RecordProcess;
class RecordProcessRequester;
class Support;
class SupportProcessRequester;

typedef std::tr1::shared_ptr<Support> SupportPtr;
typedef std::tr1::shared_ptr<RecordProcess> RecordProcessPtr;
typedef std::tr1::shared_ptr<PVDatabase> PVDatabasePtr;

// following are for private fields
typedef epics::pvData::LinkedListNode<epics::pvData::Requester::shared_pointer> RequesterListNode;
typedef epics::pvData::LinkedList<epics::pvData::Requester::shared_pointer> RequesterList;
typedef epics::pvData::LinkedListNode<PVListener> PVListenerListNode;
typedef epics::pvData::LinkedList<PVListener> PVListenerList;
typedef epics::pvData::LinkedListNode<PVRecordClient> PVRecordClientListNode;
typedef epics::pvData::LinkedList<PVRecordClient> PVRecordClientList;
typedef std::map<epics::pvData::String,PVRecordPtr> RecordMap;
typedef std::map<epics::pvData::String,epics::pvData::PVStructurePtr> StructureMap;
// end definitions for private fields


class PVListener {
public:
    POINTER_DEFINITIONS(PVListener);
    virtual ~PVListener();
    virtual void dataPut(PVRecordFieldPtr const &pvRecordField) = 0;
    virtual void dataPut(
        PVRecordStructurePtr const &requested,
        PVRecordFieldPtr const &pvRecordField) = 0;
    virtual void beginGroupPut(PVRecordPtr const &pvRecord) = 0;
    virtual void endGroupPut(PVRecordPtr const &pvRecord) = 0;
    virtual void unlisten(PVRecordPtr const &pvRecord) = 0;
};

class PVRecordClient {
public:
    POINTER_DEFINITIONS(PVRecordClient);
    virtual ~PVRecordClient();
    virtual void detach(PVRecordPtr const &pvRecord) = 0;
};

class PVRecord :
    virtual public epics::pvData::Requester
{
public:
    POINTER_DEFINITIONS(PVRecord);
    PVRecord(
        epics::pvData::String recordName,
        epics::pvData::PVStructurePtr const & pvStructure);
    virtual ~PVRecord();
    void destroy();
    virtual epics::pvData::String getRequesterName();
    virtual void message(epics::pvData::String message, epics::pvData::MessageType);
    void toString(epics::pvData::StringBuilder buf) ;
    void toString(epics::pvData::StringBuilder buf,int indentLevel) ;
    RecordProcessPtr getRecordProcess();
    void setRecordProcess(RecordProcessPtr const &recordProcess);
    PVRecordFieldPtr findPVRecordField(epics::pvData::PVFieldPtr const &pvField);
    PVRecordStructurePtr getPVRecordStructure();
    epics::pvData::String getRecordName();
    void addRequester(epics::pvData::Requester::shared_pointer const & requester);
    void removeRequester(epics::pvData::Requester::shared_pointer const & requester);
    void lock();
    void unlock();
    void lockOtherRecord(PVRecord::shared_pointer const & otherRecord);
    void beginGroupPut();
    void endGroupPut();
    void registerListener(PVListenerPtr const &pvListener);
    void unregisterListener(PVListenerPtr const &pvListener);
    bool isRegisteredListener(PVListenerPtr const &pvListener);
    void removeEveryListener();
    void registerClient(PVRecordClientPtr const &pvRecordClient);
    void unregisterClient(PVRecordClientPtr const &pvRecordClient);
    void detachClients();
    int getNumberClients();
private:
    static epics::pvData::int32 numberRecords;
    epics::pvData::String recordName;
    epics::pvData::PVStructurePtr pvStructure;
    PVRecordStructurePtr pvRecordStructure;
    epics::pvData::int32 id;
    epics::pvData::int32 depthGroupPut;
    RecordProcessPtr recordProcess;
    epics::pvData::ConvertPtr convert;
    RequesterList requesterList;
    PVListenerList listenerList;
    PVRecordClientList clientList;
    epics::pvData::Mutex mutex;
    epics::pvData::Lock recordLock;
    int lockRecursiveCount;
};

class PVRecordField :
    virtual public epics::pvData::Requester ,
    virtual public epics::pvData::PostHandler 
{
public:
    POINTER_DEFINITIONS(PVRecordField);
    virtual ~PVRecordField();
    virtual epics::pvData::String getRequesterName();
    virtual void message(
        epics::pvData::String, epics::pvData::MessageType);
    SupportPtr getSupport();
    void setSupport(SupportPtr const &support);
    PVRecordStructurePtr getParent();
    epics::pvData::PVFieldPtr getPVField();
    epics::pvData::String getFullFieldName();
    epics::pvData::String getFullName();
    PVRecordPtr getPVRecord();
    void renameField(epics::pvData::String newName);
    bool addListener(PVListenerPtr const &pvListener);
    void removeListener(PVListenerPtr const &pvListener);
    virtual void postPut();
private:
    PVRecordField(epics::pvData::PVFieldPtr const &pvField,
        PVRecordStructurePtr const &parent,
        PVRecordPtr const &pvRecord);
    void postParent(PVRecordFieldPtr const &subField);
    void postSubField();
    void callListener();
    void createNames();
    friend class PVRecord;
    friend class PVRecordStructure;
    PVListenerList listenerList;
    SupportPtr support;
    epics::pvData::PVFieldPtr pvField;
    PVRecordPtr pvRecord;
    PVRecordStructurePtr parent;
    bool isStructure;
    epics::pvData::String fullName;
    epics::pvData::String fullFieldName;
};

class PVRecordStructure : public PVRecordField {
public:
    POINTER_DEFINITIONS(PVRecordStructure);
    virtual ~PVRecordStructure();
    // array of pointers to PVRecordField`
    PVRecordFieldPtrArray getPVRecordFields();
    epics::pvData::PVStructurePtr getPVStructure();
private:
    PVRecordStructure(epics::pvData::PVStructurePtr const &pvStructure,
        PVRecordStructurePtr const & parent,
        PVRecordPtr const &pvRecord);
    friend class PVRecord;
    friend class PVRecordField;
    epics::pvData::PVStructurePtr pvStructure;
    PVRecordFieldPtrArray pvRecordFieldPtrArray;
};
    
class PVDatabase :
     public virtual epics::pvData::Requester,
     public virtual epics::pvData::Command
{
public:
    POINTER_DEFINITIONS(PVDatabase);
    virtual ~PVDatabase();
    virtual epics::pvData::String getRequesterName();
    virtual void message(
        epics::pvData::String, epics::pvData::MessageType);
    epics::pvData::String getName();
    virtual void command();
    void mergeIntoMaster();
    PVRecordPtr findRecord(epics::pvData::String recordName);
    bool addRecord(PVRecordPtr const &record);
    bool removeRecord(PVRecordPtr const &record);
    void getRecordNames(epics::pvData::PVStringArrayPtr const &result);
    epics::pvData::PVStructurePtr findStructure(
        epics::pvData::String structureName);
    bool addStructure(epics::pvData::PVStructurePtr const &structure);
    bool removeStructure(epics::pvData::PVStructurePtr const &structure);
    void getStructureNames(epics::pvData::PVStringArrayPtr const &result);
    void addRequester(epics::pvData::Requester::shared_pointer const &requester);
    void removeRequester(epics::pvData::Requester::shared_pointer const &requester);
    void recordList(
         epics::pvData::PVStringArrayPtr const &result,
         epics::pvData::String regularExpression);
    void structureList(
        epics::pvData::PVStringArrayPtr const &result,
        epics::pvData::String regularExpression);
    void recordToString(
        epics::pvData::StringBuilder buf,
        epics::pvData::String regularExpression);
    void structureToString(
        epics::pvData::StringBuilder buf,
        epics::pvData::String regularExpression);
private:
    void merge(RecordMap,StructureMap);
    PVDatabase(epics::pvData::String name);
    friend class PVDatabaseFactory;
    static int messageQueueSize;
    epics::pvData::String name;
    RecordMap recordMap;
    StructureMap structureMap;
    bool isMaster;
    epics::pvData::Mutex databaseMutex;
    epics::pvData::Mutex requesterMutex;
    RequesterList requesterList;
    epics::pvData::MessageQueue messageQueue;
    epics::pvData::Executor *executor;
    epics::pvData::ExecutorNode *executorNode;
};


class PVDatabaseFactory : private epics::pvData::NoDefaultMethods {
public:
    ~PVDatabaseFactory();
    static PVDatabaseFactoryPtr getPVDatabaseFactory();
    PVDatabasePtr create(epics::pvData::String name);
    PVDatabasePtr getMaster();
    PVDatabasePtr getBeingInstalled();
private:
    PVDatabaseFactory();
    PVDatabasePtr master;
    PVDatabasePtr beingInstalled;
};


}}

#endif  /* PVDATABASE_H */
