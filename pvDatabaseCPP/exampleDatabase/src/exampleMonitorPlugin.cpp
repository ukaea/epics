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

#include <pv/convert.h>
#include <pv/monitorPlugin.h>

#define epicsExportSharedSymbols
#include <pv/exampleMonitorPlugin.h>

using namespace epics::pvData;
using std::cout;
using std::endl;
using std::string;

namespace epics { namespace pvDatabase { 

static string pluginName("onChange");
static ConvertPtr convert(getConvert());

class OnChangePlugin;
typedef std::tr1::shared_ptr<OnChangePlugin> OnChangePluginPtr;
class OnChangePluginCreator;
typedef std::tr1::shared_ptr<OnChangePluginCreator> OnChangePluginCreatorPtr;

class OnChangePlugin : public MonitorPlugin
{
public:
    virtual ~OnChangePlugin(){}
    OnChangePlugin() {}
    bool init(
        FieldConstPtr const &field,
        StructureConstPtr const &top,
        PVStructurePtr const &pvFieldOptions)
   {
        pvField = getPVDataCreate()->createPVField(field);
        raiseMonitor = true;
        if(pvFieldOptions) {
            PVStringPtr pvString =
                pvFieldOptions->getSubField<PVString>("raiseMonitor");
                if(pvString) {
                    string value = pvString->get();
                    if(value.compare("false")==0) raiseMonitor = false;
                }
        }
        return true;
   }
   virtual string &getName(){return pluginName;}
   virtual bool causeMonitor(
        PVFieldPtr const &pvNew,
        PVStructurePtr const &pvTop,
        MonitorElementPtr const &monitorElement)
   {
       bool isSame = convert->equals(pvNew,pvField);
       if(isSame) return false;
       convert->copy(pvNew,pvField);
       return raiseMonitor;
   }
private:
   PVFieldPtr pvField;
   bool raiseMonitor;
};

class OnChangePluginCreator : public MonitorPluginCreator
{
public:
    virtual string &getName(){return pluginName;}
    virtual MonitorPluginPtr create(
        FieldConstPtr const &field,
        StructureConstPtr const &top,
        PVStructurePtr const &pvFieldOptions)
   {
       OnChangePluginPtr plugin(new OnChangePlugin());
       bool result = plugin->init(field,top,pvFieldOptions);
       if(!result) return MonitorPluginPtr();
       return plugin;
   }

};

void ExampleMonitorPlugin::create()
{
    static OnChangePluginCreatorPtr plugin;
    static Mutex mutex;
    Lock xx(mutex);
    if(!plugin) {
        plugin = OnChangePluginCreatorPtr(new OnChangePluginCreator());
        MonitorPluginManager::get()->addPlugin(pluginName,plugin);
    }
}


}}

