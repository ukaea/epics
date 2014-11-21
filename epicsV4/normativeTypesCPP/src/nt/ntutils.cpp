/* ntutils.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */

#define epicsExportSharedSymbols
#include <pv/ntutils.h>

using namespace std;

namespace epics { namespace nt {

bool NTUtils::is_a(const std::string &u1, const std::string &u2)
{
    // remove minor for the u2
    size_t pos = u2.find_last_of('.');
    std::string su2 = (pos == string::npos) ? u2 : u2.substr(0, pos);

    // "starts with comparison"
    return su2.size() <= u1.size() && u1.compare(0, su2.size(), su2) == 0;
}

}}
