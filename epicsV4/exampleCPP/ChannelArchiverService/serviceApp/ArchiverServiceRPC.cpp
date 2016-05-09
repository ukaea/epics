// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <memory>
#include <stdexcept>
#include <sstream>
#include <string>
#include <vector>
#include <float.h>

// EPICS Archiver Includes
#include <epicsVersion.h>
#include <alarm.h>
#include <epicsMath.h>
#include <AutoPtr.h>
#include <MsgLogger.h>
#include <RegularExpression.h>
#include <BinaryTree.h>
#include <AutoIndex.h>
#include <SpreadsheetReader.h>
#include <LinearReader.h>
#include <PlotReader.h>

#include <pv/rpcServer.h>

#include "ArchiverServiceRPC.h"
#include "common.h"


using namespace epics::pvData;
using epics::pvAccess::RPCRequestException;

namespace epics
{

namespace channelArchiverService
{

ArchiverServiceRPC::~ArchiverServiceRPC() {}

namespace
{

/**
 * Converts a string to a Long.
 * Throws an exception if conversion impossible.
 *
 * @param  str  the argument for the rpc
 * @return      the result of the conversion.
 */
int64_t toLong(const std::string & str)
{
    int64_t result = 0;
    std::stringstream ss(str);
    if (!(ss >> result))
    {
        throw RPCRequestException(Status::STATUSTYPE_ERROR,
            "Cannot convert string " + str + " to Long");
    }
    
    std::string remainder;
    if ((ss >> remainder) && remainder != "")
    {
        throw RPCRequestException(Status::STATUSTYPE_ERROR,
            "Cannot convert string " + str + " to Long");
    }

    return result;
}

}

/**
 * Fills in the list of table column labels
 */
void LabelTable(PVStructurePtr const & pvResult)
{
    PVStringArray::svector labels;
    labels.push_back("value");
    labels.push_back("secondsPastEpoch");
    labels.push_back("nanoseconds");
    labels.push_back("status");
    labels.push_back("severity");
    getStringArrayField(pvResult, "labels")->replace(freeze(labels));
}

ArchiverServiceRPC::ArchiverServiceRPC(char * indexFilename)
{
	indexes.push_back(indexFilename);
}

ArchiverServiceRPC::ArchiverServiceRPC(const std::vector<std::string> & indexFilenames)
{
	indexes.resize(indexFilenames.size());
	std::copy(indexFilenames.begin(), indexFilenames.end(), indexes.begin());
}

/**
 * Queries the EPICS R-Tree Channel Archiver, returning raw samples
 */
PVStructurePtr ArchiverServiceRPC::queryRaw(
    PVStructurePtr const & pvArgument,
    std::string & name, 
    const epicsTimeStamp & t0,
    const epicsTimeStamp & t1,
    int64_t maxRecords)
{
    // Create the result pvStructure

    PVStructurePtr pvResult(
        getPVDataCreate()->createPVStructure(makeArchiverResponseStructure(*getFieldCreate())));

    LabelTable(pvResult);

    // The result table is built up as one STL vector per column
    
    PVDoubleArray::svector values;
    PVLongArray::svector   secPastEpoch;
    PVIntArray::svector    nsec;
    PVIntArray::svector    stats;
    PVIntArray::svector    sevrs;

    int64_t recordCount = 0;

    for (std::vector<std::string>::const_iterator it = indexes.begin();
    	 it != indexes.end(); ++it)
    {
        // Open the Index
		AutoPtr<Index> index(new AutoIndex());

		try
		{
			index->open(it->c_str(), true);
		}
		catch(GenericException & e)
		{
			std::cout << e.what() << std::endl;
			throw RPCRequestException(Status::STATUSTYPE_ERROR, e.what());
		}

		const epicsTime start = t0;
		const epicsTime end = t1;

		// Create a Database Cursor

		AutoPtr<DataReader> reader(new RawDataReader(*index));

		// Seek to the first sample at or before 'start' for the named channel
		const RawValue::Data *data = 0;
		try
		{
			data = reader->find(stdString(name.c_str()), &start);
		}
		catch(...)
		{
			throw RPCRequestException(Status::STATUSTYPE_ERROR, "Error querying archive");
		}

		// find returns the reading immediately before start, unless start date is
		//  before first reading in archive, so skip to next.
		if((data != 0) && (RawValue::getTime(data) < start))
		{
			data = reader->next();
		}


		// Fill the table

		for(; recordCount < maxRecords; recordCount++)
		{
			if(data == 0)
			{
				break;
			}
			double value;

			// missing support for waveforms and strings

			RawValue::getDouble(reader->getType(), reader->getCount(), data, value, 0);
			epicsTimeStamp t = RawValue::getTime(data);

			if(end < t)
			{
				break;
			}

			int status = RawValue::getStat(data);
			int severity = RawValue::getSevr(data);

			values.push_back(value);
			secPastEpoch.push_back(t.secPastEpoch);
			nsec.push_back(t.nsec);
			stats.push_back(status);
			sevrs.push_back(severity);

			data = reader->next();

		}
    }
        
    // Pack the table into the pvStructure using some STL helper functions

    PVStructurePtr resultValues = pvResult->getSubField<PVStructure>("value");

    getDoubleArrayField(resultValues, "value")->replace(freeze(values));
    getLongArrayField(resultValues, "secondsPastEpoch")->replace(freeze(secPastEpoch));
    getIntArrayField(resultValues, "nanoseconds")->replace(freeze(nsec));
    getIntArrayField(resultValues, "status")->replace(freeze(stats));
    getIntArrayField(resultValues, "severity")->replace(freeze(sevrs));
        
    return pvResult;
}


/**
 * Queries the EPICS R-Tree Channel Archiver, returning raw samples
 */
PVStructurePtr ArchiverServiceRPC::request(PVStructurePtr const & pvArgument)
    throw (RPCRequestException)
{
    // Unpack the request type
    std::string name;
    int64_t start = 0;
    int64_t end   = std::numeric_limits<int32_t>::max();
    int64_t maxRecords = 1000000000; // limit to 1e9 values unless another number is specified

    std::string id = pvArgument->getField()->getID();

    // If type id is empty treat as pure request (NTURI.query sent as
    //request argument) otherwise check it's an NTURI with a query field

    PVStructurePtr query = pvArgument;

    if (id != Structure::DEFAULT_ID)
    {
        // Check type id is NTURI and major version is correct
        if (getTypeIdBasePlusMajor(id) != getTypeIdBasePlusMajor(ntURIStr))
        {
            // Fail request, reporting whether the type itself or the
            // major version was wrong 
            if (getTypeIdBase(id) != getTypeIdBase(ntURIStr))
            {
                throw RPCRequestException(Status::STATUSTYPE_ERROR,
                    "Request does not have correct id for normative type NTURI");
            }
            throw RPCRequestException(Status::STATUSTYPE_ERROR,
                "Request uses unsupported major version of NTURI");
        }

        query = pvArgument->getSubField<PVStructure>("query");
        if (!query)
        {
            throw RPCRequestException(Status::STATUSTYPE_ERROR,
                "No structure 'query' field (required for this service)");             
        }      
    }

    if (query->getSubField<PVString>(nameStr))
    {
        name = query->getSubField<PVString>(nameStr)->get();
        if (name == "")
        {
            throw RPCRequestException(Status::STATUSTYPE_ERROR, "Empty channel name");
        }
    }
    else
    {
        throw RPCRequestException(Status::STATUSTYPE_ERROR, "No channel name");
    }
        
    if (query->getSubField<PVString>(startStr))
    {
        start = toLong((query->getSubField<PVString>(startStr)->get()));
    }

    if (query->getSubField<PVString>(endStr))
    {
        end = toLong((query->getSubField<PVString>(endStr)->get()));
    }

    if (query->getSubField<PVString>(countStr))
    {
        maxRecords = toLong((query->getSubField<PVString>(countStr)->get()));
    }

    epicsTimeStamp t0, t1;

    t0.secPastEpoch = start;
    t0.nsec = 0;
    t1.secPastEpoch  = end;
    t1.nsec = 0;

    return queryRaw(pvArgument, name, t0, t1, maxRecords);
}

}

}
