/* install.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 */
#ifndef INSTALL_H
#define INSTALL_H
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

class AfterStart;
class AfterStartNode;
class AfterStartRequester;
class NewAfterStartRequester;

class Install;
class SupportCreation;

// from pvDatabase.h
class PVDatabase;
class PVRecord;

class AfterStartNode : private epics::pvData::NoDefaultMethods {
public:
    AfterStartNode();
    virtual ~AfterStartNode();
};

class AfterStartRequester {
public:
    virtual void callback(AfterStartNode &node) = 0;
};

class NewAfterStartRequester {
public:
    virtual void callback(AfterStart &afterStart) = 0;
};

class AfterStart : private epics::pvData::NoDefaultMethods {
public:
    AfterStart();
    ~AfterStart();
    static std::auto_ptr<AfterStartNode> createNode(
        AfterStartRequester &requester);
    static void newAfterStartRegister(NewAfterStartRequester &requester);
    static void newAfterStartUnregister(NewAfterStartRequester &requester);
    void requestCallback(
        AfterStartNode &node,
        bool afterMerge,
        epics::pvData::ThreadPriority priority);
    void callRequesters(bool afterMerge);
    void done(AfterStartNode &node);
    void doneAndRequest(
        AfterStartNode &node,
        bool afterMerge,
        epics::pvData::ThreadPriority priority);
private:
    //TBD
};

class SupportCreation : private epics::pvData::NoDefaultMethods {
public:
    SupportCreation(PVDatabase &pvDatabase,epics::pvData::Requester &requester);
    ~SupportCreation();
    bool createSupport();
    bool initializeSupport();
    bool  startSupport(AfterStart &afterStart);
private:
    //TBD
};


class Install {
public:
    static Install &getInstall();
    bool installStructures(
        epics::pvData::String xmlFile,
        epics::pvData::Requester &requester);
    bool installStructures(
        PVDatabase &pvDatabase,
        epics::pvData::Requester &requester);
    bool installStructure(
        epics::pvData::PVStructure &pvStructure,
        epics::pvData::Requester &requester);
    bool installRecords(
        epics::pvData::String xmlFile,
        epics::pvData::Requester &requester);
    bool installRecords(
        PVDatabase &pvDatabase,
        epics::pvData::Requester &requester);
    bool installRecord(
        PVRecord &pvRecord,
        epics::pvData::Requester &requester);
private:
    Install();
    ~Install();
    //TBD
};

}}

#endif  /* INSTALL_H */
