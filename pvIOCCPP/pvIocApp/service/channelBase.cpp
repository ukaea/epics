/* channelBase.cpp */
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

#include "dbFldTypes.h"
#include "dbDefs.h"

#include <pv/pvIntrospect.h>
#include <pv/pvData.h>
#include <pv/noDefaultMethods.h>
#include <pv/standardField.h>

#define epicsExportSharedSymbols
#include <pv/channelBase.h>

namespace epics { namespace pvAccess { 
using namespace epics::pvData;
using std::tr1::static_pointer_cast;
using std::tr1::dynamic_pointer_cast;

int ChannelBaseDebugLevel = 0;
void ChannelBaseDebug::setLevel(int level) {ChannelBaseDebugLevel = level;}
int ChannelBaseDebug::getLevel() {return ChannelBaseDebugLevel;}

ChannelBase::ChannelBase(
    ChannelProvider::shared_pointer const & provider,
    ChannelRequester::shared_pointer const & requester,
    String const &name)
:   provider(provider),
    requester(requester),
    channelName(name),
    beingDestroyed(false)
{
    if(ChannelBaseDebug::getLevel()>0) {
         printf("ChannelBase::ChannelBase\n");
    }
}

ChannelBase::~ChannelBase()
{
    if(ChannelBaseDebug::getLevel()>0) {
         printf("ChannelBase::~ChannelBase\n");
    }
}

void ChannelBase::destroy()
{
    if(ChannelBaseDebug::getLevel()>0) {
         printf("ChannelBase::destroy beingDestroyed %s\n",
         (beingDestroyed ? "true" : "false"));
    }
    {
        Lock xx(mutex);
        if(beingDestroyed) return;
        beingDestroyed = true;
    }
    while(true) {
        std::set<ChannelProcess::shared_pointer>::iterator it;
        it = channelProcessList.begin();
        if(it==channelProcessList.end()) break;
        it->get()->destroy();
        channelProcessList.erase(it);
    }
    while(true) {
        std::set<ChannelGet::shared_pointer>::iterator it;
        it = channelGetList.begin();
        if(it==channelGetList.end()) break;
        it->get()->destroy();
        channelGetList.erase(it);
    }
    while(true) {
        std::set<ChannelPut::shared_pointer>::iterator it;
        it = channelPutList.begin();
        if(it==channelPutList.end()) break;
        it->get()->destroy();
        channelPutList.erase(it);
    }
    while(true) {
        std::set<ChannelPutGet::shared_pointer>::iterator it;
        it = channelPutGetList.begin();
        if(it==channelPutGetList.end()) break;
        it->get()->destroy();
        channelPutGetList.erase(it);
    }
    while(true) {
        std::set<Monitor::shared_pointer>::iterator it;
        it = channelMonitorList.begin();
        if(it==channelMonitorList.end()) break;
        it->get()->destroy();
        channelMonitorList.erase(it);
    }
    while(true) {
        std::set<ChannelRPC::shared_pointer>::iterator it;
        it = channelRPCList.begin();
        if(it==channelRPCList.end()) break;
        it->get()->destroy();
        channelRPCList.erase(it);
    }
    while(true) {
        std::set<ChannelArray::shared_pointer>::iterator it;
        it = channelArrayList.begin();
        if(it==channelArrayList.end()) break;
        it->get()->destroy();
        channelArrayList.erase(it);
    }
    std::tr1::static_pointer_cast<ChannelBaseProvider>(provider)->removeChannel(getPtrSelf());
}


void ChannelBase::addChannelProcess(ChannelProcess::shared_pointer const & channelProcess)
{
    if(ChannelBaseDebug::getLevel()>0) {
         printf("ChannelBase::addChannelProcess\n");
    }
    Lock xx(mutex);
    if(beingDestroyed) return;
    channelProcessList.insert(channelProcess);
}

void ChannelBase::addChannelGet(ChannelGet::shared_pointer const &channelGet)
{
    if(ChannelBaseDebug::getLevel()>0) {
         printf("ChannelBase::addChannelGet\n");
    }
    Lock xx(mutex);
    if(beingDestroyed) return;
    channelGetList.insert(channelGet);
}

void ChannelBase::addChannelPut(ChannelPut::shared_pointer const &channelPut)
{
    if(ChannelBaseDebug::getLevel()>0) {
         printf("ChannelBase::addChannelPut\n");
    }
    Lock xx(mutex);
    if(beingDestroyed) return;
    channelPutList.insert(channelPut);
}

void ChannelBase::addChannelPutGet(ChannelPutGet::shared_pointer const &channelPutGet)
{
    if(ChannelBaseDebug::getLevel()>0) {
         printf("ChannelBase::addChannelPutGet\n");
    }
    Lock xx(mutex);
    if(beingDestroyed) return;
    channelPutGetList.insert(channelPutGet);
}

void ChannelBase::addChannelMonitor(Monitor::shared_pointer const &monitor)
{
    if(ChannelBaseDebug::getLevel()>0) {
         printf("ChannelBase::addChannelMonitor\n");
    }
    Lock xx(mutex);
    if(beingDestroyed) return;
    channelMonitorList.insert(monitor);
}

void ChannelBase::addChannelRPC(ChannelRPC::shared_pointer const &channelRPC)
{
    if(ChannelBaseDebug::getLevel()>0) {
         printf("ChannelBase::addChannelRPC\n");
    }
    Lock xx(mutex);
    if(beingDestroyed) return;
    channelRPCList.insert(channelRPC);
}

void ChannelBase::addChannelArray(ChannelArray::shared_pointer const &channelArray)
{
    if(ChannelBaseDebug::getLevel()>0) {
         printf("ChannelBase::addChannelArray\n");
    }
    Lock xx(mutex);
    if(beingDestroyed) return;
    channelArrayList.insert(channelArray);
}

void ChannelBase::removeChannelProcess(ChannelProcess::shared_pointer const &ref)
{
    if(ChannelBaseDebug::getLevel()>0) {
         printf("ChannelBase::removeChannelProcess\n");
    }
    Lock xx(mutex);
    if(beingDestroyed) return;
    channelProcessList.erase(ref);
}

void ChannelBase::removeChannelGet(ChannelGet::shared_pointer const &ref)
{
    if(ChannelBaseDebug::getLevel()>0) {
         printf("ChannelBase::removeChannelGet\n");
    }
    Lock xx(mutex);
    if(beingDestroyed) return;
    channelGetList.erase(ref);
}

void ChannelBase::removeChannelPut(ChannelPut::shared_pointer const &ref)
{
    if(ChannelBaseDebug::getLevel()>0) {
         printf("ChannelBase::removeChannelPut\n");
    }
    Lock xx(mutex);
    if(beingDestroyed) return;
    channelPutList.erase(ref);
}

void ChannelBase::removeChannelPutGet(ChannelPutGet::shared_pointer const &ref)
{
    if(ChannelBaseDebug::getLevel()>0) {
         printf("ChannelBase::removeChannelPutGet\n");
    }
    Lock xx(mutex);
    if(beingDestroyed) return;
    channelPutGetList.erase(ref);
}

void ChannelBase::removeChannelMonitor(Monitor::shared_pointer const &ref)
{
    if(ChannelBaseDebug::getLevel()>0) {
         printf("ChannelBase::removeChannelMonitor\n");
    }
    Lock xx(mutex);
    if(beingDestroyed) return;
    channelMonitorList.erase(ref);
}

void ChannelBase::removeChannelRPC(ChannelRPC::shared_pointer const &ref)
{
    if(ChannelBaseDebug::getLevel()>0) {
         printf("ChannelBase::removeChannelRPC\n");
    }
    Lock xx(mutex);
    if(beingDestroyed) return;
    channelRPCList.erase(ref);
}

void ChannelBase::removeChannelArray(ChannelArray::shared_pointer const &ref)
{
    if(ChannelBaseDebug::getLevel()>0) {
         printf("ChannelBase::removeChannelArray\n");
    }
    Lock xx(mutex);
    if(beingDestroyed) return;
    channelArrayList.erase(ref);
}

String ChannelBase::getRequesterName()
{
    return requester->getRequesterName();
}

void ChannelBase::message(
        String const &message,
        MessageType messageType)
{
    requester->message(message,messageType);
}

ChannelProvider::shared_pointer ChannelBase::getProvider()
{
    return provider;
}

String ChannelBase::getRemoteAddress()
{
    return String("local");
}

Channel::ConnectionState ChannelBase::getConnectionState()
{
    return Channel::CONNECTED;
}

String ChannelBase::getChannelName()
{
    return channelName;
}

ChannelRequester::shared_pointer ChannelBase::getChannelRequester()
{
    return requester;
}

bool ChannelBase::isConnected()
{
    return true;
}

void ChannelBase::getField(GetFieldRequester::shared_pointer const &requester,
        String const &subField)
{
    Status status(Status::STATUSTYPE_ERROR,
        String("client asked for illegal field"));
    requester->getDone(status,FieldConstPtr());
}

AccessRights ChannelBase::getAccessRights(
        PVField::shared_pointer const &pvField)
{
    throw std::logic_error(String("Not Implemented"));
}

ChannelProcess::shared_pointer ChannelBase::createChannelProcess(
        ChannelProcessRequester::shared_pointer const & channelProcessRequester,
        PVStructure::shared_pointer const & pvRequest)
{
    Status status(Status::STATUSTYPE_ERROR,
        String("ChannelProcess not supported"));
    channelProcessRequester->channelProcessConnect(
        status,
        ChannelProcess::shared_pointer());
    return ChannelProcess::shared_pointer();
}

ChannelGet::shared_pointer ChannelBase::createChannelGet(
        ChannelGetRequester::shared_pointer const &channelGetRequester,
        PVStructure::shared_pointer const &pvRequest)
{
    Status status(Status::STATUSTYPE_ERROR,
        String("ChannelGet not supported"));
    channelGetRequester->channelGetConnect(
        status,
        ChannelGet::shared_pointer(),
        PVStructure::shared_pointer(),
        BitSet::shared_pointer());
    return ChannelGet::shared_pointer();
}

ChannelPut::shared_pointer ChannelBase::createChannelPut(
        ChannelPutRequester::shared_pointer const &channelPutRequester,
        PVStructure::shared_pointer const &pvRequest)
{
    Status status(Status::STATUSTYPE_ERROR,
        String("ChannelPut not supported"));
    channelPutRequester->channelPutConnect(
        status,
        ChannelPut::shared_pointer(),
        PVStructure::shared_pointer(),
        BitSet::shared_pointer());
    return ChannelPut::shared_pointer();
}

ChannelPutGet::shared_pointer ChannelBase::createChannelPutGet(
        ChannelPutGetRequester::shared_pointer const &channelPutGetRequester,
        PVStructure::shared_pointer const &pvRequest)
{
    Status status(Status::STATUSTYPE_ERROR,
        String("ChannelPutGet not supported"));
    channelPutGetRequester->channelPutGetConnect(
        status,
        ChannelPutGet::shared_pointer(),
        PVStructure::shared_pointer(),
        PVStructure::shared_pointer());
    return ChannelPutGet::shared_pointer();
}

ChannelRPC::shared_pointer ChannelBase::createChannelRPC(
        ChannelRPCRequester::shared_pointer const & channelRPCRequester,
        PVStructure::shared_pointer const & pvRequest)
{
    Status status(Status::STATUSTYPE_ERROR,
        String("ChannelRPC not supported"));
    channelRPCRequester->channelRPCConnect(status,ChannelRPC::shared_pointer());
    return ChannelRPC::shared_pointer();
}

Monitor::shared_pointer ChannelBase::createMonitor(
        MonitorRequester::shared_pointer const &monitorRequester,
        PVStructure::shared_pointer const &pvRequest)
{
    Status status(Status::STATUSTYPE_ERROR,
        String("ChannelMonitor not supported"));
    Monitor::shared_pointer thisPointer = dynamic_pointer_cast<Monitor>(getPtrSelf());
    monitorRequester->monitorConnect(
        status,
        thisPointer,
        StructureConstPtr());
    return Monitor::shared_pointer();
}

ChannelArray::shared_pointer ChannelBase::createChannelArray(
        ChannelArrayRequester::shared_pointer const &channelArrayRequester,
        PVStructure::shared_pointer const &pvRequest)
{
    Status status(Status::STATUSTYPE_ERROR,
        String("ChannelArray not supported"));
    channelArrayRequester->channelArrayConnect(
        status,
        ChannelArray::shared_pointer(),
        PVArray::shared_pointer());
    return ChannelArray::shared_pointer();
}

void ChannelBase::printInfo()
{
    printf("ChannelBase provides access to service\n");
}

void ChannelBase::printInfo(StringBuilder out)
{
    *out += "ChannelBase provides access to service";
}

}}
