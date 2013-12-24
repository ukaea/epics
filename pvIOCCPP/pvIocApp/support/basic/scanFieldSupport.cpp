/* scanFieldSupport.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 */

#include <pv/basicSupport.h>

namespace epics { namespace pvIOC { 

class ScanFieldSupport :
     public Support,
     public SupportProcessRequester
{
public:
    ScanFieldSupport(PVRecordField &pvRecordField)
    : Support(epics::pvData::String("org.epics.ioc.generic"),pvRecordField){}
    virtual ~ScanFieldSupport() {}
    virtual void initialize();
    virtual void start(AfterStart &afterStart);
    virtual void stop();
    virtual void uninitialize();
    virtual void process(SupportProcessRequester &supportProcessRequester);
    virtual void supportProcessDone(RequestResult requestResult);
private:
    // TBD
}}

ScanFieldSupport:: ScanFieldSupport(PVRecordField &pvRecordField)
: Support(epics::pvData::String("org.epics.ioc.generic"),
  pvRecordField)
{
    throw std::logic_error(String("Not Implemented"));
}

void ScanFieldSupport::initialize()
{
    throw std::logic_error(String("Not Implemented"));
}

void ScanFieldSupport::start(AfterStart &afterStart)
{
    throw std::logic_error(String("Not Implemented"));
}

void ScanFieldSupport::stop()
{
    throw std::logic_error(String("Not Implemented"));
}

void ScanFieldSupport::uninitialize()
{
    throw std::logic_error(String("Not Implemented"));
}

void ScanFieldSupport::process(SupportProcessRequester &supportProcessRequester)
{
    throw std::logic_error(String("Not Implemented"));
}

void ScanFieldSupport::supportProcessDone(RequestResult requestResult)
{
    throw std::logic_error(String("Not Implemented"));
}


std::auto_ptr<Support> scanFieldSupportCreate(
    String name, PVRecordField &pvRecordField)
{
    return std::auto_ptr<Support>(new ScanFieldSupport(name,pvRecordField));
}
