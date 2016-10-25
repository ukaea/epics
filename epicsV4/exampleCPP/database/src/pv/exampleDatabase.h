/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 * @date 2013.07.24
 */
#ifndef EXAMPLEDATABASE_H
#define EXAMPLEDATABASE_H

#ifdef epicsExportSharedSymbols
#   define exampledatabaseEpicsExportSharedSymbols
#   undef epicsExportSharedSymbols
#endif

#include <pv/pvDatabase.h>

#ifdef exampledatabaseEpicsExportSharedSymbols
#   define epicsExportSharedSymbols
#	undef exampledatabaseEpicsExportSharedSymbols
#endif

#include <shareLib.h>

namespace epics { namespace exampleCPP { namespace database {

class epicsShareClass  ExampleDatabase{
public:
    static void create();
};


}}}

#endif  /* EXAMPLEDATABASE_H */
