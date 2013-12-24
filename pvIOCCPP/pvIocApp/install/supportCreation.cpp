/* pvDatabase.cpp */
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
#include <pv/install.h>

namespace epics { namespace pvIOC { 

using namespace epics::pvData;
using namespace epics::pvAccess;


SupportCreation::SupportCreation(
    PVDatabase &pvDatabase,Requester &requester)
{
    throw std::logic_error(String("Not Implemented"));
}

SupportCreation::~SupportCreation()
{
    throw std::logic_error(String("Not Implemented"));
}

bool SupportCreation::createSupport()
{
    throw std::logic_error(String("Not Implemented"));
}

bool SupportCreation::initializeSupport()
{
    throw std::logic_error(String("Not Implemented"));
}

bool SupportCreation::startSupport(AfterStart &afterStart)
{
    throw std::logic_error(String("Not Implemented"));
}


}}
