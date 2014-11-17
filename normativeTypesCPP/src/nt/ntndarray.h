/* ntndarray.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
#ifndef NTNDARRAY_H
#define NTNDARRAY_H

#include <pv/ntfield.h>

#include <vector>
#include <string>

namespace epics { namespace nt {

class NTNDArray;
typedef std::tr1::shared_ptr<NTNDArray> NTNDArrayPtr;

namespace detail {

    /**
     * Interface for in-line creating of NTNDArray.
     * One instance can be used to create multiple instances.
     * An instance of this object must not be used concurrently (an object has a state).
     * @author mse
     */
    class epicsShareClass NTNDArrayBuilder :
        public std::tr1::enable_shared_from_this<NTNDArrayBuilder>
    {
    public:
        POINTER_DEFINITIONS(NTNDArrayBuilder);

        /**
         * Add descriptor field to the NTNDArray.
         * @return this instance of a {@code NTNDArrayBuilder}.
         */
        shared_pointer addDescriptor();

        /**
         * Add alarm structure to the NTNDArray.
         * @return this instance of a {@code NTNDArrayBuilder}.
         */
        shared_pointer addAlarm();

        /**
         * Add timeStamp structure to the NTNDArray.
         * @return this instance of a {@code NTNDArrayBuilder}.
         */
        shared_pointer addTimeStamp();

        /**
         * Add display structure to the NTNDArray.
         * @return this instance of a {@code NTNDArrayBuilder}.
         */
        shared_pointer addDisplay();

        /**
         * Create a {@code Structure} that represents NTNDArray.
         * This resets this instance state and allows new instance to be created.
         * @return a new instance of a {@code Structure}.
         */
        epics::pvData::StructureConstPtr createStructure();

        /**
         * Create a {@code PVStructure} that represents NTNDArray.
         * This resets this instance state and allows new {@code instance to be created.
         * @return a new instance of a {@code PVStructure}
         */
        epics::pvData::PVStructurePtr createPVStructure();

        /**
         * Create a {@code NTNDArray} instance.
         * This resets this instance state and allows new {@code instance to be created.
         * @return a new instance of a {@code NTNDArray}
         */
        NTNDArrayPtr create();
        /**
         * Add extra {@code Field} to the type.
         * @param name name of the field.
         * @param field a field to add.
         * @return this instance of a {@code NTArrayBuilder}.
         */
        shared_pointer add(std::string const & name, epics::pvData::FieldConstPtr const & field);

    private:
        NTNDArrayBuilder();

        void reset();

        bool descriptor;
        bool timeStamp;
        bool alarm;
        bool display;

        // NOTE: this preserves order, however it does not handle duplicates
        epics::pvData::StringArray extraFieldNames;
        epics::pvData::FieldConstPtrArray extraFields;

        friend class ::epics::nt::NTNDArray;
    };

}

typedef std::tr1::shared_ptr<detail::NTNDArrayBuilder> NTNDArrayBuilderPtr;

/**
 * Convenience Class for NTNDArray
 * @author dgh
 */
class NTNDArray
{
public:
    POINTER_DEFINITIONS(NTNDArray);

    static const std::string URI;

    /**
     * Wrap (aka dynamic cast, or wrap) the structure to NTNDArray.
     * First isCompatible is called.
     * This method will nullptr if the structure is is not compatible.
     * This method will nullptr if the structure is nullptr.
     * @param structure The structure to wrap-ed (dynamic cast, wrapped) to NTNDArray.
     * @return NTNDArray instance on success, nullptr otherwise.
     */
    static shared_pointer wrap(epics::pvData::PVStructurePtr const & structure);

    /**
     * Wrap (aka dynamic cast, or wrap) the structure to NTMultiChannel without checking for isCompatible
     * @param structure The structure to wrap-ed (dynamic cast, wrapped) to NTNDArray.
     * @return NTNDArray instance.
     */
    static shared_pointer wrapUnsafe(epics::pvData::PVStructurePtr const & structure);

    /**
     * Is the structure an NTNDArray.
     * @param structure The structure to test.
     * @return (false,true) if (is not, is) an NTNDArray.
     */
    static bool is_a(epics::pvData::StructureConstPtr const & structure);

    /**
     * Is the pvStructure compatible with  NTNDArray..
     * This method introspects the fields to see if they are compatible.
     * @param pvStructure The pvStructure to test.
     * @return (false,true) if (is not, is) an NTMultiChannel.
     */
    static bool isCompatible(
        epics::pvData::PVStructurePtr const &pvStructure);
    /**
     * Create a NTNDArrayBuilder instance
     * @return builder instance.
     */
    static NTNDArrayBuilderPtr createBuilder();

    /**
     * Destructor.
     */
    ~NTNDArray() {}

     /**
      * Attach a pvTimeStamp to timeStamp field.
      * @param pvTimeStamp The pvTimeStamp that will be attached.
      * Does nothing if no timeStamp.
      * @return true if the operation was successfull (i.e. this instance has a timeStamp field), otherwise false.
      */
    bool attachTimeStamp(epics::pvData::PVTimeStamp &pvTimeStamp) const;

     /**
      * Attach a pvTimeStamp to dataTimeStamp field.
      * @param pvTimeStamp The pvTimeStamp that will be attached.
      * Does nothing if no timeStamp.
      * @return true if the operation was successfull (i.e. this instance has a timeStamp field), otherwise false.
      */
    bool attachDataTimeStamp(epics::pvData::PVTimeStamp &pvTimeStamp) const;

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
     * Get the value field.
     * @return The PVField for the values.
     */
    epics::pvData::PVUnionPtr getValue() const;

    /**
     * Get the codec field.
     * @return the PVStructurePtr.
     */
    epics::pvData::PVStructurePtr getCodec() const;

    /**
     * Get the compressedDataSize field.
     * @return PVStructurePtr.
     */
    epics::pvData::PVLongPtr getCompressedDataSize() const;

    /**
     * Get the uncompressedDataSize field.
     * @return PVStructurePtr.
     */
    epics::pvData::PVLongPtr getUncompressedDataSize() const;

    /**
     * Get the attribute field.
     * @return the PVStructurePtr.
     */
    epics::pvData::PVStructureArrayPtr getAttribute() const;

    /**
     * Get the dimension field.
     * @return the PVStructurePtr.
     */
    epics::pvData::PVStructureArrayPtr getDimension() const;

    /**
     * Get the uniqueId field.
     * @return PVStructurePtr.
     */
    epics::pvData::PVIntPtr getUniqueId() const;

    /**
     * Get the data timeStamp field.
     * @return PVStructurePtr.
     */
    epics::pvData::PVStructurePtr getDataTimeStamp() const;

    /**
     * Get the descriptor field.
     * @return The pvString or null if no function field.
     */
    epics::pvData::PVStringPtr getDescriptor() const;

    /**
     * Get the timeStamp field.
     * @return PVStructurePtr which may be null.
     */
    epics::pvData::PVStructurePtr getTimeStamp() const;

    /**
     * Get the alarm field.
     * @return PVStructurePtr which may be null.
     */
    epics::pvData::PVStructurePtr getAlarm() const;

    /**
     * Get the display field.
     * @return PVStructurePtr which may be null.
     */
    epics::pvData::PVStructurePtr getDisplay() const;

private:
    NTNDArray(epics::pvData::PVStructurePtr const & pvStructure);
    epics::pvData::PVStructurePtr pvNTNDArray;

    friend class detail::NTNDArrayBuilder;
};

}}
#endif  /* NTNDARRAY_H */
