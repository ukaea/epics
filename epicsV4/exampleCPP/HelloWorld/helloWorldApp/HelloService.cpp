/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS exampleCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */


#include <pv/rpcServer.h>
#include "HelloService.h"

using namespace epics::pvData;

namespace epics
{

namespace helloService
{

// returns this service's result structure type definition.
StructureConstPtr makeResponseStructure()
{
    FieldCreatePtr factory = getFieldCreate();

    FieldConstPtrArray fields;
    StringArray names;

    names.push_back("greeting");
    fields.push_back(factory->createScalar(pvString));

    return factory->createStructure(names, fields);
}


// Definition of the Hello World RPC service.

epics::pvData::PVStructurePtr HelloService::request(
    epics::pvData::PVStructurePtr const & pvArgument
    ) throw (pvAccess::RPCRequestException)
{   
    // Extract the arguments. Just one in this case.
    // Report an error by throwing a RPCRequestException 
    epics::pvData::PVStringPtr nameField = pvArgument->getStringField("personsname");
    if (!nameField)
    {
        throw pvAccess::RPCRequestException(Status::STATUSTYPE_ERROR,
            "PVString field with name 'personsname' expected.");
    }

    // Create the result structure of the data interface.
    PVStructurePtr result(
        getPVDataCreate()->createPVStructure(makeResponseStructure()));

    // Extract from the constructed data interface the value of
    // "greeting" field. The value we'll return, is "Hello" concatenated
    // to the value of the input parameter called "personsname".
    PVStringPtr greetingValueField = result->getStringField("greeting");
	greetingValueField->put("Hello " + nameField->get());

    return result;
}

}

}
