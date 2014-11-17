/* ntnameValue.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */

#include <pv/ntnameValue.h>

using namespace std;
using namespace epics::pvData;

namespace epics { namespace nt {
static NTFieldPtr ntField = NTField::get();

namespace detail {


NTNameValueBuilder::shared_pointer NTNameValueBuilder::value(
        epics::pvData::ScalarType scalarType
        )
{
    valueType = scalarType;
    valueTypeSet = true;

    return shared_from_this();
}

StructureConstPtr NTNameValueBuilder::createStructure()
{
    if (!valueTypeSet)
        throw std::runtime_error("value type not set");

    FieldBuilderPtr builder =
            getFieldCreate()->createFieldBuilder()->
               setId(NTNameValue::URI)->
               addArray("name", pvString)->
               addArray("value", valueType);

    if (descriptor)
        builder->add("descriptor", pvString);

    if (alarm)
        builder->add("alarm", ntField->createAlarm());

    if (timeStamp)
        builder->add("timeStamp", ntField->createTimeStamp());

    size_t extraCount = extraFieldNames.size();
    for (size_t i = 0; i< extraCount; i++)
        builder->add(extraFieldNames[i], extraFields[i]);

    StructureConstPtr s = builder->createStructure();

    reset();
    return s;
}

NTNameValueBuilder::shared_pointer NTNameValueBuilder::addDescriptor()
{
    descriptor = true;
    return shared_from_this();
}

NTNameValueBuilder::shared_pointer NTNameValueBuilder::addAlarm()
{
    alarm = true;
    return shared_from_this();
}

NTNameValueBuilder::shared_pointer NTNameValueBuilder::addTimeStamp()
{
    timeStamp = true;
    return shared_from_this();
}

PVStructurePtr NTNameValueBuilder::createPVStructure()
{
    return getPVDataCreate()->createPVStructure(createStructure());
}

NTNameValuePtr NTNameValueBuilder::create()
{
    return NTNameValuePtr(new NTNameValue(createPVStructure()));
}

NTNameValueBuilder::NTNameValueBuilder()
{
    reset();
}

void NTNameValueBuilder::reset()
{
    valueTypeSet = false;
    descriptor = false;
    alarm = false;
    timeStamp = false;
    extraFieldNames.clear();
    extraFields.clear();
}

NTNameValueBuilder::shared_pointer NTNameValueBuilder::add(string const & name, FieldConstPtr const & field)
{
    extraFields.push_back(field); extraFieldNames.push_back(name);
    return shared_from_this();
}


}

const std::string NTNameValue::URI("epics:nt/NTNameValue:1.0");

NTNameValue::shared_pointer NTNameValue::wrap(PVStructurePtr const & structure)
{
    if(!isCompatible(structure)) return shared_pointer();
    return wrapUnsafe(structure);
}

NTNameValue::shared_pointer NTNameValue::wrapUnsafe(PVStructurePtr const & structure)
{
    return shared_pointer(new NTNameValue(structure));
}

bool NTNameValue::is_a(StructureConstPtr const & structure)
{
    return structure->getID() == URI;
}

bool NTNameValue::isCompatible(PVStructurePtr const & pvStructure)
{
    if(!pvStructure) return false;
    PVStringArrayPtr pvName = pvStructure->getSubField<PVStringArray>("name");
    if(!pvName) return false;
    PVFieldPtr pvValue = pvStructure->getSubField("value");
    if(!pvValue) return false;
    PVFieldPtr pvField = pvStructure->getSubField("descriptor");
    if(pvField && !pvStructure->getSubField<PVString>("descriptor")) return false;
    pvField = pvStructure->getSubField("alarm");
    if(pvField && !ntField->isAlarm(pvField->getField())) return false;
    pvField = pvStructure->getSubField("timeStamp");
    if(pvField && !ntField->isTimeStamp(pvField->getField())) return false;
    return true;
}

NTNameValueBuilderPtr NTNameValue::createBuilder()
{
    return NTNameValueBuilderPtr(new detail::NTNameValueBuilder());
}

bool NTNameValue::attachTimeStamp(PVTimeStamp &pvTimeStamp) const
{
    PVStructurePtr ts = getTimeStamp();
    if (ts)
        return pvTimeStamp.attach(ts);
    else
        return false;
}

bool NTNameValue::attachAlarm(PVAlarm &pvAlarm) const
{
    PVStructurePtr al = getAlarm();
    if (al)
        return pvAlarm.attach(al);
    else
        return false;
}

PVStructurePtr NTNameValue::getPVStructure() const
{
    return pvNTNameValue;
}

PVStringPtr NTNameValue::getDescriptor() const
{
    return pvNTNameValue->getSubField<PVString>("descriptor");
}

PVStructurePtr NTNameValue::getTimeStamp() const
{
    return pvNTNameValue->getSubField<PVStructure>("timeStamp");
}

PVStructurePtr NTNameValue::getAlarm() const
{
    return pvNTNameValue->getSubField<PVStructure>("alarm");
}

PVStringArrayPtr NTNameValue::getName() const
{
    return pvNTNameValue->getSubField<PVStringArray>("name");
}

PVFieldPtr NTNameValue::getValue() const
{
    return pvNTNameValue->getSubField("value");
}

NTNameValue::NTNameValue(PVStructurePtr const & pvStructure) :
    pvNTNameValue(pvStructure)
{}


}}
