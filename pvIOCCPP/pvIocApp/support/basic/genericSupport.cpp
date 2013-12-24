/* genericSupport.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 */
#include "basic.h"

namespace epics { namespace pvIOC { 

class GenericSupport :
     public Support,
     public SupportProcessRequester
{
public:
    GenericSupport(PVRecordField &pvRecordField);
    virtual ~GenericSupport() {}
    virtual void initialize();
    virtual void start(AfterStart &afterStart);
    virtual void stop();
    virtual void uninitialize();
    virtual void process(SupportProcessRequester &supportProcessRequester);
    virtual void supportProcessDone(RequestResult requestResult);
private:
    // TBD
};

GenericSupport::GenericSupport(PVRecordField &pvRecordField)
:  Support(epics::pvData::String("org.epics.ioc.generic"),pvRecordField)
{
     throw std::logic_error(String("Not Implemented"));
}

void GenericSupport::initialize()
{
    throw std::logic_error(String("Not Implemented"));
}

void GenericSupport::start(AfterStart &afterStart)
{
    throw std::logic_error(String("Not Implemented"));
}

void GenericSupport::stop()
{
    throw std::logic_error(String("Not Implemented"));
}

void GenericSupport::uninitialize()
{
    throw std::logic_error(String("Not Implemented"));
}

void GenericSupport::process(SupportProcessRequester &supportProcessRequester)
{
    throw std::logic_error(String("Not Implemented"));
}

void GenericSupport::supportProcessDone(RequestResult requestResult)
{
    throw std::logic_error(String("Not Implemented"));
}


std::auto_ptr<Support> genericSupportCreate(String name, PVRecordField &pvRecordField)
{
    return std::auto_ptr<Support>(new GenericSupport(name,pvRecordField));
}

}}
