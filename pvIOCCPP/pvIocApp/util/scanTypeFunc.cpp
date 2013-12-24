/* scanTypeFunc.cpp */
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

namespace ScanTypeFunc {
    static const char* names[] = {
        "scanPassive", "scanEvent", "scanPeriodic"
    };
    const char* name(ScanType t) {
        if (t<int(scanPassive) || t>int(scanPeriodic))
            THROW_EXCEPTION2(std::invalid_argument, "logic error unknown ScanType");
        return names[t];
    }
    void toString(StringBuilder buf,const ScanType type) {
        *buf += name(type);
    }
} // namespace ScanTypeFunc


}}
