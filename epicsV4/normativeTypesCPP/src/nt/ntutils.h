/* ntutils.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
#ifndef NTUTILS_H
#define NTUTILS_H

#include <string>
#include <shareLib.h>

namespace epics { namespace nt {

/**
 * Utility methods for NT types.
 * @author mse
 */
class epicsShareClass NTUtils {
public:

    /**
     * Checks whether NT types are compatible by checking their IDs,
     * i.e. their names and major version must match.
     * @param u1 the first uri.
     * @param u2 the second uri.
     * @return true of URIs are compatible, false otherwise.
     */
    static bool is_a(const std::string &u1, const std::string &u2);

private:
    // disable object creation
    NTUtils() {}
};

}}

#endif  /* NTUTILS_H */

