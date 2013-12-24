/* scanField.cpp */
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

ScanField::ScanField(PVRecord& pvRecord)
{
    throw std::logic_error(String("Not Implemented"));
}

ScanField::~ScanField()
{
    throw std::logic_error(String("Not Implemented"));
}

std::auto_ptr<ScanField> ScanField::create(PVRecord& pvRecord)
{
    throw std::logic_error(String("Not Implemented"));
}

ThreadPriority ScanField::getPriority()
{
    throw std::logic_error(String("Not Implemented"));
}

PVInt & ScanField::getPriorityIndexPV()
{
    throw std::logic_error(String("Not Implemented"));
}

ScanType ScanField::getScanType()
{
    throw std::logic_error(String("Not Implemented"));
}

PVInt & ScanField::getScanTypeIndexPV()
{
    throw std::logic_error(String("Not Implemented"));
}

double ScanField::getRate()
{
    throw std::logic_error(String("Not Implemented"));
}

PVDouble & ScanField::getRatePV()
{
    throw std::logic_error(String("Not Implemented"));
}

String ScanField::getEventName()
{
    throw std::logic_error(String("Not Implemented"));
}

PVString & ScanField::getEventNamePV()
{
    throw std::logic_error(String("Not Implemented"));
}

bool ScanField::getSingleProcessRequester()
{
    throw std::logic_error(String("Not Implemented"));
}

PVBoolean & ScanField::getSingleProcessRequesterPV()
{
    throw std::logic_error(String("Not Implemented"));
}

bool ScanField::getProcessAfterStart()
{
    throw std::logic_error(String("Not Implemented"));
}

PVBoolean & ScanField::getProcessAfterStartPV()
{
    throw std::logic_error(String("Not Implemented"));
}

}}
