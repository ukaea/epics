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
#include <pv/examplePutLinkRecord.h>

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

ExamplePutLinkRecordPtr ExamplePutLinkRecord::create(
    PvaClientPtr  const & pva,
    string const & recordName,
    string const & providerName,
    string const & channelName)
{
    PVStructurePtr pvStructure = getStandardPVField()->scalarArray(pvDouble,"timeStamp");
    ExamplePutLinkRecordPtr pvRecord(
        new ExamplePutLinkRecord(
           recordName,pvStructure)); 
    if(!pvRecord->init(pva,channelName,providerName)) pvRecord.reset();
    return pvRecord;
}

ExamplePutLinkRecord::ExamplePutLinkRecord(
    string const & recordName,
    PVStructurePtr const & pvStructure)
: PVRecord(recordName,pvStructure)
{
}


bool ExamplePutLinkRecord::init(PvaClientPtr const & pva,string const & channelName,string const & providerName)
{
    initPVRecord();

    PVStructurePtr pvStructure = getPVRecordStructure()->getPVStructure();
    pvValue = pvStructure->getSubField<PVDoubleArray>("value");
    if(!pvValue) {
        return false;
    }
    pvaClientPut = pva->channel(channelName,providerName)->createPut();
    return true;
}

void ExamplePutLinkRecord::process()
{
    pvaClientPut->getData()->putDoubleArray(pvValue->view());
    pvaClientPut->put();
    PVRecord::process();
}


}}}
