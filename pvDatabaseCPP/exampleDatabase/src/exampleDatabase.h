/* exampleDatabase.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
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

namespace epics { namespace pvDatabase { 

class epicsShareClass  ExampleDatabase{
public:
    static void create();
};


}}

#endif  /* EXAMPLEDATABASE_H */
