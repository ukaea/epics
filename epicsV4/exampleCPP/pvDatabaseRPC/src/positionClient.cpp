/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */


#include <pv/pvData.h>
#include <pv/rpcService.h>
#include <pv/clientFactory.h>
#include <pv/rpcClient.h>

#include <string>
#include <iostream>

using namespace epics::pvData;

static StructureConstPtr makeRequestStructure()
{
    static StructureConstPtr requestStructure;
    if (requestStructure.get() == 0)
    {
        FieldCreatePtr fieldCreate = getFieldCreate();

        requestStructure = fieldCreate->createFieldBuilder()->
            add("method", pvString)->
            createStructure();
    }
    return requestStructure;
}

static StructureConstPtr makePointStructure()
{
    static StructureConstPtr pointStructure;
    if (pointStructure.get() == 0)
    {
        FieldCreatePtr fieldCreate = getFieldCreate();

        pointStructure = fieldCreate->createFieldBuilder()->
            setId("point_t")->
            add("x",pvDouble)->
            add("y",pvDouble)->
            createStructure();
    }
    return pointStructure;
}

static StructureConstPtr makeArgumentStructure()
{
    static StructureConstPtr argStructure;
    if (argStructure.get() == 0)
    {
        FieldCreatePtr fieldCreate = getFieldCreate();

        argStructure = fieldCreate->createFieldBuilder()->
            createStructure();
    }
    return argStructure;
}

static StructureConstPtr makeRewindArgumentStructure()
{
    static StructureConstPtr argStructure;
    if (argStructure.get() == 0)
    {
        FieldCreatePtr fieldCreate = getFieldCreate();

        argStructure = fieldCreate->createFieldBuilder()->
            add("value", pvInt)->
            createStructure();
    }
    return argStructure;
}

static StructureConstPtr makeConfigureArgumentStructure()
{
    static StructureConstPtr argStructure;
    if (argStructure.get() == 0)
    {
        FieldCreatePtr fieldCreate = getFieldCreate();

        argStructure = fieldCreate->createFieldBuilder()->
            addArray("value", makePointStructure())->
            createStructure();
    }
    return argStructure;
}

// Set a pvAccess connection timeout, after which the client gives up trying 
// to connect to server.
const static double DEFAULT_TIMEOUT = 3.0;
const static std::string DEVICE_NAME = "mydevice";
const static std::string APP_NAME = "control";



void usage_help() {
    std::cout <<  "Usage:\n"
              <<  "For help on commands run supplying the arguments:\n"
              <<  "help <command>\n"
              <<  std::endl;
    }

void usage_configure()
{
    std::cout <<  "Usage:\n"
              <<  "Run application supplying the arguments:\n"
              <<  "configure x_1 y_1 [x_2 y_2] ... [x_n y_n]\n"
              <<  "Sets the sequence of points through which "
              <<  DEVICE_NAME + " will move\n"
              <<  "to (x_i,y_i), i = 1..n"
              <<  " and changes state to READY.\n"
              <<  "Device must be IDLE.\n"
              <<  std::endl;
}

void usage_run()
{
    std::cout <<  "Usage:\n"
              <<  "Run application supplying the argument:\n"
              <<  "run\n"
              <<  "Starts the device moving through the sequence of points supplied on\n"
              <<  "configuration, changes state to RUNNING and returns.\n"
              <<  "No additional arguments are required.\n"
              <<  "Device must be READY.\n"
              <<  std::endl;
}

void usage_scan()
{
    std::cout <<  "Usage:\n"
              <<  "Run application supplying the argument:\n"
              <<  "scan\n"
              <<  "Starts the device moving through the sequence of points supplied on\n"
              <<  "configuration, changes state to RUNNING and blocks until completion.\n"
              <<  "No additional arguments are required.\n"
              <<  "Returns an error if scan is stopped or aborted.\n"
              <<  "Device must be READY.\n"
              <<  std::endl;
}

void usage_pause()
{
    std::cout <<  "Usage:\n"
              <<  "Run application supplying the argument:\n"
              <<  "pause\n"
              <<  "Pauses the current scan and changes state to PAUSED.\n"
              <<  "No additional arguments are required.\n"
              <<  "Device must be RUNNING.\n"
              <<  std::endl;
}

void usage_resume()
{
    std::cout <<  "Usage:\n"
              <<  "Run application supplying the argument:\n"
              <<  "resume\n"
              <<  "Resumes the current scan and changes state to RUNNING.\n"
              <<  "No additional arguments are required.\n"
              <<  "Device must be PAUSED.\n"
              <<  std::endl;
}

void usage_rewind()
{
    std::cout <<  "Usage:\n"
              <<  "Run application supplying the arguments:\n"
              <<  "rewind <steps>\n"
              <<  "Rewinds the current scan the requested number of steps or to start.\n"
              <<  "Required argument is the number of steps, which must be non-negative.\n"
              <<  "Device must be RUNNING or PAUSED.\n"
              <<  std::endl;
}

