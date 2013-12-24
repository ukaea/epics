/* pvDatabase.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 */
/* Marty Kraimer 2011.03 */
#include <string>
#include <stdexcept>
#include <memory>
#include <cstdio>
#include <cstddef>
#include <cstdlib>
#include <string>

#include <epicsThread.h>
#include <epicsExit.h>

#include <pv/pvIntrospect.h>
#include <pv/pvData.h>
#include <pv/noDefaultMethods.h>
#include <pv/support.h>
#include <pv/pvDatabase.h>

namespace epics { namespace pvIOC { 

using namespace epics::pvData;
using namespace epics::pvAccess;

int PVDatabase::messageQueueSize = 300;

PVDatabase::PVDatabase(String name)
: name(name),
  isMaster(((name.compare("master")==0) ? true : false)),
  messageQueue(messageQueueSize),
  executor(0),
  executorNode(0)
{
    if(name.compare("master")==0) {
        isMaster = true;
        executor = new Executor(String("PVDatabaseMessage"),lowestPriority);
        executorNode = executor->createNode(this);
    }
}

PVDatabase::~PVDatabase()
{
    if(isMaster) delete executor;
}

String PVDatabase::getRequesterName()
{
    return name;
}

void PVDatabase::command() // handles messages
{
    while(true) {
        String message;
        MessageType messageType;
        MessageNode *messageNode = 0;
        int numOverrun = 0;
        {
            Lock xx(databaseMutex);
            messageNode = messageQueue.get();
            numOverrun = messageQueue.getClearOverrun();
            if(messageNode==0) break;
            message = messageNode->getMessage();
            messageType = messageNode->getMessageType();
            messageNode->setMessageNull();
        }
        {
            Lock xx(requesterMutex);
            if(requesterList.isEmpty()) {
                FILE *out = stdout;
                if(messageType!=infoMessage) out = stderr;
                if(numOverrun>0) {
                     fprintf(out,"%s numberOverrun %d dropped messages\n",
                         messageTypeName[messageType].c_str(),
                         numOverrun);
                }
             } else {
                RequesterListNode *node = requesterList.getHead();
                while(node!=0) {
                    Requester &requester = node->getObject();
                    requester.message(message,messageType);
                    if(numOverrun>0) {
                        char buffer[20];
                        sprintf(buffer,"%d dropped messages",numOverrun);
                        requester.message(buffer,messageType);
                    }
                    node = requesterList.getNext(*node);
                }
            }
        }
    }
}

void PVDatabase::message(String message,MessageType messageType)
{
    if(isMaster) {
        bool execute;
        {
            Lock xx(databaseMutex);
            if(messageQueue.isEmpty()) execute = true;
            messageQueue.put(message,messageType,true);
        }
        if(execute) executor->execute(executorNode);
        return;
    }
    Lock xx(requesterMutex);
    if(requesterList.isEmpty()) {
        FILE *out = stdout;
        if(messageType!=infoMessage) out = stderr;
        fprintf(out,"%s %s\n",
             messageTypeName[messageType].c_str(),
             message.c_str());
     } else {
        RequesterListNode *node = requesterList.getHead();
        while(node!=0) {
            Requester &requester = node->getObject();
            requester.message(message,messageType);
            node = requesterList.getNext(*node);
        }
    }
}

String PVDatabase::getName()
{
    return name;
}

void PVDatabase::mergeIntoMaster()
{
    if(isMaster) return;
    PVDatabaseFactory &factory = PVDatabaseFactory::getPVDatabaseFactory();
    PVDatabase &master = factory.getMaster();
    Lock xx(databaseMutex);
    master.merge(recordMap,structureMap);
}

void PVDatabase::merge(RecordMap recMap,StructureMap structMap)
{
    Lock xx(databaseMutex);
    RecordMap::iterator iter = recMap.begin();
    while(iter!=recMap.end()) {
        String key = iter->first;
        PVRecordPtr pvRecord = iter->second;
        recordMap.insert(RecordMap::value_type(key,pvRecord));
        recMap.erase(iter);
        iter = recMap.begin();
    }
    StructureMap::iterator iter1 = structMap.begin();
    while(iter1!=structMap.end()) {
        String key = iter1->first;
        PVStructurePtr pvStructure = iter1->second;
        structureMap.insert(StructureMap::value_type(key,pvStructure));
        structMap.erase(iter1);
        iter1 = structMap.begin();
    }
    
}

PVRecordPtr PVDatabase::findRecord(String recordName)
{
    Lock xx(databaseMutex);
    RecordMap::iterator iter = recordMap.find(recordName);
    if(iter!=recordMap.end()) {
        return iter->second;
    }
    return 0;
}

bool PVDatabase::addRecord(PVRecordPtr record)
{
    {
        Lock xx(databaseMutex);
        String key = record->getRecordName();
        RecordMap::iterator iter = recordMap.find(key);
        if(iter!=recordMap.end()) {
             message(String("PVDatabase::addRecord " + key + "already exists"),
                 warningMessage);
             return false;
        }
        if(!isMaster) {
            PVDatabaseFactory &factory =
                 PVDatabaseFactory::getPVDatabaseFactory();
            PVDatabase &master = factory.getMaster();
            if(master.findRecord(key)!=0) {
                message(String(
                    "PVDatabase::addRecord " + key + "already exists in master"),
                    warningMessage);
                return false;
            }
        }
        recordMap.insert(RecordMap::value_type(key,record));
    }
    if(isMaster) record->addRequester(*this);
    return true;
}

bool PVDatabase::removeRecord(PVRecord &record)
{
    if(isMaster) record.removeRequester(*this);
    Lock xx(databaseMutex);
    String key = record.getRecordName();
    RecordMap::iterator iter = recordMap.find(key);
    if(iter!=recordMap.end()) {
        recordMap.erase(iter);
        return true;
    }
    return false;
}

void PVDatabase::getRecordNames(PVStringArray &result)
{
    Lock xx(databaseMutex);
    int32 size = recordMap.size();
    String *strings = new String[size];
    RecordMap::iterator iter = recordMap.begin();
    int index = 0;
    while(iter!=recordMap.end()) {
        String key = iter->first;
        strings[index++] = key;
        iter = recordMap.begin();
    }
    result.put(0,size,strings,0);
}


PVStructurePtr PVDatabase::findStructure(String structureName)
{
    Lock xx(databaseMutex);
    StructureMap::iterator iter = structureMap.find(structureName);
    if(iter!=structureMap.end()) {
        return iter->second;
    }
    if(isMaster) return 0;
    PVDatabaseFactory &factory = PVDatabaseFactory::getPVDatabaseFactory();
    PVDatabase &master = factory.getMaster();
    return master.findStructure(structureName);
}

bool PVDatabase::addStructure(PVStructurePtr structure)
{
    Lock xx(databaseMutex);
    String key = structure->getField()->getFieldName();
    StructureMap::iterator iter = structureMap.find(key);
    if(iter!=structureMap.end()) return false;
    if(!isMaster) {
        PVDatabaseFactory &factory =
             PVDatabaseFactory::getPVDatabaseFactory();
        PVDatabase &master = factory.getMaster();
        if(master.findStructure(key)!=0) return false;
    }
    structureMap.insert(StructureMap::value_type(key,structure));
    return true;
}

bool PVDatabase::removeStructure(PVStructure &structure)
{
    Lock xx(databaseMutex);
    String key = structure.getField()->getFieldName();
    StructureMap::iterator iter = structureMap.find(key);
    if(iter!=structureMap.end()) {
        structureMap.erase(iter);
        return true;
    }
    return false;
}

void PVDatabase::getStructureNames(PVStringArray &result)
{
    Lock xx(databaseMutex);
    int32 size = structureMap.size();
    String *strings = new String[size];
    StructureMap::iterator iter = structureMap.begin();
    int index = 0;
    while(iter!=structureMap.end()) {
        String key = iter->first;
        strings[index++] = key;
        iter = structureMap.begin();
    }
    result.put(0,size,strings,0);
}

void PVDatabase::addRequester(Requester &requester)
{
    Lock xx(requesterMutex);
    RequesterListNode *node = requesterList.getHead();
    while(node!=0) {
        Requester &xxx = node->getObject();
        if(&xxx==&requester) {
            requester.message(
                String("already on requesterList"),warningMessage);
            return;
        }
        node = requesterList.getNext(*node);
    }
    node = new RequesterListNode(requester);
    requesterList.addTail(*node);
}

void PVDatabase::removeRequester(Requester &requester)
{
    Lock xx(requesterMutex);
    RequesterListNode *node = requesterList.getHead();
    while(node!=0) {
        Requester &xxx = node->getObject();
        if(&xxx==&requester) {
            requesterList.remove(*node);
            delete node;
            return;
        }
        node = requesterList.getNext(*node);
    }
}

void PVDatabase::recordList(
         PVStringArray &result,
         String regularExpression)
{
    throw std::logic_error(String("Not Implemented"));
}

void PVDatabase::structureList(
        PVStringArray &result,
        String regularExpression)
{
    throw std::logic_error(String("Not Implemented"));
}

void PVDatabase::recordToString(
        StringBuilder buf,
        String regularExpression)
{
    throw std::logic_error(String("Not Implemented"));
}

void PVDatabase::structureToString(
        StringBuilder buf,
        String regularExpression)
{
    throw std::logic_error(String("Not Implemented"));
}

PVDatabaseFactory::PVDatabaseFactory()
: master(std::auto_ptr<PVDatabase>( new PVDatabase(String("master")))),
  beingInstalled(std::auto_ptr<PVDatabase>(new PVDatabase(String("beingInstalled"))))
{
}

PVDatabaseFactory::~PVDatabaseFactory()
{
}

std::auto_ptr<PVDatabase> PVDatabaseFactory::create(String name)
{
    return std::auto_ptr<PVDatabase>(new PVDatabase(name));
}

PVDatabase &PVDatabaseFactory::getMaster()
{
    return *master.get();
}

PVDatabase &PVDatabaseFactory::getBeingInstalled()
{
    return *beingInstalled.get();
}

static PVDatabaseFactory *pvDatabaseFactory;

static void myDeleteStatic(void*)
{
    delete pvDatabaseFactory;
}

PVDatabaseFactory &PVDatabaseFactory::getPVDatabaseFactory()
{
    pvDatabaseFactory = new PVDatabaseFactory();
    epicsAtExit(&myDeleteStatic,0);
    return *pvDatabaseFactory;
}

}}
