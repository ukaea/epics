/* eventScanner.cpp */
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

#include <pv/support.h>
#include <pv/pvDatabase.h>
#include <pv/util.h>

namespace epics { namespace pvIOC { 

using namespace epics::pvData;
using namespace epics::pvAccess;

EventScanner::EventScanner()
{
    throw std::logic_error(String("Not Implemented"));
}

EventScanner::~EventScanner()
{
    throw std::logic_error(String("Not Implemented"));
}

bool EventScanner::removeRecord(
        PVRecord &pvRecord, String eventName, ThreadPriority scanPriority)
{
    throw std::logic_error(String("Not Implemented"));
}

std::auto_ptr<EventAnnounce> EventScanner::addEventAnnouncer(
        String eventName, String announcer)
{
    throw std::logic_error(String("Not Implemented"));
}

void EventScanner::removeEventAnnouncer(
        EventAnnounce &eventAnnounce, String announcer)
{
    throw std::logic_error(String("Not Implemented"));
}

void EventScanner::toString(StringBuilder buf)
{
    throw std::logic_error(String("Not Implemented"));
}

void EventScanner::show( StringBuilder buf, String eventName)
{
    throw std::logic_error(String("Not Implemented"));
}

}}
