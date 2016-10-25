/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 * @date 2016.06.17
 */

#include <pv/standardPVField.h>
#include <pv/ntscalar.h>

#define epicsExportSharedSymbols
#include <pv/doubleArrayRecord.h>

using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvaClient;
using namespace epics::pvDatabase;
using std::tr1::static_pointer_cast;
using std::tr1::dynamic_pointer_cast;
using std::cout;
using std::endl;
using std::string;

namespace epics { namespace exampleCPP { namespace exampleLink {

DoubleArrayRecordPtr DoubleArrayRecord::create(
    PvaClientPtr  const & pva,
    string const & recordName,
    string const & providerName,
    string const & channelName)
{
    PVStructurePtr pvStructure = getStandardPVField()->scalarArray(pvDouble,"timeStamp");
    DoubleArrayRecordPtr pvRecord(
        new DoubleArrayRecord(
           recordName,pvStructure)); 
    if(!pvRecord->init(pva,channelName,providerName)) pvRecord.reset();
    return pvRecord;
}

DoubleArrayRecord::DoubleArrayRecord(
    string const & recordName,
    PVStructurePtr const & pvStructure)
: PVRecord(recordName,pvStructure)
{
}


bool DoubleArrayRecord::init(PvaClientPtr const & pva,string const & channelName,string const & providerName)
{
    initPVRecord();
    return true;
}


}}}
