/* noopSupport.cpp */
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

//class Support does all the work
class NoopSupport : public Support {
public:
    NoopSupport(PVRecordField &pvRecordField)
    : Support(epics::pvData::String("org.epics.ioc.noop"),pvRecordField){}
    virtual ~NoopSupport() {}
}}

std::auto_ptr<Support> noopSupportCreate(String name, PVRecordField &pvRecordField)
{
    return std::auto_ptr<Support>(new NoopSupport(name,pvRecordField));
}
