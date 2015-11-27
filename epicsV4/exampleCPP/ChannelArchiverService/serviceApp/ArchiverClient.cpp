/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS exampleCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */

#include <algorithm>
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>


#include <epicsGetopt.h>
#include <epicsStdlib.h>
#include <epicsTime.h>

#include <RawValue.h>

#include <pv/pvData.h>
#include <pv/logger.h>
#include <pv/rpcService.h>
#include <pv/clientFactory.h>
#include <pv/rpcClient.h>

#include "common.h"
#include "ArchiverClientResponseHandler.h"


using namespace epics::pvData;

namespace epics
{

namespace channelArchiverService
{
/**
 * Verbosity levels for output and debug
 */
enum DebugLevel
{
    QUIET,
    NORMAL,
    VERBOSE
};

/**
 * Creates an NTURI request.
 *
 * @param  path          the value of the NTURI path field
 * @param  fieldnames    the names of fields in the NTURI query
 * @return values        the values of fields in the NTURI query
 */
PVStructurePtr createRequest(const std::string & path,
    const std::vector<std::string> & fieldnames,
    const std::vector<std::string> & values)
{    
    StructureConstPtr archiverStructure = makeRequestStructure(*getFieldCreate(), fieldnames);
    PVStructurePtr request(getPVDataCreate()->createPVStructure(archiverStructure));

    // set scheme.
    request->getSubField<PVString>("scheme")->put("pva");

    // set path.
    request->getSubField<PVString>("path")->put(path);

    // Set query.
    PVStructurePtr query = request->getSubField<PVStructure>("query");

    for (size_t i = 0; i < fieldnames.size(); ++i)
    {
        query->getSubField<PVString>(fieldnames[i])->put(values[i]);
    }
 
    return request;
}


/**
 * Converts the input string encoding the fields to be outputted into a
 * vector of output field enums.
 *
 * @param  inString         The input string encoding the fields to be displayed.
 * @param  fields           The result containing fields to be displayed.
 */
void makeOutputtedFields(const std::string & inString, std::vector<OutputField> & fields)
{
    for (size_t i = 0; i < inString.length();++i)
    {
        char fieldChar = inString[i];
        switch(fieldChar) 
        {
        case 't': 
            fields.push_back(REAL_TIME);            
            break;

        case 'v':
            fields.push_back(VALUE);   
            break;

        case 'D':
            fields.push_back(DATE); 
            break;

        case 'A':
           fields.push_back(ALARM); 
           break;

        case 's':
            fields.push_back(SECONDS_PAST_EPOCH); 
            break;

        case 'n':
            fields.push_back(NANO_SECONDS); 
            break;

        case 'S':
            fields.push_back(STATUS);
            break;

        case 'V':
            fields.push_back(SEVERITY); 
            break;

        default:
            break; 
        }
    }
}



/**
 * Displays help for ArchiverClient.
 *
 */
void showHelp()
{
    std::cout << "ArchiverClient [OPTIONS] channel...\n"
              << "ArchiverClient -h\n\n";

    std::cout << "Queries a Channel Archive Server for the data for the channel\n\n"; 

    std::cout << "Options:\n"
      << "-h              display help\n"
      << "-S=SERVICE      query SERVICE for results\n" 
      << "-s= START_TIME  query archive for results starting from START_TIME\n"
      << "                in seconds after EPICS epoch\n"
      << "-e=END_TIME     query archive for results up to END_TIME\n"
      << "                in seconds after EPICS epoch\n"
      << "-M=MAX          limit results to first MAX records\n"
      << "-f= FILENAME    output results of archiver query to file named FILENAME\n"
      << "-n              print channel name, preceded by #, before results\n"
      << "-t              transpose rows and columns\n"
      << "-T              print column headers, preceded by #, before results\n"
      << "                after channel name, if printed.\n"
      << "-q              suppress all output to standard out except for archive data.\n"
      << "-v              output verbose logging information.\n"  
      << "-x              results of archiver query request outputted using\n"
      << "                scientific (i.e. exponent/mantissa) format\n"
      << "-d              results of archiver query request outputted using\n"
      << "                scientific (i.e. exponent/mantissa) format\n"
      << "-p=PREC         results of archiver query request given to precision PREC\n"
      << "                Default value is 6.\n"
      << "-o              specifies which fields to output, in which order"
      << std::endl;
}

}

}

/**
 * Channel Archiver Service Client main.
 *
 */ 
