/* ntscalarArray.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */

#define epicsExportSharedSymbols
#include <pv/ntscalarArray.h>
#include <pv/ntutils.h>

using namespace std;
using namespace epics::pvData;

namespace epics { namespace nt {

static NTFieldPtr ntField = NTField::get();

namespace detail {

NTScalarArrayBuilder::shared_pointer NTScalarArrayBuilder::arrayValue(
        epics::pvData::ScalarType elementType
        )
{
    valueType = elementType;
    valueTypeSet = true;

    return shared_from_this();
}

StructureConstPtr NTScalarArrayBuilder::createStructure()
{
    if (!valueTypeSet)
        throw std::runtime_error("value array element type not set");

    FieldBuilderPtr builder =
            getFieldCreate()->createFieldBuilder()->
               setId(NTScalarArray::URI)->
               addArray("value", valueType);

    if (descriptor)
        builder->add("descriptor", pvString);

    if (alarm)
        builder->add("alarm", ntField->createAlarm());

    if (timeStamp)
        builder->add("timeStamp", ntField->createTimeStamp());

    if (display)
        builder->add("display", ntField->createDisplay());

    if (control)
        builder->add("control", ntField->createControl());

    size_t extraCount = extraFieldNames.size();
    for (size_t i = 0; i< extraCount; i++)
        builder->add(extraFieldNames[i], extraFields[i]);


    StructureConstPtr s = builder->createStructure();

    reset();
    return s;
}

NTScalarArrayBuilder::shared_pointer NTScalarArrayBuilder::addDescriptor()
{
    descriptor = true;
    return shared_from_this();
}

NTScalarArrayBuilder::shared_pointer NTScalarArrayBuilder::addAlarm()
{
    alarm = true;
    return shared_from_this();
}

NTScalarArrayBuilder::shared_pointer NTScalarArrayBuilder::addTimeStamp()
{
    timeStamp = true;
    return shared_from_this();
}

NTScalarArrayBuilder::shared_pointer NTScalarArrayBuilder::addDisplay()
{
    display = true;
    return shared_from_this();
}

NTScalarArrayBuilder::shared_pointer NTScalarArrayBuilder::addControl()
{
    control = true;
    return shared_from_this();
}

PVStructurePtr NTScalarArrayBuilder::createPVStructure()
{
    return getPVDataCreate()->createPVStructure(createStructure());
}

NTScalarArrayPtr NTScalarArrayBuilder::create()
{
    return NTScalarArrayPtr(new NTScalarArray(createPVStructure()));
}

NTScalarArrayBuilder::NTScalarArrayBuilder()
{
    reset();
}

void NTScalarArrayBuilder::reset()
{
    valueTypeSet = false;
    descriptor = false;
    alarm = false;
    timeStamp = false;
    display = false;
    control = false;
}

NTScalarArrayBuilder::shared_pointer NTScalarArrayBuilder::add(string const & name, FieldConstPtr const & field)
{
    extraFields.push_back(field); extraFieldNames.push_back(name);
    return shared_from_this();
}

}

const std::string NTScalarArray::URI("epics:nt/NTScalarArray:1.0");

NTScalarArray::shared_pointer NTScalarArray::wrap(PVStructurePtr const & structure)
{
    if(!isCompatible(structure)) return shared_pointer();
    return wrapUnsafe(structure);
}

NTScalarArray::shared_pointer NTScalarArray::wrapUnsafe(PVStructurePtr const & structure)
{
    return shared_pointer(new NTScalarArray(structure));
}

bool NTScalarArray::is_a(StructureConstPtr const & structure)
{
    return NTUtils::is_a(structure->getID(), URI);
}

bool NTScalarArray::isCompatible(PVStructurePtr const & pvStructure)
{
    if(!pvStructure) return false;
    PVScalarArrayPtr pvValue = pvStructure->getSubField<PVScalarArray>("value");
    if(!pvValue) return false;
    PVFieldPtr pvField = pvStructure->getSubField("descriptor");
    if(pvField && !pvStructure->getSubField<PVString>("descriptor")) return false;
    pvField = pvStructure->getSubField("alarm");
    if(pvField && !ntField->isAlarm(pvField->getField())) return false;
    pvField = pvStructure->getSubField("timeStamp");
    if(pvField && !ntField->isTimeStamp(pvField->getField())) return false;
    pvField = pvStructure->getSubField("display");
    if(pvField && !ntField->isDisplay(pvField->getField())) return false;
    pvField = pvStructure->getSubField("control");
    if(pvField && !ntField->isControl(pvField->getField())) return false;
    return true;
}


NTScalarArrayBuilderPtr NTScalarArray::createBuilder()
{
    return NTScalarArrayBuilderPtr(new detail::NTScalarArrayBuilder());
}

bool NTScalarArray::attachTimeStamp(PVTimeStamp &pvTimeStamp) const
{
    PVStructurePtr ts = getTimeStamp();
    if (ts)
        return pvTimeStamp.attach(ts);
    else
        return false;
}

bool NTScalarArray::attachAlarm(PVAlarm &pvAlarm) const
{
    PVStructurePtr al = getAlarm();
    if (al)
        return pvAlarm.attach(al);
    else
        return false;
}

bool NTScalarArray::attachDisplay(PVDisplay &pvDisplay) const
{
    PVStructurePtr dp = getDisplay();
    if (dp)
        return pvDisplay.attach(dp);
    else
        return false;
}

bool NTScalarArray::attachControl(PVControl &pvControl) const
{
    PVStructurePtr ctrl = getControl();
    if (ctrl)
        return pvControl.attach(ctrl);
    else
        return false;
}

PVStructurePtr NTScalarArray::getPVStructure() const
{
    return pvNTScalarArray;
}

PVStringPtr NTScalarArray::getDescriptor() const
{
    return pvNTScalarArray->getSubField<PVString>("descriptor");
}

PVStructurePtr NTScalarArray::getTimeStamp() const
{
    return pvNTScalarArray->getSubField<PVStructure>("timeStamp");
}

PVStructurePtr NTScalarArray::getAlarm() const
{
    return pvNTScalarArray->getSubField<PVStructure>("alarm");
}

PVStructurePtr NTScalarArray::getDisplay() const
{
    return pvNTScalarArray->getSubField<PVStructure>("display");
}

PVStructurePtr NTScalarArray::getControl() const
{
    return pvNTScalarArray->getSubField<PVStructure>("control");
}

PVFieldPtr NTScalarArray::getValue() const
{
    return pvValue;
}

NTScalarArray::NTScalarArray(PVStructurePtr const & pvStructure) :
    pvNTScalarArray(pvStructure), pvValue(pvNTScalarArray->getSubField("value"))
{}


}}
