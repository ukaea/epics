/* ntmultiChannel.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */

#include <pv/ntmultiChannel.h>
#include <algorithm>

using namespace std;
using namespace epics::pvData;

namespace epics { namespace nt { 


static FieldCreatePtr fieldCreate = getFieldCreate();
static PVDataCreatePtr pvDataCreate = getPVDataCreate();
static NTFieldPtr ntField = NTField::get();

namespace detail {

NTMultiChannelBuilder::shared_pointer NTMultiChannelBuilder::value(UnionConstPtr valuePtr)
{
    valueType = valuePtr;
    return shared_from_this();
}


NTMultiChannelBuilder::shared_pointer NTMultiChannelBuilder::addDescriptor()
{
    descriptor = true;
    return shared_from_this();
}

NTMultiChannelBuilder::shared_pointer NTMultiChannelBuilder::addAlarm()
{
    alarm = true;
    return shared_from_this();
}

NTMultiChannelBuilder::shared_pointer NTMultiChannelBuilder::addTimeStamp()
{
    timeStamp = true;
    return shared_from_this();
}

NTMultiChannelBuilder::shared_pointer NTMultiChannelBuilder::addSeverity()
{
    severity = true;
    return shared_from_this();
}

NTMultiChannelBuilder::shared_pointer NTMultiChannelBuilder::addStatus()
{
    status = true;
    return shared_from_this();
}

NTMultiChannelBuilder::shared_pointer NTMultiChannelBuilder::addMessage()
{
    message = true;
    return shared_from_this();
}

NTMultiChannelBuilder::shared_pointer NTMultiChannelBuilder::addSecondsPastEpoch()
{
    secondsPastEpoch = true;
    return shared_from_this();
}

NTMultiChannelBuilder::shared_pointer NTMultiChannelBuilder::addNanoseconds()
{
    nanoseconds = true;
    return shared_from_this();
}

NTMultiChannelBuilder::shared_pointer NTMultiChannelBuilder::addUserTag()
{
    userTag = true;
    return shared_from_this();
}

StructureConstPtr NTMultiChannelBuilder::createStructure()
{
    StandardFieldPtr standardField = getStandardField();
    size_t nfields = 3;
    size_t extraCount = extraFieldNames.size();
    nfields += extraCount;
    if(descriptor) ++nfields;
    if(alarm) ++nfields;
    if(timeStamp) ++nfields;
    if(severity) ++nfields;
    if(status) ++nfields;
    if(message) ++nfields;
    if(secondsPastEpoch) ++nfields;
    if(nanoseconds) ++nfields;
    if(userTag) ++nfields;
    FieldConstPtrArray fields(nfields);
    StringArray names(nfields);
    size_t ind = 0;
    names[ind] = "value";
    if(valueType) {
        fields[ind++] =  fieldCreate->createUnionArray(valueType);
    } else {
        fields[ind++] =  fieldCreate->createVariantUnionArray();
    }
    names[ind] = "channelName";
    fields[ind++] =  fieldCreate->createScalarArray(pvString);
    names[ind] = "isConnected";
    fields[ind++] =  fieldCreate->createScalarArray(pvBoolean);
    if(timeStamp) {
        names[ind] = "timeStamp";
        fields[ind++] = standardField->timeStamp();
    }
    if(alarm) {
        names[ind] = "alarm";
        fields[ind++] = standardField->alarm();
    }
    if(descriptor) {
        names[ind] = "descriptor";
        fields[ind++] = fieldCreate->createScalar(pvString);
    }
    if(severity) {
        names[ind] = "severity";
        fields[ind++] = fieldCreate->createScalarArray(pvInt);
    }
    if(status) {
        names[ind] = "status";
        fields[ind++] = fieldCreate->createScalarArray(pvInt);
    }
    if(message) {
        names[ind] = "message";
        fields[ind++] = fieldCreate->createScalarArray(pvString);
    }
    if(secondsPastEpoch) {
        names[ind] = "secondsPastEpoch";
        fields[ind++] = fieldCreate->createScalarArray(pvLong);
    }
    if(nanoseconds) {
        names[ind] = "nanoseconds";
        fields[ind++] = fieldCreate->createScalarArray(pvInt);
    }
    if(userTag) {
        names[ind] = "userTag";
        fields[ind++] = fieldCreate->createScalarArray(pvInt);
    }
    for (size_t i = 0; i< extraCount; i++) {
        names[ind] = extraFieldNames[i];
        fields[ind++] = extraFields[i];
    }

    StructureConstPtr st = fieldCreate->createStructure(NTMultiChannel::URI,names,fields);
    reset();
    return st;
}

PVStructurePtr NTMultiChannelBuilder::NTMultiChannelBuilder::createPVStructure()
{
    return pvDataCreate->createPVStructure(createStructure());
}

NTMultiChannelPtr NTMultiChannelBuilder::create()
{
    return NTMultiChannelPtr(new NTMultiChannel(createPVStructure()));
}

NTMultiChannelBuilder::NTMultiChannelBuilder()
{
    reset();
}

void NTMultiChannelBuilder::reset()
{
    valueType.reset();
    extraFieldNames.clear();
    extraFields.clear();
    descriptor = false;
    alarm = false;
    timeStamp = false;
    severity = false;
    status = false;
    message = false;
    secondsPastEpoch = false;
    nanoseconds = false;
    userTag = false;
}


NTMultiChannelBuilder::shared_pointer NTMultiChannelBuilder::add(string const & name, FieldConstPtr const & field)
{
    extraFields.push_back(field); extraFieldNames.push_back(name);
    return shared_from_this();
}

}

const std::string NTMultiChannel::URI("epics:nt/NTMultiChannel:1.0");

NTMultiChannel::shared_pointer NTMultiChannel::wrap(PVStructurePtr const & structure)
{
    if(!isCompatible(structure)) return shared_pointer();
    return wrapUnsafe(structure);
}

NTMultiChannel::shared_pointer NTMultiChannel::wrapUnsafe(PVStructurePtr const & structure)
{
    return shared_pointer(new NTMultiChannel(structure));
}

bool NTMultiChannel::is_a(StructureConstPtr const &structure)
{
    return structure->getID() == URI;
}

bool NTMultiChannel::isCompatible(PVStructurePtr const &pvStructure)
{
    if(!pvStructure) return false;
    PVUnionArrayPtr pvValue = pvStructure->getSubField<PVUnionArray>("value");
    if(!pvValue) return false;
    PVFieldPtr pvField = pvStructure->getSubField("descriptor");
    if(pvField && !pvStructure->getSubField<PVString>("descriptor")) return false;
    pvField = pvStructure->getSubField("alarm");
    if(pvField && !ntField->isAlarm(pvField->getField())) return false;
    pvField = pvStructure->getSubField("timeStamp");
    if(pvField && !ntField->isTimeStamp(pvField->getField())) return false;
    pvField = pvStructure->getSubField("severity");
    if(pvField && !pvStructure->getSubField<PVIntArray>("severity")) return false;
    pvField = pvStructure->getSubField("status");
    if(pvField && !pvStructure->getSubField<PVIntArray>("status")) return false;
    pvField = pvStructure->getSubField("message");
    if(pvField && !pvStructure->getSubField<PVStringArray>("message")) return false;
    pvField = pvStructure->getSubField("secondsPastEpoch");
    if(pvField && !pvStructure->getSubField<PVLongArray>("secondsPastEpoch")) return false;
    pvField = pvStructure->getSubField("nanoseconds");
    if(pvField && !pvStructure->getSubField<PVIntArray>("nanoseconds")) return false;
    pvField = pvStructure->getSubField("userTag");
    if(pvField && !pvStructure->getSubField<PVIntArray>("userTag")) return false;
    return true;
}

NTMultiChannelBuilderPtr NTMultiChannel::createBuilder()
{
    return NTMultiChannelBuilderPtr(new detail::NTMultiChannelBuilder());
}


NTMultiChannel::NTMultiChannel(PVStructurePtr const & pvStructure)
: pvNTMultiChannel(pvStructure),
  pvTimeStamp(pvStructure->getSubField<PVStructure>("timeStamp")),
  pvAlarm(pvStructure->getSubField<PVStructure>("alarm")),
  pvValue(pvStructure->getSubField<PVUnionArray>("value")),
  pvChannelName(pvStructure->getSubField<PVStringArray>("channelName")),
  pvIsConnected(pvStructure->getSubField<PVBooleanArray>("isConnected")),
  pvSeverity(pvStructure->getSubField<PVIntArray>("severity")),
  pvStatus(pvStructure->getSubField<PVIntArray>("status")),
  pvMessage(pvStructure->getSubField<PVStringArray>("message")),
  pvSecondsPastEpoch(pvStructure->getSubField<PVLongArray>("secondsPastEpoch")),
  pvNanoseconds(pvStructure->getSubField<PVIntArray>("nanoseconds")),
  pvUserTag(pvStructure->getSubField<PVIntArray>("userTag")),
  pvDescriptor(pvStructure->getSubField<PVString>("descriptor"))
{
}


void  NTMultiChannel::attachTimeStamp(PVTimeStamp &pv) const
{
    if(!pvTimeStamp) return;
    pv.attach(pvTimeStamp);
}

void  NTMultiChannel::attachAlarm(PVAlarm &pv) const
{
    if(!pvAlarm) return;
    pv.attach(pvAlarm);
}

}}
