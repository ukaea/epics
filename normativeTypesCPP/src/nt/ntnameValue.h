/* ntnameValue.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
#ifndef NTNAMEVALUE_H
#define NTNAMEVALUE_H

#ifdef epicsExportSharedSymbols
#   define ntnameValueEpicsExportSharedSymbols
#   undef epicsExportSharedSymbols
#endif

#include <pv/pvDisplay.h>
#include <pv/pvControl.h>

#ifdef ntnameValueEpicsExportSharedSymbols
#   define epicsExportSharedSymbols
#	undef ntnameValueEpicsExportSharedSymbols
#endif

#include <pv/ntfield.h>

#include <shareLib.h>

namespace epics { namespace nt {

class NTNameValue;
typedef std::tr1::shared_ptr<NTNameValue> NTNameValuePtr;

namespace detail {

    /**
     * Interface for in-line creating of NTNameValue.
     * One instance can be used to create multiple instances.
     * An instance of this object must not be used concurrently (an object has a state).
     * @author mse
     */
    class epicsShareClass NTNameValueBuilder :
        public std::tr1::enable_shared_from_this<NTNameValueBuilder>
    {
    public:
        POINTER_DEFINITIONS(NTNameValueBuilder);

        /**
         * Set a value array {@code Scalar} type.
         * @param scalarType value array scalar array.
         * @return this instance of a {@code NTTableBuilder}.
         */
        shared_pointer value(epics::pvData::ScalarType scalarType);

        /**
         * Add descriptor field to the NTNameValue.
         * @return this instance of a {@code NTNameValueBuilder}.
         */
        shared_pointer addDescriptor();

        /**
         * Add alarm structure to the NTNameValue.
         * @return this instance of a {@code NTNameValueBuilder}.
         */
        shared_pointer addAlarm();

        /**
         * Add timeStamp structure to the NTNameValue.
         * @return this instance of a {@code NTNameValueBuilder}.
         */
        shared_pointer addTimeStamp();

        /**
         * Create a {@code Structure} that represents NTNameValue.
         * This resets this instance state and allows new instance to be created.
         * @return a new instance of a {@code Structure}.
         */
        epics::pvData::StructureConstPtr createStructure();

        /**
         * Create a {@code PVStructure} that represents NTNameValue.
         * This resets this instance state and allows new {@code instance to be created.
         * @return a new instance of a {@code PVStructure}
         */
        epics::pvData::PVStructurePtr createPVStructure();

        /**
         * Create a {@code NTNameValue} instance.
         * This resets this instance state and allows new {@code instance to be created.
         * @return a new instance of a {@code NTNameValue}
         */
        NTNameValuePtr create();

        /**
         * Add extra {@code Field} to the type.
         * @param name name of the field.
         * @param field a field to add.
         * @return this instance of a {@code NTNameValueBuilder}.
         */
        shared_pointer add(std::string const & name, epics::pvData::FieldConstPtr const & field);

    private:
        NTNameValueBuilder();

        void reset();

        bool valueTypeSet;
        epics::pvData::ScalarType valueType;

        bool descriptor;
        bool alarm;
        bool timeStamp;

        // NOTE: this preserves order, however it does not handle duplicates
        epics::pvData::StringArray extraFieldNames;
        epics::pvData::FieldConstPtrArray extraFields;

        friend class ::epics::nt::NTNameValue;
    };

}

typedef std::tr1::shared_ptr<detail::NTNameValueBuilder> NTNameValueBuilderPtr;



/**
 * Convenience Class for NTNameValue
 * @author mrk
 */
class epicsShareClass NTNameValue
{
public:
    POINTER_DEFINITIONS(NTNameValue);

    static const std::string URI;

    /**
     * Wrap (aka dynamic cast, or wrap) the structure to NTNameValue.
     * First isCompatible is called.
     * This method will nullptr if the structure is is not compatible.
     * This method will nullptr if the structure is nullptr.
     * @param structure The structure to wrap-ed (dynamic cast, wrapped) to NTNameValue.
     * @return NTNameValue instance on success, nullptr otherwise.
     */
    static shared_pointer wrap(epics::pvData::PVStructurePtr const & structure);

    /**
     * Wrap (aka dynamic cast, or wrap) the structure to NTMultiChannel without checking for isCompatible
     * @param structure The structure to wrap-ed (dynamic cast, wrapped) to NTNameValue.
     * @return NTNameValue instance.
     */
    static shared_pointer wrapUnsafe(epics::pvData::PVStructurePtr const & structure);

    /**
     * Is the structure an NTNameValue.
     * @param structure The structure to test.
     * @return (false,true) if (is not, is) an NTNameValue.
     */
    static bool is_a(epics::pvData::StructureConstPtr const & structure);
    /**
     * Is the pvStructure compatible with  NTNameValue..
     * This method introspects the fields to see if they are compatible.
     * @param pvStructure The pvStructure to test.
     * @return (false,true) if (is not, is) an NTMultiChannel.
     */
    static bool isCompatible(
        epics::pvData::PVStructurePtr const &pvStructure);
    /**
     * Create a NTNameValue builder instance.
     * @return builder instance.
     */
    static NTNameValueBuilderPtr createBuilder();

    /**
     * Destructor.
     */
    ~NTNameValue() {}

     /**
      * Attach a pvTimeStamp.
      * @param pvTimeStamp The pvTimeStamp that will be attached.
      * Does nothing if no timeStamp.
      * @return true if the operation was successfull (i.e. this instance has a timeStamp field), otherwise false.
      */
    bool attachTimeStamp(epics::pvData::PVTimeStamp &pvTimeStamp) const;

    /**
     * Attach an pvAlarm.
     * @param pvAlarm The pvAlarm that will be attached.
     * Does nothing if no alarm.
      * @return true if the operation was successfull (i.e. this instance has a timeStamp field), otherwise false.
     */
    bool attachAlarm(epics::pvData::PVAlarm &pvAlarm) const;

    /**
     * Get the pvStructure.
     * @return PVStructurePtr.
     */
    epics::pvData::PVStructurePtr getPVStructure() const;

    /**
     * Get the descriptor field.
     * @return The pvString or null if no function field.
     */
    epics::pvData::PVStringPtr getDescriptor() const;

    /**
     * Get the timeStamp.
     * @return PVStructurePtr which may be null.
     */
    epics::pvData::PVStructurePtr getTimeStamp() const;

    /**
     * Get the alarm.
     * @return PVStructurePtr which may be null.
     */
    epics::pvData::PVStructurePtr getAlarm() const;

    /**
     * Get the name array field.
     * @return The PVStringArray for the name.
     */
    epics::pvData::PVStringArrayPtr getName() const;

    /**
     * Get the value array field.
     * @return The PVField for the value.
     */
    epics::pvData::PVFieldPtr getValue() const;

    /**
     * Get the value array field of a specified type (e.g. PVDoubleArray).
     * @return The <PVT> array for the value.
     */
    template<typename PVT>
    std::tr1::shared_ptr<PVT> getValue() const
    {
        epics::pvData::PVFieldPtr pvField = getValue();
        if (pvField.get())
            return std::tr1::dynamic_pointer_cast<PVT>(pvField);
        else
            return std::tr1::shared_ptr<PVT>();
    }

private:
    NTNameValue(epics::pvData::PVStructurePtr const & pvStructure);
    epics::pvData::PVStructurePtr pvNTNameValue;
    friend class detail::NTNameValueBuilder;
};

}}
#endif  /* NTNAMEVALUE_H */
