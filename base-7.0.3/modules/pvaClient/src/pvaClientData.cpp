/* pvaClientData.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 * @date 2019.04
 */

#include <typeinfo>
#include <sstream>

#include <pv/createRequest.h>
#include <pv/convert.h>

#define epicsExportSharedSymbols

#include <pv/pvaClient.h>

using std::tr1::static_pointer_cast;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace std;

namespace epics { namespace pvaClient {


typedef std::tr1::shared_ptr<PVArray> PVArrayPtr;
static ConvertPtr convert = getConvert();
static string noStructure("no pvStructure ");
static string noValue("no value field");
static string noScalar("value is not a scalar");
static string noArray("value is not an array");
static string noScalarArray("value is not a scalarArray");
static string noAlarm("no alarm");
static string noTimeStamp("no timeStamp");

PvaClientDataPtr PvaClientData::create(StructureConstPtr const & structure)
{
    if(PvaClient::getDebug()) cout << "PvaClientData::create\n";
    PvaClientDataPtr epv(new PvaClientData(structure));
    return epv;
}

PvaClientData::PvaClientData(StructureConstPtr const & structure)
: structure(structure)
{
}

void PvaClientData::checkValue()
{
    if(PvaClient::getDebug()) cout << "PvaClientData::checkValue\n";
    if(pvValue) return;
    throw std::runtime_error(messagePrefix + noValue);
}

void PvaClientData::setMessagePrefix(std::string const & value)
{
    messagePrefix = value + " ";
}

StructureConstPtr PvaClientData::getStructure()
{
    return structure;
}

PVStructurePtr PvaClientData::getPVStructure()
{
    if(pvStructure) return pvStructure;
    throw std::runtime_error(messagePrefix + noStructure);
}

BitSetPtr PvaClientData::getChangedBitSet()
{
    if(bitSet)return bitSet;
    throw std::runtime_error(messagePrefix + noStructure);
}

std::ostream & PvaClientData::showChanged(std::ostream & out)
{
    if(!bitSet) throw std::runtime_error(messagePrefix + noStructure);
    size_t nextSet = bitSet->nextSetBit(0);
    PVFieldPtr pvField;
    while(nextSet!=string::npos) {
        if(nextSet==0) {
             pvField = pvStructure;
        } else {
              pvField = pvStructure->getSubField(nextSet);
        }
        string name = pvField->getFullName();
        out << name << " = " << pvField << endl;
        nextSet = bitSet->nextSetBit(nextSet+1);
    }
    return out;
}

void PvaClientData::setData(
    PVStructurePtr const & pvStructureFrom,
    BitSetPtr const & bitSetFrom)
{
   if(PvaClient::getDebug()) cout << "PvaClientData::setData\n";
   pvStructure = pvStructureFrom;
   bitSet = bitSetFrom;
   pvValue = pvStructure->getSubField("value");
}

bool PvaClientData::hasValue()
{
    if(PvaClient::getDebug()) cout << "PvaClientData::hasValue\n";
    if(!pvValue) return false;
    return true;
}

bool PvaClientData::isValueScalar()
{
    if(PvaClient::getDebug()) cout << "PvaClientData::isValueScalar\n";
    if(!pvValue) return false;
    if(pvValue->getField()->getType()==scalar) return true;
    return false;
}

bool PvaClientData::isValueScalarArray()
{
    if(PvaClient::getDebug()) cout << "PvaClientData::isValueScalarArray\n";
    if(!pvValue) return false;
    if(pvValue->getField()->getType()==scalarArray) return true;
    return false;
}

PVFieldPtr  PvaClientData::getValue()
{
   if(PvaClient::getDebug()) cout << "PvaClientData::getValue\n";
   checkValue();
   return pvValue;
}

PVScalarPtr  PvaClientData::getScalarValue()
{
    if(PvaClient::getDebug()) cout << "PvaClientData::getScalarValue\n";
    checkValue();
    if(pvValue->getField()->getType()!=scalar) {
       throw std::runtime_error(messagePrefix + noScalar);
    }
    return pvStructure->getSubField<PVScalar>("value");
}

PVArrayPtr  PvaClientData::getArrayValue()
{
    if(PvaClient::getDebug()) cout << "PvaClientData::getArrayValue\n";
    checkValue();
    Type type = pvValue->getField()->getType();
    if(type!=scalarArray && type!=structureArray && type!=unionArray) {
        throw std::runtime_error(messagePrefix + noArray);
    }
    return pvStructure->getSubField<PVArray>("value");
}

PVScalarArrayPtr  PvaClientData::getScalarArrayValue()
{
    if(PvaClient::getDebug()) cout << "PvaClientData::getScalarArrayValue\n";
    checkValue();
    Type type = pvValue->getField()->getType();
    if(type!=scalarArray) {
        throw std::runtime_error(messagePrefix + noScalarArray);
    }
    return pvStructure->getSubField<PVScalarArray>("value");
}

double PvaClientData::getDouble()
{
    if(PvaClient::getDebug()) cout << "PvaClientData::getDouble\n";
    PVScalarPtr pvScalar;
    PVStructurePtr pvStructure = getPVStructure();
    PVFieldPtr pvValue  = pvStructure->getSubField("value");
    if(pvValue) {
        Type type = pvValue->getField()->getType();
        if(type==scalar) pvScalar = static_pointer_cast<PVScalar>(pvValue);
    }
    if(!pvScalar) {
        while(true) {
             const PVFieldPtrArray fieldPtrArray(pvStructure->getPVFields());
             if(fieldPtrArray.size()!=1) {
                  throw std::logic_error(
                      "PvaClientData::getDouble() pvRequest for multiple fields");
             }
             PVFieldPtr pvField(fieldPtrArray[0]);
             Type type = pvField->getField()->getType();
             if(type==scalar) {
                 pvScalar = static_pointer_cast<PVScalar>(pvField);
                 break;
             }
             if(pvField->getField()->getType()!=epics::pvData::structure) break;
             pvStructure = static_pointer_cast<PVStructure>(pvField);
        }
    }
    if(!pvScalar) {
        throw std::logic_error(
            "PvaClientData::getDouble() did not find a scalar field");
    }
    ScalarType scalarType = pvScalar->getScalar()->getScalarType();
    if(scalarType==pvDouble) {
        PVDoublePtr pvDouble = static_pointer_cast<PVDouble>(pvScalar);
        return pvDouble->get();
    }
    if(!ScalarTypeFunc::isNumeric(scalarType)) {
        throw std::logic_error(
            "PvaClientData::getDouble() did not find a numeric scalar field");
    }
    return convert->toDouble(pvScalar);
}

string PvaClientData::getString()
{
    if(PvaClient::getDebug()) cout << "PvaClientData::getString\n";
    PVScalarPtr pvScalar;
    PVStructurePtr pvStructure = getPVStructure();
    PVFieldPtr pvValue  = pvStructure->getSubField("value");
    if(pvValue) {
        Type type = pvValue->getField()->getType();
        if(type==scalar) pvScalar = static_pointer_cast<PVScalar>(pvValue);
    }
    if(!pvScalar) {
        while(true) {
             const PVFieldPtrArray fieldPtrArray(pvStructure->getPVFields());
             if(fieldPtrArray.size()!=1) {
                  throw std::logic_error(
                      "PvaClientData::getString() pvRequest for multiple fields");
             }
             PVFieldPtr pvField(fieldPtrArray[0]);
             Type type = pvField->getField()->getType();
             if(type==scalar) {
                 pvScalar = static_pointer_cast<PVScalar>(pvField);
                 break;
             }
             if(pvField->getField()->getType()!=epics::pvData::structure) break;
             pvStructure = static_pointer_cast<PVStructure>(pvField);
        }
    }
    if(!pvScalar) {
        throw std::logic_error(
            "PvaClientData::getString() did not find a scalar field");
    }
    return convert->toString(pvScalar);
}

shared_vector<const double> PvaClientData::getDoubleArray()
{
    if(PvaClient::getDebug()) cout << "PvaClientData::getDoubleArray\n";
    PVScalarArrayPtr pvScalarArray;
    PVStructurePtr pvStructure = getPVStructure();
    PVFieldPtr pvValue  = pvStructure->getSubField("value");
    if(pvValue) {
        Type type = pvValue->getField()->getType();
        if(type==scalarArray) {
            pvScalarArray = static_pointer_cast<PVScalarArray>(pvValue);
        }
    }
    if(!pvScalarArray) {
        while(true) {
             const PVFieldPtrArray fieldPtrArray(pvStructure->getPVFields());
             if(fieldPtrArray.size()!=1) {
                  throw std::logic_error(
                      "PvaClientData::getDoubleArray() pvRequest for multiple fields");
             }
             PVFieldPtr pvField(fieldPtrArray[0]);
             Type type = pvField->getField()->getType();
             if(type==scalarArray) {
                 pvScalarArray = static_pointer_cast<PVScalarArray>(pvField);
                 break;
             }
             if(pvField->getField()->getType()!=epics::pvData::structure) break;
             pvStructure = static_pointer_cast<PVStructure>(pvField);
        }
    }
    if(!pvScalarArray) {
        throw std::logic_error(
            "PvaClientData::getDoubleArray() did not find a scalarArray field");
    }
    ScalarType scalarType = pvScalarArray->getScalarArray()->getElementType();
    if(!ScalarTypeFunc::isNumeric(scalarType)) {
        throw std::logic_error(
            "PvaClientData::getDoubleArray() did not find a numeric scalarArray field");
    }
    shared_vector<const double> retValue;
    pvScalarArray->getAs<const double>(retValue);
    return retValue;
}

shared_vector<const string> PvaClientData::getStringArray()
{
    if(PvaClient::getDebug()) cout << "PvaClientData::getStringArray\n";
    PVScalarArrayPtr pvScalarArray;
    PVStructurePtr pvStructure = getPVStructure();
    PVFieldPtr pvValue  = pvStructure->getSubField("value");
    if(pvValue) {
       Type type = pvValue->getField()->getType();
        if(type==scalarArray) {
             pvScalarArray = static_pointer_cast<PVScalarArray>(pvValue);
        }
    }
    if(!pvScalarArray) {
        while(true) {
             const PVFieldPtrArray fieldPtrArray(pvStructure->getPVFields());
             if(fieldPtrArray.size()!=1) {
                  throw std::logic_error(
                      "PvaClientData::getStringArray() pvRequest for multiple fields");
             }
             PVFieldPtr pvField(fieldPtrArray[0]);
             Type type = pvField->getField()->getType();
             if(type==scalarArray) {
                 pvScalarArray = static_pointer_cast<PVScalarArray>(pvField);
                 break;
             }
             if(pvField->getField()->getType()!=epics::pvData::structure) break;
             pvStructure = static_pointer_cast<PVStructure>(pvField);
        }
    }
    if(!pvScalarArray) {
        throw std::logic_error(
            "PvaClientData::getStringArray() did not find a scalarArray field");
    }
    shared_vector<const string> retValue;
    pvScalarArray->getAs<const string>(retValue);
    return retValue;
}

Alarm PvaClientData::getAlarm()
{
   if(PvaClient::getDebug()) cout << "PvaClientData::getAlarm\n";
   if(!pvStructure) throw new std::runtime_error(messagePrefix + noStructure);
   PVStructurePtr pvs = pvStructure->getSubField<PVStructure>("alarm");
   if(!pvs) throw std::runtime_error(messagePrefix + noAlarm);
   pvAlarm.attach(pvs);
   if(pvAlarm.isAttached()) {
       Alarm alarm;
       pvAlarm.get(alarm);
       pvAlarm.detach();
       return alarm;
   }
   throw std::runtime_error(messagePrefix + noAlarm);
}

TimeStamp PvaClientData::getTimeStamp()
{
   if(PvaClient::getDebug()) cout << "PvaClientData::getTimeStamp\n";
   if(!pvStructure) throw new std::runtime_error(messagePrefix + noStructure);
   PVStructurePtr pvs = pvStructure->getSubField<PVStructure>("timeStamp");
   if(!pvs) throw std::runtime_error(messagePrefix + noTimeStamp);
   pvTimeStamp.attach(pvs);
   if(pvTimeStamp.isAttached()) {
       TimeStamp timeStamp;
       pvTimeStamp.get(timeStamp);
       pvTimeStamp.detach();
       return timeStamp;
   }
   throw std::runtime_error(messagePrefix + noTimeStamp);
}

}}
