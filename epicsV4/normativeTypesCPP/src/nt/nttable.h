/* nttable.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
#ifndef NTTABLE_H
#define NTTABLE_H

#include <vector>
#include <string>

#ifdef epicsExportSharedSymbols
#   define nttableEpicsExportSharedSymbols
#   undef epicsExportSharedSymbols
#endif

#include <pv/pvDisplay.h>
#include <pv/pvControl.h>

#ifdef nttableEpicsExportSharedSymbols
#   define epicsExportSharedSymbols
#	undef nttableEpicsExportSharedSymbols
#endif

#include <pv/ntfield.h>

#include <shareLib.h>

namespace epics { namespace nt {

class NTTable;
typedef std::tr1::shared_ptr<NTTable> NTTablePtr;

namespace detail {

    /**
     * Interface for in-line creating of NTTable.
     * One instance can be used to create multiple instances.
     * An instance of this object must not be used concurrently (an object has a state).
     * @author mse
     */
    class epicsShareClass NTTableBuilder :
        public std::tr1::enable_shared_from_this<NTTableBuilder>
    {
    public:
        POINTER_DEFINITIONS(NTTableBuilder);

        /**
         * Add a column of given {@code Scalar} type.
         * @param name name of the column.
         * @param elementType column type, a scalar array.
         * @return this instance of a {@code NTTableBuilder}.
         */
        shared_pointer add(std::string const & name, epics::pvData::ScalarType elememtType);

        /**
         * Add descriptor field to the NTTable.
         * @return this instance of a {@code NTTableBuilder}.
         */
        shared_pointer addDescriptor();

        /**
         * Add alarm structure to the NTTable.
         * @return this instance of a {@code NTTableBuilder}.
         */
        shared_pointer addAlarm();

        /**
         * Add timeStamp structure to the NTTable.
         * @return this instance of a {@code NTTableBuilder}.
         */
        shared_pointer addTimeStamp();

        /**
         * Create a {@code Structure} that represents NTTable.
         * This resets this instance state and allows new instance to be created.
         * @return a new instance of a {@code Structure}.
         */
        epics::pvData::StructureConstPtr createStructure();

        /**
         * Create a {@code PVStructure} that represents NTTable.
         * This resets this instance state and allows new {@code instance to be created.
         * @return a new instance of a {@code PVStructure}
         */
        epics::pvData::PVStructurePtr createPVStructure();

        /**
         * Create a {@code NTTable} instance.
         * This resets this instance state and allows new {@code instance to be created.
         * @return a new instance of a {@code NTTable}
         */
        NTTablePtr create();
        /**
         * Add extra {@code Field} to the type.
         * @param name name of the field.
         * @param field a field to add.
         * @return this instance of a {@code NTTableBuilder}.
         */
        shared_pointer add(std::string const & name, epics::pvData::FieldConstPtr const & field);

    private:
        NTTableBuilder();

        void reset();

        std::vector<std::string> labels;
        std::vector<epics::pvData::ScalarType> types;

        bool descriptor;
        bool alarm;
        bool timeStamp;

        // NOTE: this preserves order, however it does not handle duplicates
        epics::pvData::StringArray extraFieldNames;
        epics::pvData::FieldConstPtrArray extraFields;

        friend class ::epics::nt::NTTable;
    };

}

typedef std::tr1::shared_ptr<detail::NTTableBuilder> NTTableBuilderPtr;



/**
 * Convenience Class for NTTable
 * @author mrk
 */
class epicsShareClass NTTable
{
public:
    POINTER_DEFINITIONS(NTTable);

    static const std::string URI;

    /**
     * Wrap (aka dynamic cast, or wrap) the structure to NTTable.
     * First isCompatible is called.
     * This method will nullptr if the structure is is not compatible.
     * This method will nullptr if the structure is nullptr.
     * @param structure The structure to wrap-ed (dynamic cast, wrapped) to NTTable.
     * @return NTTable instance on success, nullptr otherwise.
     */
    static shared_pointer wrap(epics::pvData::PVStructurePtr const & structure);

    /**
     * Wrap (aka dynamic cast, or wrap) the structure to NTMultiChannel without checking for isCompatible
     * @param structure The structure to wrap-ed (dynamic cast, wrapped) to NTTable.
     * @return NTTable instance.
     */
    static shared_pointer wrapUnsafe(epics::pvData::PVStructurePtr const & structure);

    /**
     * Is the structure an NTTable.
     * @param structure The structure to test.
     * @return (false,true) if (is not, is) an NTTable.
     */
    static bool is_a(epics::pvData::StructureConstPtr const & structure);
    /**
     * Is the pvStructure compatible with  NTTable.
     * This method introspects the fields to see if they are compatible.
     * @param pvStructure The pvStructure to test.
     * @return (false,true) if (is not, is) an NTMultiChannel.
     */
    static bool isCompatible(
        epics::pvData::PVStructurePtr const &pvStructure);
    /**
     * Create a NTTable builder instance.
     * @return builder instance.
     */
    static NTTableBuilderPtr createBuilder();

    /**
     * Destructor.
     */
    ~NTTable() {}

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
     * Get the labels field.
     * @return The pvStringArray for the labels.
     */
    epics::pvData::PVStringArrayPtr getLabels() const;

    /**
     * Get the PVField (column) for a field that follows the label field.
     * @param columnName The name of the column.
     * @return The PVFieldPtr for the field.
     */
    epics::pvData::PVFieldPtr getColumn(std::string const & columnName) const;

    /**
     * Get the PVField (column) for a field that follows the label field of a specified type (e.g. PVDoubleArray).
     * @param columnName The name of the column.
     * @return The <PVT> field.
     */
    template<typename PVT>
    std::tr1::shared_ptr<PVT> getColumn(std::string const & columnName) const
    {
        epics::pvData::PVFieldPtr pvField = getColumn(columnName);
        if (pvField.get())
            return std::tr1::dynamic_pointer_cast<PVT>(pvField);
        else
            return std::tr1::shared_ptr<PVT>();
    }

private:
    NTTable(epics::pvData::PVStructurePtr const & pvStructure);
    epics::pvData::PVStructurePtr pvNTTable;
    friend class detail::NTTableBuilder;
};

}}
#endif  /* NTTABLE_H */
