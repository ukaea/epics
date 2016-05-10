// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution

#include <pv/pvData.h>
#include <pv/rpcService.h>
#include <pv/clientFactory.h>
#include <pv/rpcClient.h>

#include <string>
#include <iostream>

using namespace epics::pvData;


static StructureConstPtr makeDeviceStructure()
{
    static StructureConstPtr deviceStructure;
    if (deviceStructure.get() == 0)
    {
        FieldCreatePtr fieldCreate = getFieldCreate();

        deviceStructure = fieldCreate->createFieldBuilder()->
            add("x",pvDouble)->
            add("y",pvDouble)->
            createStructure();
    }
    return deviceStructure;
}


static StructureConstPtr makeArgumentStructure()
{
    static StructureConstPtr requestStructure;
    if (requestStructure.get() == 0)
    {
        FieldCreatePtr fieldCreate = getFieldCreate();

        requestStructure = fieldCreate->createFieldBuilder()->
            addArray("value", makeDeviceStructure())->
            createStructure();
    }
    return requestStructure;
}

// Set a pvAccess connection timeout, after which the client gives up trying 
// to connect to server.
const static double REQUEST_TIMEOUT = 3.0;
const static std::string DEVICE_NAME = "mydevice";
const static std::string APP_NAME = "move";

void usage()
{
    std::cout << "Usage: " << APP_NAME << " [x_1 y_1] ... [x_n y_n]\n"
              << "Sequentially sets the values of the x and y fields of "
              << DEVICE_NAME << " to (x_i,y_i).\n"
              << "Returns on completion."
              << std::endl;
}

/**
  */
int main (int argc, char *argv[])
{
    for (int i = 1; i < argc; ++i)
    {
        std::string arg(argv[i]);
        if (arg == "-h" || arg == "--help")
        {
            usage();
            return 0;
        }
    } 

    if ((argc % 2) != 1)
    {
        std::cerr << APP_NAME << " requires an even number of arguments."
                  << std::endl;
        usage();
        return 1;
    }
    // Start the pvAccess client side.
    epics::pvAccess::ClientFactory::start();

    try
    {
        PVStructurePtr arguments(getPVDataCreate()->createPVStructure(makeArgumentStructure()));

        PVStructureArray::svector values;

        for (int i = 1; i < argc; )
        {
            PVStructurePtr point(getPVDataCreate()->createPVStructure(makeDeviceStructure()));
            point->getSubField<PVDouble>("x")->put(atof(argv[i++]));
            point->getSubField<PVDouble>("y")->put(atof(argv[i++]));
            values.push_back(point);
        }

	    arguments->getSubField<PVStructureArray>("value")->replace(freeze(values));

        epics::pvAccess::RPCClient::shared_pointer client
             = epics::pvAccess::RPCClient::create(DEVICE_NAME);

        PVStructurePtr response = client->request(arguments,
            REQUEST_TIMEOUT + 1.0 * (argc/2));

        std::cout << "Done" << std::endl;
    }
    catch (epics::pvAccess::RPCRequestException & ex)
    {
        std::cerr << "Operation failed. RPCException:" << std::endl;
        std::cerr << ex.what() << std::endl;
    }
    catch (...)
    {
        // Catch any other exceptions so we always call ClientFactory::stop().
        std::cerr << "Unexpected exception." << std::endl;
    }

    // Stop pvAccess client, so that this application exits cleanly.
    epics::pvAccess::ClientFactory::stop();

    return 0;
}
