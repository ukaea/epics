/**
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution.
 */
/**
 * @author mrk
 */

#include <string>
#include <cstring>
#include <stdexcept>

#include <dbAccess.h>
#include <dbChannel.h>
#include <dbEvent.h>
#include <dbNotify.h>
#include <special.h>
#include <link.h>
#include <alarm.h>

#include <pv/pvIntrospect.h>
#include <pv/pvData.h>
#include <pv/convert.h>
#include <pv/pvEnumerated.h>
#include <pv/pvAccess.h>
#include <pv/standardField.h>
#include <pv/standardPVField.h>
#include <pv/control.h>
#include <pv/display.h>
#include <pv/timeStamp.h>
#include <pv/pvAlarm.h>
#include <pv/pvTimeStamp.h>
#include <pv/pvControl.h>
#include <pv/pvDisplay.h>
#include <pv/caStatus.h>

#include "dbUtil.h"

using namespace epics::pvData;
using std::tr1::static_pointer_cast;
using std::string;
using epics::pvAccess::ca::dbrStatus2alarmMessage;
using epics::pvAccess::ca::dbrStatus2alarmStatus;

namespace epics { namespace pvaSrv { 

// Filter out unrequested fields from a source structure according to a
// structure conforming to the format of the "field" field of a pvRequest,
// preserving type ids of unchanged structures.
static StructureConstPtr refineStructure(StructureConstPtr const & source,
           StructureConstPtr const & requestedFields)
{
    if (requestedFields.get() == NULL || requestedFields->getNumberFields() == 0)
        return source;

    FieldBuilderPtr builder = getFieldCreate()->createFieldBuilder();
    bool addId = true;

    FieldConstPtrArray fields = source->getFields();
    StringArray names = source->getFieldNames();
    size_t i = 0;
    for (FieldConstPtrArray::const_iterator it = fields.begin(); it != fields.end(); ++it)
    {
        FieldConstPtr field = *it;
        const std::string & name = names[i++];
        FieldConstPtr reqField = requestedFields->getField(name);
        if (reqField.get())
        {
            if (field->getType() != structure || (reqField->getType() != structure))
                builder->add(name,field);
            else
            {
                StructureConstPtr substruct =
                    std::tr1::dynamic_pointer_cast<const Structure>(field);

                StructureConstPtr reqSubstruct =
                    std::tr1::dynamic_pointer_cast<const Structure>(reqField);

                StructureConstPtr nested = refineStructure(substruct, reqSubstruct);
                builder->add(name,nested);
                if (nested->getID() != substruct->getID())
                    addId = false;
            }
        }
        else
            addId =  false;
    }
    if (addId)
        builder->setId(source->getID());
    return  builder->createStructure();
}

// Return DBF type of final field (after any filters applied)
static short dbChannelFinalDBFType(dbChannel *dbChan)
{
    // dbChannelFinalFieldType returns dbChannel final_type which is equal
    // to addr.dbr_field_type in the case of no filters, so if field type
    // is DBF_DEVICE or DBF_MENU it will return DBR_ENUM. Return
    // DBF type, at least if no filters.
    if (ellCount(&dbChan->filters) == 0)
        return dbChannelFieldType(dbChan);
    else
        // This is correct when field_type is not DBF_DEVICE or DBF_MENU
        return dbChannelFinalFieldType(dbChan);
}

DbUtilPtr DbUtil::getDbUtil()
{
    static DbUtilPtr util;
    static Mutex mutex;
    Lock xx(mutex);

    if(util.get()==NULL) {
        util = DbUtilPtr(new DbUtil());
    }
    return util;
}

DbUtil::DbUtil()
    :
      processBit(    0x0001),
      blockBit(      0x0002),
      timeStampBit(  0x0004),
      alarmBit(      0x0008),
      displayBit(    0x0010),
      controlBit(    0x0020),
      valueAlarmBit( 0x0040),
      // DB data characteristics
      getValueBit(   0x0080),
      scalarValueBit(0x0100),
      arrayValueBit( 0x0200),
      enumValueBit(  0x0400),
      enumIndexBit(  0x0800),
      noAccessBit(   0x1000),
      noModBit(      0x2000),
      dbPutBit(      0x4000),
      isLinkBit(     0x8000),
      recordString("record"),
      processString("record._options.process"),
      blockString("record._options.block"),
      queueSizeString("record._options.queueSize"),
      fieldString("field"),
      valueString("value"),
      valueIndexString("value.index"),
      valueChoicesString("value.choices"),
      timeStampString("timeStamp"),
      alarmString("alarm"),
      displayString("display"),
      controlString("control"),
      valueAlarmString("valueAlarm"),
      lowAlarmLimitString("lowAlarmLimit"),
      lowWarningLimitString("lowWarningLimit"),
      highWarningLimitString("highWarningLimit"),
      highAlarmLimitString("highAlarmLimit"),
      allString("value,timeStamp,alarm,display,control,valueAlarm"),
      indexString("index"),
      choicesString("choices")
{}

int DbUtil::getProperties(
        Requester::shared_pointer const &requester,
        PVStructure::shared_pointer const &pvr,
        dbChannel *dbChan,
        bool processDefault)
{
    PVStructure *pvRequest = pvr.get();
    int propertyMask = 0;
    PVFieldPtr pvField = pvRequest->getSubField(processString);
    if(pvField.get()!=NULL) {
        PVStringPtr pvString = pvRequest->getSubField<PVString>(processString);
        if(pvString.get()!=NULL) {
            string value = pvString->get();
            if(value.compare("true")==0) propertyMask |= processBit;
        }
    } else {
        if(processDefault) propertyMask |= processBit;
    }
    pvField = pvRequest->getSubField(blockString);
    if (pvField.get()) {
        PVStringPtr pvString = pvRequest->getSubField<PVString>(blockString);
        if (pvString.get()) {
            string value = pvString->get();
            if (value.compare("true") == 0) propertyMask |= blockBit;
        }
    } else {
        if (propertyMask & processBit) propertyMask |= blockBit;
    }
    bool getValue = false;
    string fieldList;
    PVStructurePtr fieldPV = pvRequest->getSubField<PVStructure>(fieldString);
    if(fieldPV.get()==NULL) {
        getValue = true;
        fieldList = allString;
    } else {
        if(fieldPV.get()!=NULL) pvRequest = fieldPV.get();
        if(pvRequest->getStructure()->getNumberFields()==0) {
            getValue = true;
            fieldList = allString;
        } else {
            pvField = pvRequest->getSubField(valueString);
            if(pvField.get()!=NULL) {
                if(fieldList.size()>0) fieldList += ',';
                fieldList += valueString;
                getValue = true;
            }
            pvField = pvRequest->getSubField(alarmString);
            if(pvField.get()!=NULL) {
                if(fieldList.size()>0) fieldList += ',';
                fieldList += alarmString;
            }
            pvField = pvRequest->getSubField(timeStampString);
            if(pvField.get()!=NULL) {
                if(fieldList.size()>0) fieldList += ',';
                fieldList += timeStampString;
            }
            pvField = pvRequest->getSubField(displayString);
            if(pvField.get()!=NULL) {
                if(fieldList.size()>0) fieldList += ',';
                fieldList += displayString;
            }
            pvField = pvRequest->getSubField(controlString);
            if(pvField.get()!=NULL) {
                if(fieldList.size()>0) fieldList += ',';
                fieldList += controlString;
            }
            pvField = pvRequest->getSubField(valueAlarmString);
            if(pvField.get()!=NULL) {
                if(fieldList.size()>0) fieldList += ',';
                fieldList += valueAlarmString;
            }
        }
    }
    if (!fieldList.size()) {
        requester->message("no valid field name specified", errorMessage);
        propertyMask = noAccessBit; return propertyMask;
    }
    if(getValue) {
        propertyMask |= getValueBit;
    }
    Type type = (dbChannelSpecial(dbChan)==SPC_DBADDR) ? scalarArray : scalar;
    ScalarType scalarType(pvBoolean);
    // Note that pvBoolean is not a supported type
    switch (dbChannelFinalDBFType(dbChan)) {
    case DBF_STRING:
        scalarType = pvString; break;
    case DBF_CHAR:
        scalarType = pvByte; break;
    case DBF_UCHAR:
        scalarType = pvUByte; break;
    case DBF_SHORT:
        scalarType = pvShort; break;
    case DBF_USHORT:
        scalarType = pvUShort; break;
    case DBF_LONG:
        scalarType = pvInt; break;
    case DBF_ULONG:
        scalarType = pvUInt; break;
    case DBF_FLOAT:
        scalarType = pvFloat; break;
    case DBF_DOUBLE:
        scalarType = pvDouble; break;
    case DBF_ENUM:
        propertyMask |= enumValueBit; break;
    case DBF_MENU:
        propertyMask |= (enumValueBit|dbPutBit); break;
    case DBF_DEVICE:
        propertyMask |= enumValueBit; break;
    case DBF_INLINK:
    case DBF_OUTLINK:
    case DBF_FWDLINK:
        scalarType = pvString;
        propertyMask |= (isLinkBit|dbPutBit); break;
    case DBF_NOACCESS:
        requester->message("access is not allowed",errorMessage);
        propertyMask = noAccessBit; return propertyMask;
    default:
        requester->message("logic error unknown DBF type",errorMessage);
        propertyMask = noAccessBit; return propertyMask;
    }
    if(type==scalar&&scalarType!=pvBoolean) {
        propertyMask |= scalarValueBit;
    }
    if(type==scalarArray&&scalarType!=pvBoolean) {
        propertyMask |= arrayValueBit;
    }
    if (dbChannelSpecial(dbChan)) {
        switch (dbChannelSpecial(dbChan)) {
        case SPC_NOMOD:
            propertyMask |= noModBit; break;
        case SPC_DBADDR: // already used
            break;
        case SPC_SCAN:
        case SPC_ALARMACK:
        case SPC_AS:
        case SPC_ATTRIBUTE:
        case SPC_MOD:
        case SPC_RESET:
        case SPC_LINCONV:
        case SPC_CALC:
            propertyMask |= dbPutBit; break;
        default:
            requester->message("logic error unknown special type",errorMessage);
            propertyMask = noAccessBit; return propertyMask;
        }
    }

    if(fieldList.length()!=0) {
        if(fieldList.find(timeStampString)!=string::npos) {
            propertyMask |= timeStampBit;
        }
        if(fieldList.find(alarmString)!=string::npos) {
            propertyMask |= alarmBit;
        }
        switch(dbChannelFinalDBFType(dbChan))
        {
        case DBF_CHAR:
        case DBF_UCHAR:
        case DBF_SHORT:
        case DBF_USHORT:
        case DBF_LONG:
        case DBF_ULONG:
        case DBF_FLOAT:
        case DBF_DOUBLE:
            if(fieldList.find(displayString)!=string::npos) {
                propertyMask |= displayBit;
            }
            if(type==scalar && fieldList.find(controlString)!=string::npos) {
                propertyMask |= controlBit;
            }
            if(type==scalar && fieldList.find(valueAlarmString)!=string::npos) {
                propertyMask |= valueAlarmBit;
            }
            break;
        default:
            break;
        }
    }
    if(propertyMask&enumValueBit) {
        pvField = pvRequest->getSubField(valueIndexString);
        if(pvField.get()!=NULL) propertyMask |= enumIndexBit;
    }
    return propertyMask;
}

PVStructurePtr DbUtil::createPVStructure(
        Requester::shared_pointer const &requester, int propertyMask,
        dbChannel *dbChan, PVStructure::shared_pointer const &pvRequest)
{
    StandardPVFieldPtr standardPVField = getStandardPVField();
    StandardFieldPtr standardField = getStandardField();
    PVDataCreatePtr pvDataCreate = getPVDataCreate();
    FieldCreatePtr fieldCreate = getFieldCreate();

    string properties;
    if((propertyMask&timeStampBit)!=0) properties+= timeStampString;
    if((propertyMask&alarmBit)!=0) {
        if(!properties.empty()) properties += ",";
        properties += alarmString;
    }
    if((propertyMask&displayBit)!=0) {
        if(!properties.empty()) properties += ",";
        properties += displayString;
    }
    if((propertyMask&controlBit)!=0) {
        if(!properties.empty()) properties += ",";
        properties += controlString;
    }
    if((propertyMask&valueAlarmBit)!=0) {
        if(!properties.empty()) properties += ",";
        properties += valueAlarmString;
    }

    StructureConstPtr unrefinedStructure;

    if((propertyMask & enumValueBit)!=0) {
        unrefinedStructure = standardField->enumerated(properties);
    }
    else {
        ScalarType scalarType = propertyMask&isLinkBit ?
                    pvString : getScalarType(requester,dbChan);
        if (scalarType == pvBoolean)
            throw std::logic_error("Should never get here");

        if((propertyMask & scalarValueBit)!=0)
            unrefinedStructure = standardField->scalar(scalarType,properties);
        else if((propertyMask & arrayValueBit)!=0)
            unrefinedStructure = standardField->scalarArray(scalarType,properties);
        else
            return nullPVStructure;
    }

    PVStructurePtr fieldPVStructure = pvRequest->getSubField<PVStructure>("field"); 
    StructureConstPtr finalStructure = fieldPVStructure.get() ?
        refineStructure(unrefinedStructure, fieldPVStructure->getStructure()) :
        unrefinedStructure;

    PVStructurePtr pvStructure = pvDataCreate->createPVStructure(finalStructure);

    if((propertyMask&enumValueBit)!=0) {
        struct dbr_enumStrs enumStrs;
        struct rset *prset = dbGetRset(&dbChan->addr);

        PVStringArrayPtr pvChoices = pvStructure->getSubField<PVStringArray>(valueChoicesString);
        if(pvChoices.get()) {
            PVStringArray::svector choices;

            if (dbChannelFinalDBFType(dbChan) == DBF_ENUM &&
                    prset && prset->get_enum_strs) {
                get_enum_strs get_strs;
                get_strs = (get_enum_strs)(prset->get_enum_strs);
                get_strs(&dbChan->addr, &enumStrs);
                size_t length = enumStrs.no_str;
                choices.reserve(length);
                for(size_t i=0; i<length; i++)
                    choices.push_back(enumStrs.strs[i]);
            }
            else if(dbChannelFinalDBFType(dbChan) == DBF_DEVICE) {
                dbFldDes *pdbFldDes = dbChannelFldDes(dbChan);
                dbDeviceMenu *pdbDeviceMenu
                        = static_cast<dbDeviceMenu *>(pdbFldDes->ftPvt);
                if(pdbDeviceMenu==NULL) {
                    requester->message(
                                "record type has no device support", errorMessage);
                    return nullPVStructure;
                }

                size_t length = pdbDeviceMenu->nChoice;
                char **papChoice = pdbDeviceMenu->papChoice;
                choices.reserve(length);
                for(size_t i=0; i<length; i++)
                    choices.push_back(papChoice[i]);
            }
            else if(dbChannelFinalDBFType(dbChan) == DBF_MENU) {
                dbFldDes *pdbFldDes = dbChannelFldDes(dbChan);
                dbMenu *pdbMenu = static_cast<dbMenu *>(pdbFldDes->ftPvt);
                size_t length = pdbMenu->nChoice;
                char **papChoice = pdbMenu->papChoiceValue;
                choices.reserve(length);
                for(size_t i=0; i<length; i++)
                    choices.push_back(papChoice[i]);
            }
            else {
                requester->message("bad enum field in V3 record",errorMessage);
                return nullPVStructure;
            }
            pvChoices->replace(freeze(choices));
        }
    }
    return pvStructure;
}

void  DbUtil::getPropertyData(
        Requester::shared_pointer const &requester,
        int propertyMask,
        dbChannel *dbChan,
        PVStructurePtr const &pvStructure)
{
    BitSet::shared_pointer bitSet;
    getPropertyData(requester, propertyMask,
        dbChan,pvStructure,bitSet);
}

void  DbUtil::getPropertyData(
        Requester::shared_pointer const &requester,
        int propertyMask,
        dbChannel *dbChan,
        PVStructurePtr const &pvStructure,
        BitSet::shared_pointer const &bitSet)
{
        getDisplayData(requester, propertyMask, dbChan,
            pvStructure, bitSet);

        getControlData(requester, propertyMask, dbChan,
            pvStructure, bitSet);

        getValueAlarmData(requester, propertyMask, dbChan,
            pvStructure, bitSet);
}

void  DbUtil::getDisplayData(
        Requester::shared_pointer const &requester,
        int propertyMask,
        dbChannel *dbChan,
        PVStructurePtr const &pvStructure,
        BitSet::shared_pointer const &bitSet)
{

    if(propertyMask&displayBit) {
        PVStructurePtr displayField = pvStructure->getSubFieldT<PVStructure>(displayString);
        char units[DB_UNITS_SIZE];
        units[0] = 0;
        long precision = 0;
        struct rset *prset = dbGetRset(&dbChan->addr);
        if(prset && prset->get_units) {
            PVStringPtr unitsField = displayField->getSubField<PVString>("units");
            if (unitsField.get()) {
                get_units gunits;
                gunits = (get_units)(prset->get_units);
                gunits(&dbChan->addr,units);
                string unitsString = string(units);
                if (unitsString != unitsField->get()) {
                    unitsField->put(unitsString);
                    if (bitSet.get()) 
                        bitSet->set(unitsField->getFieldOffset());
                }
            }
        }
        PVStringPtr formatField = displayField->getSubField<PVString>("format");
        if (formatField.get()) {
            string format;
            ScalarType scalarType = getScalarType(requester, dbChan);
            if (scalarType == pvFloat || scalarType == pvDouble) {
                if(prset && prset->get_precision) {
                    get_precision gprec = (get_precision)(prset->get_precision);
                    gprec(&dbChan->addr,&precision);
                    if(precision>0) {
                        char fmt[16];
                        sprintf(fmt,"%%.%ldf",precision);
                        format = string(fmt);
                    } else {
                        const static string defaultFormat("%f");
                        format = defaultFormat;
                    }
                } else {
                    format="%f";
                }
            } else if (scalarType == pvString)
                format="%s";
            else if (scalarType == pvUByte || scalarType == pvUShort ||
                     scalarType == pvUInt  || scalarType == pvULong) {
                format="%u";
            } else {
                format="%d";
            }
            if (format != formatField->get()) {
                formatField->put(format);
                if (bitSet.get()) 
                   bitSet->set(formatField->getFieldOffset());
            }
        }
        struct dbr_grDouble graphics;
        if(prset && prset->get_graphic_double) {
            get_graphic_double gg =
                    (get_graphic_double)(prset->get_graphic_double);
            gg(&dbChan->addr,&graphics);

            PVDoublePtr limitLowField = displayField->getSubField<PVDouble>("limitLow");
            if (limitLowField.get() &&
                    limitLowField->get() != graphics.lower_disp_limit) {
                limitLowField->put(graphics.lower_disp_limit);
                if (bitSet.get()) 
                    bitSet->set(limitLowField->getFieldOffset());
            }

            PVDoublePtr limitHighField = displayField->getSubField<PVDouble>("limitHigh");
            if (limitHighField.get() &&
                    limitHighField->get() != graphics.upper_disp_limit) {
                limitHighField->put(graphics.upper_disp_limit);
                if (bitSet.get()) 
                    bitSet->set(limitHighField->getFieldOffset());
            }
        }
    }
}

void  DbUtil::getControlData(
        Requester::shared_pointer const &requester,
        int propertyMask,
        dbChannel *dbChan,
        PVStructurePtr const &pvStructure,
        BitSet::shared_pointer const &bitSet)
{
    if(propertyMask&controlBit) {
        PVStructurePtr controlField = pvStructure->getSubFieldT<PVStructure>(controlString);
        struct rset *prset = dbGetRset(&dbChan->addr);
        struct dbr_ctrlDouble graphics;
        memset(&graphics,0,sizeof(graphics));
        if(prset && prset->get_control_double) {
            get_control_double cc =
                    (get_control_double)(prset->get_control_double);
            cc(&dbChan->addr, &graphics);

            PVDoublePtr limitLowField = controlField->getSubField<PVDouble>("limitLow");
            if (limitLowField.get() &&
                    limitLowField->get() != graphics.lower_ctrl_limit) {
                limitLowField->put(graphics.lower_ctrl_limit);
                if (bitSet.get()) 
                    bitSet->set(limitLowField->getFieldOffset());
            }

            PVDoublePtr limitHighField = controlField->getSubField<PVDouble>("limitHigh");
            if (limitHighField.get() &&
                    limitHighField->get() != graphics.upper_ctrl_limit) {
                limitHighField->put(graphics.upper_ctrl_limit);
                if (bitSet.get()) 
                    bitSet->set(limitHighField->getFieldOffset());
            }
        }
    }
}

void  DbUtil::getValueAlarmData(
        Requester::shared_pointer const &requester,
        int propertyMask,
        dbChannel *dbChan,
        PVStructurePtr const &pvStructure,
        BitSet::shared_pointer const &bitSet)
{
    if(propertyMask&valueAlarmBit) {
        struct rset *prset = dbGetRset(&dbChan->addr);
        struct dbr_alDouble ald;
        memset(&ald,0,sizeof(ald));
        if(prset && prset->get_alarm_double) {
            get_alarm_double cc =
                    (get_alarm_double)(prset->get_alarm_double);
            cc(&dbChan->addr,&ald);
        }
        PVStructurePtr pvAlarmLimits =
                pvStructure->getSubFieldT<PVStructure>(valueAlarmString);
        PVBooleanPtr pvActive = pvAlarmLimits->getSubField<PVBoolean>("active");
        if(pvActive.get()!=NULL) pvActive->put(false);
        PVScalarPtr pvScalar = pvAlarmLimits->getSubField<PVScalar>(lowAlarmLimitString);
        if(pvScalar.get()!=NULL) {
            double lowerAlarmLimit = getConvert()->toDouble(pvScalar);
            if (lowerAlarmLimit != ald.lower_alarm_limit)
            {
                getConvert()->fromDouble(pvScalar,ald.lower_alarm_limit);
                if (bitSet.get())
                    bitSet->set(pvScalar->getFieldOffset());
            }
        }
        pvScalar = pvAlarmLimits->getSubField<PVScalar>(lowWarningLimitString);
        if(pvScalar.get()!=NULL) {
            double lowerWarningLimit = getConvert()->toDouble(pvScalar);
            if (lowerWarningLimit != ald.lower_warning_limit)
            {
                getConvert()->fromDouble(pvScalar,ald.lower_warning_limit);
                if (bitSet.get())
                    bitSet->set(pvScalar->getFieldOffset());
            }
        }
        pvScalar = pvAlarmLimits->getSubField<PVScalar>(highWarningLimitString);
        if(pvScalar.get()!=NULL) {
            double higherAlarmWarning = getConvert()->toDouble(pvScalar);
            if (higherAlarmWarning != ald.upper_warning_limit)
            {
                getConvert()->fromDouble(pvScalar,ald.upper_warning_limit);
                if (bitSet.get())
                    bitSet->set(pvScalar->getFieldOffset());
            }
        }
        pvScalar = pvAlarmLimits->getSubField<PVScalar>(highAlarmLimitString);
        if(pvScalar.get()!=NULL) {
            double higherAlarmLimit = getConvert()->toDouble(pvScalar);
            if (higherAlarmLimit != ald.upper_alarm_limit)
            {
                getConvert()->fromDouble(pvScalar,ald.upper_alarm_limit);
                if (bitSet.get())
                    bitSet->set(pvScalar->getFieldOffset());
            }
        }
    }
}

Status  DbUtil::get(
        Requester::shared_pointer const &requester,
        int propertyMask,
        dbChannel *dbChan,
        PVStructurePtr const &pvStructure,
        BitSet::shared_pointer const &bitSet,
        CaData *caData)
{
    if((propertyMask&getValueBit)!=0) {
        PVFieldPtrArray pvFields = pvStructure->getPVFields();
        PVFieldPtr pvField = pvFields[0];
        if((propertyMask&scalarValueBit)!=0) {
            PVScalarPtr pvScalar = static_pointer_cast<PVScalar>(pvField);
            ScalarType scalarType = pvScalar->getScalar()->getScalarType();
            bool wasChanged = false;
            switch(scalarType) {
            case pvByte: {
                int8 val = 0;
                if(caData) {
                    val = caData->byteValue;
                } else {
                    val = *static_cast<int8 *>(dbChannelField(dbChan));
                }
                PVBytePtr pv = static_pointer_cast<PVByte>(pvField);
                if(pv->get()!=val) {
                    pv->put(val);
                    wasChanged = true;
                }
                break;
            }
            case pvUByte: {
                uint8 val = 0;
                if(caData) {
                    val = caData->ubyteValue;
                } else {
                    val = *static_cast<uint8 *>(dbChannelField(dbChan));
                }
                PVUBytePtr pv = static_pointer_cast<PVUByte>(pvField);
                if(pv->get()!=val) {
                    pv->put(val);
                    wasChanged = true;
                }
                break;
            }
            case pvShort: {
                int16 val = 0;
                if(caData) {
                    val = caData->shortValue;
                } else {
                    val = *static_cast<int16 *>(dbChannelField(dbChan));
                }
                PVShortPtr pv = static_pointer_cast<PVShort>(pvField);
                if(pv->get()!=val) {
                    pv->put(val);
                    wasChanged = true;
                }
                break;
            }
            case pvUShort: {
                uint16 val = 0;
                if(caData) {
                    val = caData->ushortValue;
                } else {
                    val = *static_cast<uint16 *>(dbChannelField(dbChan));
                }
                PVUShortPtr pv = static_pointer_cast<PVUShort>(pvField);
                if(pv->get()!=val) {
                    pv->put(val);
                    wasChanged = true;
                }
                break;
            }
            case pvInt: {
                int32 val = 0;
                if(caData) {
                    val = caData->intValue;
                } else {
                    val = *static_cast<int32 *>(dbChannelField(dbChan));
                }
                PVIntPtr pv = static_pointer_cast<PVInt>(pvField);
                if(pv->get()!=val) {
                    pv->put(val);
                    wasChanged = true;
                }
                break;
            }
            case pvUInt: {
                uint32 val = 0;
                if(caData) {
                    val = caData->uintValue;
                } else {
                    val = *static_cast<uint32 *>(dbChannelField(dbChan));
                }
                PVUIntPtr pv = static_pointer_cast<PVUInt>(pvField);
                if(pv->get()!=val) {
                    pv->put(val);
                    wasChanged = true;
                }
                break;
            }
            case pvFloat: {
                float val = 0;
                if(caData) {
                    val = caData->floatValue;
                } else {
                    val = *static_cast<float *>(dbChannelField(dbChan));
                }
                PVFloatPtr pv = static_pointer_cast<PVFloat>(pvField);
                if(pv->get()!=val) {
                    pv->put(val);
                    wasChanged = true;
                }
                break;
            }
            case pvDouble: {
                double val = 0;
                if(caData) {
                    val = caData->doubleValue;
                } else {
                    val = *static_cast<double *>(dbChannelField(dbChan));
                }
                PVDoublePtr pv = static_pointer_cast<PVDouble>(pvField);
                if(pv->get()!=val) {
                    pv->put(val);
                    wasChanged = true;
                }
                break;
            }
            case pvString: {
                char * val = 0;
                if(propertyMask&isLinkBit) {
                    char buffer[200];
                    for(int i=0; i<200; i++) buffer[i]  = 0;
                    long result = dbGetField(&dbChan->addr,DBR_STRING,
                                             buffer,0,0,0);
                    if(result!=0) {
                        requester->message("dbGetField error",errorMessage);
                    }
                    val = buffer;
                } else {
                    val = static_cast<char *>(dbChannelField(dbChan));
                }
                string sval(val);
                PVStringPtr pvString = static_pointer_cast<PVString>(pvField);
                if(pvString->get().empty()) {
                    pvString->put(sval);
                    wasChanged = true;
                } else {
                    if((pvString->get().compare(sval))!=0) {
                        pvString->put(sval);
                        wasChanged = true;
                    }
                }
                break;
            }
            default:
                throw std::logic_error("Should never get here");
            }
            if(wasChanged) bitSet->set(pvField->getFieldOffset());
        } else if((propertyMask&arrayValueBit)!=0) {
            PVScalarArrayPtr pvArray = static_pointer_cast<PVScalarArray>(pvField);
            ScalarType scalarType = pvArray->getScalarArray()->getElementType();
            long rec_length = 0;
            long rec_offset = 0;
            struct rset *prset = dbGetRset(&dbChan->addr);
            get_array_info get_info;
            get_info = (get_array_info)(prset->get_array_info);
            get_info(&dbChan->addr, &rec_length, &rec_offset);
            if(rec_offset!=0) {
                throw std::logic_error("Can't handle offset != 0");
            }
            size_t length = rec_length;

            switch(scalarType) {
            case pvByte: {
                shared_vector<int8> xxx(length);
                int8 *pv3 = static_cast<int8 *>(dbChannelField(dbChan));
                for(size_t i=0; i<length; i++) xxx[i] = pv3[i];
                shared_vector<const int8> data(freeze(xxx));
                PVByteArrayPtr pva = static_pointer_cast<PVByteArray>(pvArray);
                pva->replace(data);
                break;
            }
            case pvUByte: {
                shared_vector<uint8> xxx(length);
                uint8 *pv3 = static_cast<uint8 *>(dbChannelField(dbChan));
                for(size_t i=0; i<length; i++) xxx[i] = pv3[i];
                shared_vector<const uint8> data(freeze(xxx));
                PVUByteArrayPtr pva = static_pointer_cast<PVUByteArray>(pvArray);
                pva->replace(data);
                break;
            }
            case pvShort: {
                shared_vector<int16> xxx(length);
                int16 *pv3 = static_cast<int16 *>(dbChannelField(dbChan));
                for(size_t i=0; i<length; i++) xxx[i] = pv3[i];
                shared_vector<const int16> data(freeze(xxx));
                PVShortArrayPtr pva = static_pointer_cast<PVShortArray>(pvArray);
                pva->replace(data);
                break;
            }
            case pvUShort: {
                shared_vector<uint16> xxx(length);
                uint16 *pv3 = static_cast<uint16 *>(dbChannelField(dbChan));
                for(size_t i=0; i<length; i++) xxx[i] = pv3[i];
                shared_vector<const uint16> data(freeze(xxx));
                PVUShortArrayPtr pva = static_pointer_cast<PVUShortArray>(pvArray);
                pva->replace(data);
                break;
            }
            case pvInt: {
                shared_vector<int32> xxx(length);
                int32 *pv3 = static_cast<int32 *>(dbChannelField(dbChan));
                for(size_t i=0; i<length; i++) xxx[i] = pv3[i];
                shared_vector<const int32> data(freeze(xxx));
                PVIntArrayPtr pva = static_pointer_cast<PVIntArray>(pvArray);
                pva->replace(data);
                break;
            }
            case pvUInt: {
                shared_vector<uint32> xxx(length);
                uint32 *pv3 = static_cast<uint32 *>(dbChannelField(dbChan));
                for(size_t i=0; i<length; i++) xxx[i] = pv3[i];
                shared_vector<const uint32> data(freeze(xxx));
                PVUIntArrayPtr pva = static_pointer_cast<PVUIntArray>(pvArray);
                pva->replace(data);
                break;
            }
            case pvFloat: {
                shared_vector<float> xxx(length);
                float *pv3 = static_cast<float *>(dbChannelField(dbChan));
                for(size_t i=0; i<length; i++) xxx[i] = pv3[i];
                shared_vector<const float> data(freeze(xxx));
                PVFloatArrayPtr pva = static_pointer_cast<PVFloatArray>(pvArray);
                pva->replace(data);
                break;
            }
            case pvDouble: {
                shared_vector<double> xxx(length);
                double *pv3 = static_cast<double *>(dbChannelField(dbChan));
                for(size_t i=0; i<length; i++) xxx[i] = pv3[i];
                shared_vector<const double> data(freeze(xxx));
                PVDoubleArrayPtr pva = static_pointer_cast<PVDoubleArray>(pvArray);
                pva->replace(data);
                break;
            }
            case pvString: {
                shared_vector<string> xxx(length);
                char *pv3 = static_cast<char *>(dbChannelField(dbChan));
                for(size_t i=0; i<length; i++) {
                    xxx[i] = pv3;
                    pv3 += dbChannelFinalFieldSize(dbChan);
                }
                shared_vector<const string> data(freeze(xxx));
                PVStringArrayPtr pva = static_pointer_cast<PVStringArray>(pvArray);
                pva->replace(data);
                break;
            }
            default:
                throw std::logic_error("Should never get here");
            }
            bitSet->set(pvField->getFieldOffset());
        } else if((propertyMask&enumValueBit)!=0) {
            int32 val = 0;
            if(caData) {
                val = caData->intValue;
            } else {
                if (dbChannelFinalDBFType(dbChan) == DBF_DEVICE) {
                    val = static_cast<epicsEnum16>(dbChannelRecord(dbChan)->dtyp);
                } else {
                    val = static_cast<int32>(*static_cast<epicsEnum16 *>(dbChannelField(dbChan)));
                }
            }
            PVStructurePtr pvEnum = static_pointer_cast<PVStructure>(pvField);
            PVIntPtr pvIndex = pvEnum->getSubField<PVInt>(indexString);
            if(pvIndex.get() && pvIndex->get()!=val) {
                pvIndex->put(val);
                bitSet->set(pvIndex->getFieldOffset());
            }
        }
    }

    if((propertyMask&timeStampBit)!=0)
    {
        PVStructurePtr pvField = pvStructure->getSubFieldT<PVStructure>(timeStampString);
        epicsTimeStamp *epicsTimeStamp;
        struct dbCommon *precord = dbChannelRecord(dbChan);
        if(caData) {
            epicsTimeStamp = &caData->timeStamp;
        } else {
                epicsTimeStamp = &precord->time;     
        }
        PVLongPtr pvSecs = pvField->getSubField<PVLong>("secondsPastEpoch");
        if (pvSecs.get()) {
            int64 seconds  = epicsTimeStamp->secPastEpoch + POSIX_TIME_AT_EPICS_EPOCH;
            if(seconds != pvSecs->get()) {
                pvSecs->put(seconds);
                bitSet->set(pvSecs->getFieldOffset());
            }
        }
        PVIntPtr pvNsecs = pvField->getSubField<PVInt>("nanoseconds");
        if (pvNsecs.get()) {
            int32 nanoseconds= epicsTimeStamp->nsec;
            if(nanoseconds != pvNsecs->get()) {
                pvNsecs->put(nanoseconds);
                bitSet->set(pvNsecs->getFieldOffset());
            }
        }
    }

    if((propertyMask&alarmBit)!=0) {
        PVStructurePtr pvField = pvStructure->getSubFieldT<PVStructure>(alarmString);
        struct dbCommon *precord = dbChannelRecord(dbChan);
        string message;
        epicsEnum16 stat;
        epicsEnum16 sevr;
        if(caData) {
            message = caData->status;
            stat = caData->stat;
            sevr = caData->sevr;
        } else {
            message = dbrStatus2alarmMessage[precord->stat];
            stat = dbrStatus2alarmStatus[precord->stat];
            sevr = precord->sevr;
        }

        PVIntPtr pvStatus = pvField->getSubField<PVInt>("status");
        if (pvStatus.get() && stat != pvStatus->get()) {
            pvStatus->put(stat);
            bitSet->set(pvStatus->getFieldOffset());
        }

        PVIntPtr pvSeverity = pvField->getSubField<PVInt>("severity");
        if (pvSeverity.get() && sevr != pvSeverity->get()) {
            pvSeverity->put(sevr);
            bitSet->set(pvSeverity->getFieldOffset());
        }

        PVStringPtr pvMessage = pvField->getSubField<PVString>("message");
        if (pvMessage.get() && message != pvMessage->get()) {
            pvMessage->put(message);
            bitSet->set(pvMessage->getFieldOffset());
        }
    }

    getPropertyData(requester, propertyMask, dbChan,
        pvStructure, bitSet);

    return Status::Ok;
}

Status  DbUtil::put(
        Requester::shared_pointer const &requester,
        int propertyMask,
        dbChannel *dbChan,
        PVFieldPtr const &pvField)
{
    if((propertyMask&getValueBit)==0) {
        requester->message("Logic Error unknown field to put",errorMessage);
        return Status::Ok;
    }

    if((propertyMask&scalarValueBit)!=0) {
        PVScalarPtr pvScalar = static_pointer_cast<PVScalar>(pvField);
        ScalarType scalarType = pvScalar->getScalar()->getScalarType();
        switch(scalarType) {
        case pvByte: {
            int8 * val = static_cast<int8 *>(dbChannelField(dbChan));
            PVBytePtr pv = static_pointer_cast<PVByte>(pvField);
            *val = pv->get();
            break;
        }
        case pvUByte: {
            uint8 * val = static_cast<uint8 *>(dbChannelField(dbChan));
            PVUBytePtr pv = static_pointer_cast<PVUByte>(pvField);
            *val = pv->get();
            break;
        }
        case pvShort: {
            int16 * val = static_cast<int16 *>(dbChannelField(dbChan));
            PVShortPtr pv = static_pointer_cast<PVShort>(pvField);
            *val = pv->get();
            break;
        }
        case pvUShort: {
            uint16 * val = static_cast<uint16 *>(dbChannelField(dbChan));
            PVUShortPtr pv = static_pointer_cast<PVUShort>(pvField);
            *val = pv->get();
            break;
        }
        case pvInt: {
            int32 * val = static_cast<int32 *>(dbChannelField(dbChan));
            PVIntPtr pv = static_pointer_cast<PVInt>(pvField);
            *val = pv->get();
            break;
        }
        case pvUInt: {
            uint32 * val = static_cast<uint32 *>(dbChannelField(dbChan));
            PVUIntPtr pv = static_pointer_cast<PVUInt>(pvField);
            *val = pv->get();
            break;
        }
        case pvFloat: {
            float * val = static_cast<float *>(dbChannelField(dbChan));
            PVFloatPtr pv = static_pointer_cast<PVFloat>(pvField);
            *val = pv->get();
            break;
        }
        case pvDouble: {
            double * val = static_cast<double *>(dbChannelField(dbChan));
            PVDoublePtr pv = static_pointer_cast<PVDouble>(pvField);
            *val = pv->get();
            break;
        }
        case pvString: {
            char * to = static_cast<char *>(dbChannelField(dbChan));
            PVStringPtr pvString = static_pointer_cast<PVString>(pvField);
            if(pvString->get().empty()) {
                *(to) = 0;
            } else {
                int size = dbChannelFinalFieldSize(dbChan)-1;
                int fromLen = pvString->get().length();
                if(fromLen<size) size = fromLen;
                strncpy(to,pvString->get().c_str(),size);
                *(to + size) = 0;
            }
            break;
        }
        default:
            requester->message("Logic Error did not handle scalarType",errorMessage);
            return Status::Ok;
        }
    } else if((propertyMask&arrayValueBit)!=0) {
        PVScalarArrayPtr pvArray = static_pointer_cast<PVScalarArray>(pvField);
        ScalarType scalarType = pvArray->getScalarArray()->getElementType();
        long no_elements  = dbChannelFinalElements(dbChan);
        long length = pvArray->getLength();
        if(length>no_elements) length = no_elements;
        struct rset *prset = dbGetRset(&dbChan->addr);
        if(prset && prset->put_array_info) {
            put_array_info put_info;
            put_info = (put_array_info)(prset->put_array_info);
            put_info(&dbChan->addr, length);
        }
        switch(scalarType) {
        case pvByte: {
            PVByteArrayPtr pva = static_pointer_cast<PVByteArray>(pvArray);
            PVByteArray::const_svector xxx = pva->view();
            int8 *pv3 = static_cast<int8 *>(dbChannelField(dbChan));
            for(long i=0; i<length; i++) pv3[i] = xxx[i];
            break;
        }
        case pvUByte: {
            PVUByteArrayPtr pva = static_pointer_cast<PVUByteArray>(pvArray);
            PVUByteArray::const_svector xxx = pva->view();
            uint8 *pv3 = static_cast<uint8 *>(dbChannelField(dbChan));
            for(long i=0; i<length; i++) pv3[i] = xxx[i];
            break;
        }
        case pvShort: {
            PVShortArrayPtr pva = static_pointer_cast<PVShortArray>(pvArray);
            PVShortArray::const_svector xxx = pva->view();
            int16 *pv3 = static_cast<int16 *>(dbChannelField(dbChan));
            for(long i=0; i<length; i++) pv3[i] = xxx[i];
            break;
        }
        case pvUShort: {
            PVUShortArrayPtr pva = static_pointer_cast<PVUShortArray>(pvArray);
            PVUShortArray::const_svector xxx = pva->view();
            uint16 *pv3 = static_cast<uint16 *>(dbChannelField(dbChan));
            for(long i=0; i<length; i++) pv3[i] = xxx[i];
            break;
        }
        case pvInt: {
            PVIntArrayPtr pva = static_pointer_cast<PVIntArray>(pvArray);
            PVIntArray::const_svector xxx = pva->view();
            int32 *pv3 = static_cast<int32 *>(dbChannelField(dbChan));
            for(long i=0; i<length; i++) pv3[i] = xxx[i];
            break;
        }
        case pvUInt: {
            PVUIntArrayPtr pva = static_pointer_cast<PVUIntArray>(pvArray);
            PVUIntArray::const_svector xxx = pva->view();
            uint32 *pv3 = static_cast<uint32 *>(dbChannelField(dbChan));
            for(long i=0; i<length; i++) pv3[i] = xxx[i];
            break;
        }
        case pvFloat: {
            PVFloatArrayPtr pva = static_pointer_cast<PVFloatArray>(pvArray);
            PVFloatArray::const_svector xxx = pva->view();
            float *pv3 = static_cast<float *>(dbChannelField(dbChan));
            for(long i=0; i<length; i++) pv3[i] = xxx[i];
            break;
        }
        case pvDouble: {
            PVDoubleArrayPtr pva = static_pointer_cast<PVDoubleArray>(pvArray);
            PVDoubleArray::const_svector xxx = pva->view();
            double *pv3 = static_cast<double *>(dbChannelField(dbChan));
            for(long i=0; i<length; i++) pv3[i] = xxx[i];
            break;
        }
        case pvString: {
            PVStringArrayPtr pva = static_pointer_cast<PVStringArray>(pvArray);
            PVStringArray::const_svector xxx = pva->view();
            char *pv3 = static_cast<char *>(dbChannelField(dbChan));
            for(long i=0; i<length; i++) {
                const char * const pxxx = xxx[i].data();
                long strlen = xxx[i].length();
                if (strlen > dbChannelFinalFieldSize(dbChan))
                    strlen = dbChannelFinalFieldSize(dbChan);
                for(long j=0; j<strlen; j++) pv3[j] = pxxx[j];
                pv3 += dbChannelFinalFieldSize(dbChan);
            }
            break;
        }
        default:
            throw std::logic_error("Should never get here");
        }
    } else if((propertyMask&enumValueBit)!=0) {
        PVIntPtr pvIndex;
        PVStructurePtr pvEnum = static_pointer_cast<PVStructure>(pvField);
        pvIndex = pvEnum->getSubField<PVInt>(indexString);

        if(dbChannelFinalDBFType(dbChan) == DBF_MENU) {
            requester->message("Not allowed to change a menu field",errorMessage);
        } else if(dbChannelFinalDBFType(dbChan) == DBF_ENUM || dbChannelFinalDBFType(dbChan) == DBF_DEVICE) {
            epicsEnum16 *value = static_cast<epicsEnum16*>(dbChannelField(dbChan));
            if (pvIndex.get()) {
               *value = pvIndex->get();
            }
            else {
                PVStringArrayPtr pvChoices = pvEnum->getSubField<PVStringArray>(choicesString);
                if (pvChoices.get())
                {
                    requester->message("Can't change the choices field",errorMessage);
                }
                else
                {
                    requester->message("Logic error. Putting to a enum subfield that's not index or choices", errorMessage);
                }
                return Status::Ok;
            }
        } else {
            requester->message("Logic Error unknown enum field",errorMessage);
            return Status::Ok;
        }
    } else {
        requester->message("Logic Error unknown field to put",errorMessage);
        return Status::Ok;
    }
    dbCommon *precord = dbChannelRecord(dbChan);
    dbFldDes *pfldDes = dbChannelFldDes(dbChan);
    int isValueField = dbIsValueField(pfldDes);
    if(isValueField) precord->udf = 0;
    bool post = false;
    if(!(propertyMask&processBit)) post = true;
    if(precord->mlis.count && !(isValueField && pfldDes->process_passive)) post = true;
    if(post) {
        db_post_events(precord, dbChannelField(dbChan), DBE_VALUE | DBE_LOG);
    }
    return Status::Ok;
}

Status  DbUtil::putField(
        Requester::shared_pointer const &requester,
        int propertyMask,
        dbChannel *dbChan,
        PVFieldPtr const &pvField)
{
    const void *pbuffer = 0;
    short dbrType = 0;
    int8 bvalue;
    uint8 ubvalue;
    int16 svalue;
    uint16 usvalue;
    int32 ivalue;
    uint32 uivalue;
    float fvalue;
    double dvalue;
    string string;
    if((propertyMask&getValueBit)==0) {
        requester->message("Logic Error unknown field to put",errorMessage);
        return Status::Ok;
    }

    if((propertyMask&scalarValueBit)!=0) {
        PVScalarPtr pvScalar = static_pointer_cast<PVScalar>(pvField);
        ScalarType scalarType = pvScalar->getScalar()->getScalarType();
        switch(scalarType) {
        case pvByte: {
            PVBytePtr pv = static_pointer_cast<PVByte>(pvField);
            bvalue = pv->get(); pbuffer = &bvalue;
            dbrType = DBF_CHAR;
            break;
        }
        case pvUByte: {
            PVUBytePtr pv = static_pointer_cast<PVUByte>(pvField);
            ubvalue = pv->get(); pbuffer = &ubvalue;
            dbrType = DBF_UCHAR;
            break;
        }
        case pvShort: {
            PVShortPtr pv = static_pointer_cast<PVShort>(pvField);
            svalue = pv->get(); pbuffer = &svalue;
            dbrType = DBF_SHORT;
            break;
        }
        case pvUShort: {
            PVUShortPtr pv = static_pointer_cast<PVUShort>(pvField);
            usvalue = pv->get(); pbuffer = &usvalue;
            dbrType = DBF_USHORT;
            break;
        }
        case pvInt: {
            PVIntPtr pv = static_pointer_cast<PVInt>(pvField);
            ivalue = pv->get(); pbuffer = &ivalue;
            dbrType = DBF_LONG;
            break;
        }
        case pvUInt: {
            PVUIntPtr pv = static_pointer_cast<PVUInt>(pvField);
            uivalue = pv->get(); pbuffer = &uivalue;
            dbrType = DBF_ULONG;
            break;
        }
        case pvFloat: {
            PVFloatPtr pv = static_pointer_cast<PVFloat>(pvField);
            fvalue = pv->get(); pbuffer = &fvalue;
            dbrType = DBF_FLOAT;
            break;
        }
        case pvDouble: {
            PVDoublePtr pv = static_pointer_cast<PVDouble>(pvField);
            dvalue = pv->get(); pbuffer = &dvalue;
            dbrType = DBF_DOUBLE;
            break;
        }
        case pvString: {
            PVStringPtr pvString = static_pointer_cast<PVString>(pvField);
            string = pvString->get();
            pbuffer = string.c_str();
            dbrType = DBF_STRING;
            break;
        }
        default:
            requester->message("Logic Error did not handle scalarType",errorMessage);
            return Status::Ok;
        }
    } else if((propertyMask&enumValueBit)!=0) {
        PVIntPtr pvIndex;
        if(pvField->getField()->getType()==structure) {
            PVStructurePtr pvEnum = static_pointer_cast<PVStructure>(pvField);
            pvIndex = pvEnum->getSubFieldT<PVInt>(indexString);
        } else {
            pvIndex = static_pointer_cast<PVInt>(pvField);
        }
        svalue = pvIndex->get(); pbuffer = &svalue;
        dbrType = DBF_ENUM;
    } else {
        requester->message("Logic Error unknown field to put",errorMessage);
        return Status::Ok;
    }
    long status = dbPutField(&dbChan->addr, dbrType, pbuffer, 1);
    if(status!=0) {
        char buf[30];
        sprintf(buf,"dbPutField returned error 0x%lx",status);
        std::string message(buf);
        requester->message(message,warningMessage);
    }
    return Status::Ok;
}

ScalarType DbUtil::getScalarType(
        Requester::shared_pointer const &requester,
        dbChannel *dbChan)
{
    switch (dbChannelFinalDBFType(dbChan)) {
    case DBF_CHAR:
        return pvByte;
    case DBF_UCHAR:
        return pvUByte;
    case DBF_SHORT:
        return pvShort;
    case DBF_USHORT:
        return pvUShort;
    case DBF_LONG:
        return pvInt;
    case DBF_ULONG:
        return pvUInt;
    case DBF_FLOAT:
        return pvFloat;
    case DBF_DOUBLE:
        return pvDouble;
    case DBF_STRING:
        return pvString;
    default:
        break;
    }
    // Note that pvBoolean is not a supported type
    return pvBoolean;
}

}}
