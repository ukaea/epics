/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS exampleCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */

#include <pv/pvData.h>
#include <pv/rpcService.h>
#include <pv/clientFactory.h>
#include <pv/rpcClient.h>

#include <string>
#include <iostream>

using namespace epics::pvData;


// Create the "data interface" required to send data to the hello service. That is,
// define the client side API of the hello service.
static StructureConstPtr makeRequestStructure()
{
    FieldCreatePtr factory = getFieldCreate();

    FieldConstPtrArray fields;
    StringArray names;

    names.push_back("personsname");
    fields.push_back(factory->createScalar(pvString));

    return factory->createStructure(names, fields);
}

// Set a pvAccess connection timeout, after which the client gives up trying 
// to connect to server.
const static double REQUEST_TIMEOUT = 3.0;


/**
 * The main establishes the connection to the helloServer, constructs the
 * mechanism to pass parameters to the server, calls the server in the EV4
 * 2-step way, gets the response from the helloServer, unpacks it, and
 * prints the greeting.
 * 
 * @param args - the name of person to greet. If not supplied then "anonymous".
  */
int main (int argc, char *argv[])
{
    // Start the pvAccess client side.
    epics::pvAccess::ClientFactory::start();

    try 
    {
        // Create the data instance used to send data to the server. That is,
        // instantiate an instance of the "introspection interface" for the data interface of
        // the hello server. The data interface was defined statically above.
        PVStructurePtr arguments(getPVDataCreate()->createPVStructure(makeRequestStructure()));

        // Get the value of the first input argument to this executable and use it 
        // to set the data to be sent to the server through the introspection interface. 
        std::string name = (argc > 1) ? argv[1] : "anonymous";
	    arguments->getSubField<PVString>("personsname")->put(name);

        // Create an RPC client to the "helloService" service
        epics::pvAccess::RPCClient::shared_pointer client
             = epics::pvAccess::RPCClient::create("helloService");

        // Create an RPC request and block until response is received. There is
        // no need to explicitly wait for connection; this method takes care of it.
        // In case of an error, an exception is thrown.
        PVStructurePtr response = client->request(arguments, REQUEST_TIMEOUT);

        // Extract the result using the introspection interface of the returned 
        // datum, 
        PVStringPtr greeting = response->getSubField<PVString>("greeting");

        // Check the result conforms to expected format and if so print it.
        if (!greeting)
        {
            std::cerr << "Acquisition of greeting was not successful. Response invalid" << std::endl;
        } 
        else
        {
            std::cout << greeting->get() << std::endl;
        }      
    }
    catch (epics::pvAccess::RPCRequestException & ex)
    {
        // The client connected to the server, but the server request method issued its 
        // standard summary exception indicating it couldn't complete the requested task.
        std::cerr << "Acquisition of greeting was not successful. RPCException:" << std::endl;
        std::cerr << ex.what() << std::endl;
    }
    catch (...)
    {
        // Catch any other exceptions so we always call ClientFactory::stop().
        std::cerr << "Acquisition of greeting was not successful. Unexpected exception." << std::endl;
    }

    // Stop pvAccess client, so that this application exits cleanly.
    epics::pvAccess::ClientFactory::stop();

    return 0;
}
