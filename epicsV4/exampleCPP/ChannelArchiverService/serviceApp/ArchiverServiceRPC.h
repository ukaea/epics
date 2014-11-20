/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS exampleCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */


#ifndef ARCHIVERSERVICERPC_H
#define ARCHIVERSERVICERPC_H

#include <string>
#include <cstring>
#include <stdexcept>
#include <memory>

#include <pv/rpcServer.h>

namespace epics
{

namespace channelArchiverService
{
/**
 * ArchiverServiceRPC is the class which implements the Channel Archiver service
 * through the RPCService interface.
 */
class ArchiverServiceRPC : public epics::pvAccess::RPCService
{
public:
    POINTER_DEFINITIONS(ArchiverServiceRPC);

   /**
    * Constructs an instance of ArchiverServiceRPC which queries a given index file.
    *
    * @param  indexFilename  the path of the index file to be queried 
    */
    ArchiverServiceRPC(char * indexFilename);

    /**
     * Constructs an instance of ArchiverServiceRPC which queries the given index files.
     *
     * @param  indexFilenames  vector containing the the paths of the index files to
     *                         be queried
     */
    ArchiverServiceRPC(const std::vector<std::string> & indexFilenames);

    virtual ~ArchiverServiceRPC();

    epics::pvData::PVStructurePtr request(
        epics::pvData::PVStructurePtr const & args
    ) throw (epics::pvAccess::RPCRequestException);

private:
    epics::pvData::PVStructurePtr queryRaw(
        epics::pvData::PVStructurePtr const & pvArgument,
        std::string & name, 
        const epicsTimeStamp & t0,
        const epicsTimeStamp & t1,
        int64_t maxRecords);

    std::vector<std::string> indexes;   
};

}

}

#endif
