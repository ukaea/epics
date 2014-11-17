/* exampleMonitorPlugin.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 * @date 2014.04.16
 */
#ifndef EXAMPLEMONITORPLUGIN_H
#define EXAMPLEMONITORPLUGIN_H

#ifdef epicsExportSharedSymbols
#   define examplemonitorPluginEpicsExportSharedSymbols
#   undef epicsExportSharedSymbols
#endif

#include <pv/pvDatabase.h>
#include <pv/monitorPlugin.h>

#ifdef examplemonitorPluginEpicsExportSharedSymbols
#   define epicsExportSharedSymbols
#	undef examplemonitorPluginEpicsExportSharedSymbols
#endif

#include <shareLib.h>


namespace epics { namespace pvDatabase { 

class epicsShareClass  ExampleMonitorPlugin{
public:
    static void create();
};


}}

#endif  /* EXAMPLEMONITORPLUGIN_H */
