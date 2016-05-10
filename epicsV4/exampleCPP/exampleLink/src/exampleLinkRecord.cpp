// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution

/* exampleLink.cpp */

/**
 * @author mrk
 * @date 2013.08.02
 */

#include <pv/standardPVField.h>
#include <pv/ntscalar.h>

#define epicsExportSharedSymbols
#include <pv/exampleLinkRecord.h>

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

ExampleLinkRecordPtr ExampleLinkRecord::create(
    PvaClientPtr  const & pva,
    string const & recordName,
    string const & providerName,
    string const & channelName)
{
    PVStructurePtr pvStructure = getStandardPVField()->scalarArray(
        pvDouble,"timeStamp");
    ExampleLinkRecordPtr pvRecord(
        new ExampleLinkRecord(
           recordName,pvStructure)); 
    if(!pvRecord->init(pva,channelName,providerName)) pvRecord.reset();
    return pvRecord;
}

ExampleLinkRecord::ExampleLinkRecord(
    string const & recordName,
    PVStructurePtr const & pvStructure)
: PVRecord(recordName,pvStructure)
{
}


bool ExampleLinkRecord::init(PvaClientPtr const & pva,string const & channelName,string const & providerName)
{
    initPVRecord();

    PVStructurePtr pvStructure = getPVRecordStructure()->getPVStructure();
    pvValue = pvStructure->getSubField<PVDoubleArray>("value");
    if(!pvValue) {
        return false;
    }
    PvaClientChannelPtr pvaClientChannel = pva->channel(channelName,providerName,0.0);
    monitorRequester = dynamic_pointer_cast<PvaClientMonitorRequester>(getPtrSelf());
    pvaClientMonitor = pvaClientChannel->monitor("value",monitorRequester);
    return true;
}

void ExampleLinkRecord::process()
{
    PVRecord::process();
}


void ExampleLinkRecord::event(PvaClientMonitorPtr const & monitor)
{
    while(monitor->poll()) {
        PVStructurePtr pvStructure = monitor->getData()->getPVStructure();
        PVDoubleArrayPtr pvDoubleArray = pvStructure->getSubField<PVDoubleArray>("value");
        if(!pvDoubleArray) throw std::runtime_error("value is not a double array");
        lock();
        try {
            beginGroupPut();
            pvValue->replace(pvDoubleArray->view());
            process();
            endGroupPut();
        } catch(...) {
           unlock();
           throw;
        }
        unlock();
        monitor->releaseEvent();
    }
}

}}}
