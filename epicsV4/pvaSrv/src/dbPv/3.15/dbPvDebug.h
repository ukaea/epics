/**
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution.
 */
/**
 * @author mrk
 */
#ifndef DBPVDEBUG_H
#define DBPVDEBUG_H

namespace epics { namespace pvaSrv { 

class DbPvDebug {
public:
    static void setLevel(int level);
    static int getLevel();
};

}}

#endif  /* DBPVDEBUG_H */