void usage_stop()
{
    std::cout <<  "Usage:\n"
              <<  "Run application supplying the argument:\n"
              <<  "stop\n"
              <<  "Stops any scan in progress and changes state to READY.\n"
              <<  "Blocking scan operation in progress will return an error.\n"
              <<  "No additional arguments are required.\n"
              <<  "Device can be any state except IDLE.\n"
              <<  std::endl;
}

void usage_abort()
{
    std::cout <<  "Usage:\n"
              <<  "Run application supplying the argument:\n"
              <<  "abort\n"
              <<  "Aborts any scan or motion in progress and changes state to IDLE.\n"
              <<  "Blocking scan operation in progress will return an error.\n"
              <<  "No additional arguments are required.\n"
              <<  "Device can be any state.\n"
              <<  std::endl;
}

void usage();

void usage(const std::string & command)
{
    if (command == std::string("configure"))
            usage_configure();
    else if (command == std::string("run"))
            usage_run();
    else if (command == std::string("scan"))
            usage_scan();
    else if (command == std::string("pause"))
            usage_pause();
    else if (command == std::string("resume"))
            usage_resume();
    else if (command == std::string("rewind"))
            usage_rewind();
    else if (command == std::string("stop"))
            usage_stop();
    else if (command == std::string("abort"))
            usage_abort();
    else if (command == std::string("help"))
            usage_help();
    else
    {
        std::cout << "Unknown command " << command;
        usage();
    }
}



void usage()
{
    std::cout <<  "Usage:\n"
              <<  "Run application supplying the arguments:\n"
              <<  " <command> [<command-arguments>]\n"
              <<  "Controls a device (" + DEVICE_NAME + ")\n" 
              <<  "Available commands are:\n"
              <<  "help\n"
              <<  "configure\n"
              <<  "run\n"
              <<  "pause\n"
              <<  "resume\n"
              <<  "rewind\n"
              <<  "stop\n"
              <<  "scan\n"
              <<  "abort.\n"
              << "<command-arguments> is command-dependent.\n"
              <<  "For help on commands run supplying the arguments:\n"
              <<  "help <command>\n"
              <<  std::endl;
}


PVStructurePtr createRewindArgs(const std::string & arg)
{
    PVStructurePtr arguments(getPVDataCreate()->createPVStructure(makeRewindArgumentStructure()));

    arguments->getSubFieldT<PVInt>("value")->put(atoi(arg.c_str()));
    return arguments;
}


PVStructurePtr createConfigArgs(int argc, char *argv[])
{
    PVStructurePtr arguments(getPVDataCreate()->createPVStructure(makeConfigureArgumentStructure()));

    PVStructureArray::svector values;

    for (int i = 2; i < argc; )
    {
        PVStructurePtr point(getPVDataCreate()->createPVStructure(makePointStructure()));
        point->getSubField<PVDouble>("x")->put(atof(argv[i++]));
        point->getSubField<PVDouble>("y")->put(atof(argv[i++]));
        values.push_back(point);
    }

	arguments->getSubField<PVStructureArray>("value")->replace(freeze(values));
    return arguments;
}

/**
  */
int main (int argc, char *argv[])
{
    if (argc < 2)
    {
        usage();
        return 1;
    }

    for (int i = 1; i < argc; ++i)
    {
        std::string arg(argv[i]);
        if (arg == "-h" || arg == "--help")
        {
            usage();
            return 0;
        }
    }

    std::string command(argv[1]);

    if (command == std::string("help"))
    {
        if(argc < 3)
        {
            usage();
            return 0;
        }
        else
        {
            usage(argv[2]);
            return 0;
        }
    }

    if (command == std::string("configure"))
    {
        if(argc < 4)
        {
            std::cerr << "configure  must have at least one pair of position coordinate arguments." << std::endl;
            usage_configure();
            return 1;
        }

        if ((argc % 2) != 0)
        {
            std::cerr << "configure requires an even number of arguments."
                      <<  std::endl;
            usage_configure();
            return 1;
        }
    }

    if (command == std::string("rewind") && argc < 3)
    {
        std::cerr << "rewind requires an argument." <<  std::endl;
        usage_rewind();
        return 1;
    }

    PVDataCreatePtr pvDataCreate = getPVDataCreate();

    PVStructurePtr pvArguments;
    if (command == std::string("configure"))
        pvArguments = createConfigArgs(argc, argv);
    else if (command == std::string("rewind"))
        pvArguments = createRewindArgs(argv[2]);
    else
        pvArguments = pvDataCreate->createPVStructure(makeArgumentStructure());

    // Start the pvAccess client side.
    epics::pvAccess::ClientFactory::start();
    try
    {
        PVStructureArray::svector values;

        PVStructurePtr pvRequest = getPVDataCreate()->createPVStructure(makeRequestStructure());
        pvRequest->getSubFieldT<PVString>("method")->put(command);

        epics::pvAccess::RPCClient::shared_pointer client
             = epics::pvAccess::RPCClient::create(DEVICE_NAME, pvRequest);

        double timeout = DEFAULT_TIMEOUT;
        if (command == std::string("scan"))
            timeout = 1e9;

        PVStructurePtr response = client->request(pvArguments, timeout);

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