int main (int argc, char *argv[])
{
    using namespace  epics::channelArchiverService;
  
    int opt;

    std::string serviceName;

    FormatParameters parameters; 
    bool printChannelName = false;
    std::string outputtedFields("tvDA");
    DebugLevel debugLevel = NORMAL;

    std::vector<std::string> queryFieldnames;
    std::vector<std::string> queryValues;

    queryFieldnames.push_back(nameStr);
    queryValues.push_back("");

    while ((opt = getopt(argc, argv, ":hS:s:e:M:f:ao:p:dxntTqv")) != -1)
    {
        switch (opt)
        {
        case 'h':
            showHelp();
            return 0;

        case 'S':
            serviceName = optarg;
            break;

        case 's':
            queryFieldnames.push_back(startStr);
            queryValues.push_back(optarg);
            break;

        case 'e':
            queryFieldnames.push_back(endStr);
            queryValues.push_back(optarg);
            break;

        case 'M':
            queryFieldnames.push_back(countStr);
            queryValues.push_back(optarg);
            break;

        case 'f':
            parameters.filename = optarg;
            break;

        case 'a':
            parameters.appendToFile = true;
            break;

        case 'o':
            outputtedFields = optarg;
            break;

        case 'p':
            parameters.precision = atoi(optarg);
            break;

        case 'd':
            parameters.format = FormatParameters::FIXED_POINT;
            break;

        case 'x':
            parameters.format = FormatParameters::SCIENTIFIC; 
            break;

        case 'n':
            printChannelName = true; 
            break;

        case 't':
            parameters.transpose = true; 
            break;

        case 'T':
            parameters.printColumnTitles = true; 
            break;

        case 'q':
            debugLevel = QUIET; 
            break;

        case 'v':
            debugLevel = VERBOSE; 
            break;

        case '?':
            std::cerr << "illegal option" << std::endl;
            break;
        }
    }

    epics::pvAccess::pvAccessSetLogLevel(
        (debugLevel == QUIET)  ? epics::pvAccess::logLevelOff  :
        (debugLevel == NORMAL) ? epics::pvAccess::logLevelInfo :
        epics::pvAccess::logLevelDebug);

    makeOutputtedFields(outputtedFields, parameters.outputtedFields);

    if (optind >= argc)
    {
        std::cerr << "Error: Too few arguments to ArchiverClient" << std::endl;
        return 1;    
    }

    epics::pvAccess::ClientFactory::start();

    try 
    {
        epics::pvAccess::RPCClient::shared_pointer
            client = epics::pvAccess::RPCClient::create(serviceName);

        for (int i = optind; i < argc; ++i)
        {
            std::string channel = argv[i];
            queryValues[0] = channel;

            if (debugLevel != QUIET)
            {
                std::cout << "Querying " << serviceName;
                for (size_t i = 0; i < queryFieldnames.size(); ++i)
                {
                    std::cout << ", " << queryFieldnames[i] << ":  " << queryValues[i]; 
                }
                std::cout << ")..." << std::endl;
            }

            if (printChannelName)
            {
                parameters.title = channel;
            }

            //  Create query and send to archiver service.
            PVStructurePtr queryRequest = createRequest(serviceName, queryFieldnames, queryValues);

            if (debugLevel == VERBOSE)
            {
                std::cout << "Query:" << std::endl;        
                std::cout << *queryRequest << std::endl;
            }

            double timeOut = 3.0;
            PVStructurePtr queryResponse = client->request(queryRequest, timeOut);

            if (!queryResponse)
            {
            	std::string errMsg = "RPC request failed";
            	throw epics::pvAccess::RPCRequestException(Status::STATUSTYPE_ERROR, errMsg);
            }

            if (debugLevel != QUIET)
            {
                std::cout << "Request successful. Processing results..." << std::endl;
            }

            if (debugLevel == VERBOSE)
            {
                std::cout << "Processing with parameters" << std::endl;

                if (parameters.filename != "")
                {
                    std::cout << "out file: " << parameters.filename
                              << (parameters.appendToFile ? " (append)" : " (overwrite)")
                              << std::endl;
                }
                std::cout << "precision: "    << parameters.precision << std::endl;
                std::cout << "format: ";
                switch (parameters.format)
                {
                case FormatParameters::SCIENTIFIC:
                    std::cout << "scientific";
                    break;

                case FormatParameters::FIXED_POINT:
                    std::cout << "fixed point";
                    break;

                case FormatParameters::DEFAULT:
                    std::cout << "default";
                    break;

                default:
                    std::cout << "?";          
                    break;
                }
                std::cout << std::endl;

                std::cout << "Output fields: "; 
                for (std::vector<OutputField>::const_iterator it = parameters.outputtedFields.begin();
                     it != parameters.outputtedFields.end(); ++it)
                {
                    if (it != parameters.outputtedFields.begin())
                    {
                        std::cout << ", ";
                    }
                    std::cout << outputFieldNames[*it];
                }
                std::cout  << std::endl;
            }

            RequestResponseHandler handler(parameters);
            handler.handle(queryResponse);

            if (debugLevel != QUIET)
            {
                if (handler.isOk())
                {
                    std::cout << "Done. ";
                    if (parameters.filename != "")
                    {
                        std::cout << "Results " << (parameters.appendToFile ? "appended" : "written")
                                  << " to " <<  parameters.filename << ".";
                    }
                    std::cout << std::endl;
                }
                else
                {
                    std::cout << "Processing unsuccessful." << std::endl;
                }
            }
            parameters.appendToFile = true; 
        }
    }
    catch (epics::pvAccess::RPCRequestException & ex)
    {
        std::cerr << "RPCException:" << std::endl;
        std::cerr << ex.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "Error: Request failed. Unexpected exception." << std::endl;
    }

    epics::pvAccess::ClientFactory::stop();
 
    return 0;
}
