/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 * @date 2013.08.02
 */

#include <pv/standardPVField.h>
#include <pv/ntscalar.h>

#define epicsExportSharedSymbols
#include <pv/exampleGetLinkRecord.h>

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

ExampleGetLinkRecordPtr ExampleGetLinkRecord::create(
    PvaClientPtr  const & pva,
    string const & recordName,
    string const & providerName,
    string const & channelName)
{
    PVStructurePtr pvStructure = getStandardPVField()->scalarArray(pvDouble,"timeStamp");
    ExampleGetLinkRecordPtr pvRecord(
        new ExampleGetLinkRecord(
           recordName,pvStructure)); 
    if(!pvRecord->init(pva,channelName,providerName)) pvRecord.reset();
    return pvRecord;
}

ExampleGetLinkRecord::ExampleGetLinkRecord(
    string const & recordName,
    PVStructurePtr const & pvStructure)
: PVRecord(recordName,pvStructure)
{
}


bool ExampleGetLinkRecord::init(PvaClientPtr const & pva,string const & channelName,string const & providerName)
{
    initPVRecord();

    PVStructurePtr pvStructure = getPVRecordStructure()->getPVStructure();
    pvValue = pvStructure->getSubField<PVDoubleArray>("value");
    if(!pvValue) {
        return false;
    }
    pvaClientGet = pva->channel(channelName,providerName)->createGet();
    return true;
}

void ExampleGetLinkRecord::process()
{
    pvaClientGet->get();
    shared_vector<const double> value = pvaClientGet->getData()->getDoubleArray();
    pvValue->replace(value);
    PVRecord::process();
}


}}}
