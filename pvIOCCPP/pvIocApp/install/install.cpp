/* install.cpp */
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


Install & Install::getInstall()
{
    throw std::logic_error(String("Not Implemented"));
}

Install::Install()
{
    throw std::logic_error(String("Not Implemented"));
}

Install::~Install()
{
    throw std::logic_error(String("Not Implemented"));
}


bool Install::installStructures(String xmlFile,Requester &requester)
{
    throw std::logic_error(String("Not Implemented"));
}

bool Install::installStructures(PVDatabase &pvDatabase,Requester &requester)
{
    throw std::logic_error(String("Not Implemented"));
}

bool Install::installStructure(PVStructure &pvStructure,Requester &requester)
{
    throw std::logic_error(String("Not Implemented"));
}

bool Install::installRecords(String xmlFile,Requester &requester)
{
    throw std::logic_error(String("Not Implemented"));
}

bool Install::installRecords(PVDatabase &pvDatabase,Requester &requester)
{
    throw std::logic_error(String("Not Implemented"));
}

bool Install::installRecord(PVRecord &pvRecord,Requester &requester)
{
    throw std::logic_error(String("Not Implemented"));
}


}}
