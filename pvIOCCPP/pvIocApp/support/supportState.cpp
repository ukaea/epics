/* supportState.cpp */
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
#include <pv/pvDatabase.h>
#include <pv/pvEnumerated.h>

#include <pv/support.h>

namespace epics { namespace pvIOC { 

using namespace epics::pvData;
using namespace epics::pvAccess;

String SupportStateFunc::supportStateNames[] =
{
    String("readyForInitialize"),
    String("readyForStart"),
    String("ready"),
    String("zombie")
};

SupportState SupportStateFunc::getSupportState(int value)
{
    switch(value) {
        case 0: return readyForInitialize;
        case 1: return readyForStart;
        case 2: return ready;
        case 3: return zombie;
    }
    String message("SupportState getSupportState(");
    message += value;
    message += ") is not a valid SupportState";
    throw std::invalid_argument(String(message));
}

const char *SupportStateFunc::name(SupportState state)
{
    switch(state) {
    case readyForInitialize: return "readyForInitialize";
    case readyForStart: return "readyForStart";
    case ready: return "ready";
    case zombie: return "zombie";
    }
    return "unknown state";
}

std::auto_ptr<PVEnumerated> getSupportState(PVField &pvField)
{
    std::auto_ptr<PVEnumerated> pvEnumerated
        = std::auto_ptr<PVEnumerated>(new PVEnumerated());
    if(!pvEnumerated->attach(&pvField)) return std::auto_ptr<PVEnumerated>(0);
    StringArray choices = pvEnumerated->getChoices();
    int len = pvEnumerated->getNumberChoices();
    for (int i=0; i<len; i++) {
        if(choices[i].compare(SupportStateFunc::supportStateNames[i])!=0) {
            pvField.message("not an supportState structure", errorMessage);
                return std::auto_ptr<PVEnumerated>(0);
        }
    }
    return pvEnumerated;
}

}}
