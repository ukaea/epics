/* support.cpp */
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

#include <pv/pvIntrospect.h>
#include <pv/pvData.h>
#include <pv/noDefaultMethods.h>
#include <pv/pvDatabase.h>

#include <pv/support.h>

namespace epics { namespace pvIOC { 

using namespace epics::pvData;
using namespace epics::pvAccess;

Support::Support(String name,PVRecordField &pvRecordField)
:  supportName(name),
   pvRecordField(pvRecordField),
   supportState(readyForInitialize)
{
    
}

Support::~Support()
{
}

epics::pvData::String Support::getRequesterName()
{
    return supportName;
}

void Support::message(String message,MessageType messageType)
{
    pvRecordField.message(message,messageType);
}

epics::pvData::String Support::getSupportName()
{
    return supportName;
}

SupportState Support::getSupportState()
{
    return supportState;
}

PVRecordField &Support::getPVRecordField()
{
    return pvRecordField;
}

void Support::initialize()
{
    setSupportState(readyForStart);
}

void Support::start(AfterStart &afterStart)
{
    setSupportState(ready);
}

void Support::stop()
{
    setSupportState(readyForStart);
}

void Support::uninitialize()
{
    if(supportState==ready) stop();
    setSupportState(readyForInitialize);
}

void Support::process(SupportProcessRequester &supportProcessRequester)
{
    supportProcessRequester.supportProcessDone(requestResultSuccess);
}

void Support::setSupportState(SupportState state)
{
    supportState = state;
}

bool Support::checkSupportState(
        SupportState expectedState,
        String message)
{
    if(expectedState==supportState) return true;
    message += String(" expected supportState ")
            + SupportStateFunc::name(expectedState)
            + "but state is " 
            + SupportStateFunc::name(supportState);
    pvRecordField.message(message,fatalErrorMessage);
    return false;
}

}}
