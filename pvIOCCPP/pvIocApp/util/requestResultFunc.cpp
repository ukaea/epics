/* requestResult.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 */
#include <string>
#include <stdexcept>
#include <memory>

#include <pv/pvIntrospect.h>
#include <pv/pvData.h>
#include <pv/noDefaultMethods.h>

#include <pv/support.h>
#include <pv/pvDatabase.h>
#include <pv/util.h>

namespace epics { namespace pvIOC { 

using namespace epics::pvData;
using namespace epics::pvAccess;

namespace RequestResultFunc {
    static const char* names[] = {
        "requestResultZombie", "requestResultSuccess", "requestResultFailure"
    };
    const char* name(RequestResult t) {
        if (t<int(requestResultZombie) || t>int(requestResultFailure))
            THROW_EXCEPTION2(std::invalid_argument, "logic error unknown RequestResult");
        return names[t];
    }
    void toString(StringBuilder buf,const RequestResult type) {
        *buf += name(type);
    }
} // namespace RequestResultFunc


}}
