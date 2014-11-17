/* ntmultiChannel.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
#ifndef NTMULTICHANNEL_H
#define NTMULTICHANNEL_H

#include <pv/ntfield.h>
#include <vector>
#include <string>


namespace epics { namespace nt { 

/**
 * Convenience Class for NTMultiChannel
 * @author mrk
 *
 */

class NTMultiChannel;
typedef std::tr1::shared_ptr<NTMultiChannel> NTMultiChannelPtr;

namespace detail {

    /**
     * Interface for in-line creating of NTMultiChannel.
     * One instance can be used to create multiple instances.
     * An instance of this object must not be used concurrently (an object has a state).
     * @author mse
     */
    class epicsShareClass NTMultiChannelBuilder :
        public std::tr1::enable_shared_from_this<NTMultiChannelBuilder>
    {
    public:
        POINTER_DEFINITIONS(NTMultiChannelBuilder);
        /**
         * specify the union for the value field.
         * If this is not called then a variantUnion is the default.
         * @return this instance of a {@code NTMultiChannelBuilder}.
         */
        shared_pointer value(epics::pvData::UnionConstPtr valuePtr);
        /**
         * Add descriptor field to the NTMultiChannel.
         * @return this instance of a {@code NTMultiChannelBuilder}.
         */
        shared_pointer addDescriptor();
        /**
         * Add alarm structure to the NTMultiChannel.
         * @return this instance of a {@code NTMultiChannelBuilder}.
         */
        shared_pointer addAlarm();
        /**
         * Add timeStamp structure to the NTMultiChannel.
         * @return this instance of a {@code NTMultiChannelBuilder}.
         */
        shared_pointer addTimeStamp();
        /**
         * Add severity array to the NTMultiChannel.
         * @return this instance of a {@code NTMultiChannelBuilder}.
         */
        shared_pointer addSeverity();
        /**
         * Add status array to the NTMultiChannel.
         * @return this instance of a {@code NTMultiChannelBuilder}.
         */
        shared_pointer addStatus();
        /**
         * Add message array to the NTMultiChannel.
         * @return this instance of a {@code NTMultiChannelBuilder}.
         */
        shared_pointer addMessage();
        /**
         * Add secondsPastEpoch array to the NTMultiChannel.
         * @return this instance of a {@code NTMultiChannelBuilder}.
         */
        shared_pointer addSecondsPastEpoch();
        /**
         * Add nanoseconds array to the NTMultiChannel.
         * @return this instance of a {@code NTMultiChannelBuilder}.
         */
        shared_pointer addNanoseconds();
        /**
         * Add userTag array to the NTMultiChannel.
         * @return this instance of a {@code NTMultiChannelBuilder}.
         */
        shared_pointer addUserTag();
        /**
         * Create a {@code Structure} that represents NTMultiChannel.
         * This resets this instance state and allows new instance to be created.
         * @return a new instance of a {@code Structure}.
         */
        epics::pvData::StructureConstPtr createStructure();
        /**
         * Create a {@code PVStructure} that represents NTMultiChannel.
         * This resets this instance state and allows new {@code instance to be created.}
         * @return a new instance of a {@code PVStructure}
         */
        epics::pvData::PVStructurePtr createPVStructure();
        /**
         * Create a {@code NTMultiChannel} instance.
         * This resets this instance state and allows new {@code instance to be created.}
         * @return a new instance of a {@code NTMultiChannel}
         */
        NTMultiChannelPtr create();
        /**
         * Add extra {@code Field} to the type.
         * @param name name of the field.
         * @param field a field to add.
         * @return this instance of a {@code NTMultiChannelBuilder}.
         */
        shared_pointer add(std::string const & name, epics::pvData::FieldConstPtr const & field);
    private:
        NTMultiChannelBuilder();

        void reset();

        epics::pvData::UnionConstPtr valueType;
        bool descriptor;
        bool alarm;
        bool timeStamp;
        bool severity;
        bool status;
        bool message;
        bool secondsPastEpoch;
        bool nanoseconds;
        bool userTag;

        // NOTE: this preserves order, however it does not handle duplicates
        epics::pvData::StringArray extraFieldNames;
        epics::pvData::FieldConstPtrArray extraFields;

        friend class ::epics::nt::NTMultiChannel;
    };

}

typedef std::tr1::shared_ptr<detail::NTMultiChannelBuilder> NTMultiChannelBuilderPtr;


class NTMultiChannel
{
public:
    POINTER_DEFINITIONS(NTMultiChannel);

    static const std::string URI;

    /**
     * Wrap (aka dynamic cast, or wrap) the structure to NTMultiChannel.
     * First isCompatible is called.
     * This method will nullptr if the structure is is not compatible.
     * @param structure The structure to wrap-ed (dynamic cast, wrapped) to NTMultiChannel.
     * @return NTMultiChannel instance on success, nullptr otherwise.
     */
    static shared_pointer wrap(epics::pvData::PVStructurePtr const & structure);

    /**
     * Wrap (aka dynamic cast, or wrap) the structure to NTMultiChannel without checking for isCompatible
     * @param structure The structure to wrap-ed (dynamic cast, wrapped) to NTMultiChannel.
     * @return NTMultiChannel instance.
     */
    static shared_pointer wrapUnsafe(epics::pvData::PVStructurePtr const & structure);
    /**
     * Is the Structure an NTMultiChannel.
     * This method structure->getID() and checks if it is the same as the URI.
     * @param structure The structure to test.
     * @return (false,true) if (is not, is) an NTMultiChannel.
     */
    static bool is_a(
        epics::pvData::StructureConstPtr const &structure);
    /**
     * Is the pvStructure compatible with  NTMultiChannel.
     * This method introspects the fields to see if they are compatible.
     * @param pvStructure The pvStructure to test.
     * @return (false,true) if (is not, is) an NTMultiChannel.
     */
    static bool isCompatible(
        epics::pvData::PVStructurePtr const &pvStructure);
    /**
     * Create a NTMultiChannelBuilder instance
     * @return builder instance.
     */
    static  NTMultiChannelBuilderPtr createBuilder();

    /**
     * Destructor
     */
    ~NTMultiChannel() {}
     /**
      * Attach a pvTimeStamp.
      * @param pvTimeStamp The pvTimeStamp that will be attached.
      * Does nothing if no timeStamp
      */
    void attachTimeStamp(epics::pvData::PVTimeStamp &pvTimeStamp) const;
    /**
     * Attach a pvAlarm.
     * @param pvAlarm The pvAlarm that will be attached.
     * Does nothing if no alarm
     */
    void attachAlarm(epics::pvData::PVAlarm &pvAlarm) const;
    /**
     * Get the pvStructure.
     * @return PVStructurePtr.
     */
    epics::pvData::PVStructurePtr getPVStructure() const
    {return pvNTMultiChannel;}
    /**
     * Get the timeStamp.
     * @return PVStructurePtr which may be null.
     */
    epics::pvData::PVStructurePtr getTimeStamp() const
    {return pvTimeStamp;}
    /**
     * Get the alarm.
     * @return PVStructurePtr which may be null.
     */
    epics::pvData::PVStructurePtr getAlarm() const
     {return pvAlarm;}
    /**
     * Get the value of each channel.
     * @return PVUnionArrayPtr
     */
    epics::pvData::PVUnionArrayPtr getValue() const 
    {return pvValue;}
    /**
     * Get the channelName of each channel.
     * @return PVStringArrayPtr
     */
    epics::pvData::PVStringArrayPtr getChannelName() const 
    { return pvChannelName;};
    /**
     * Get the connection state of each channel.
     * @return PVBooleanArrayPtr
     */
    epics::pvData::PVBooleanArrayPtr getIsConnected() const 
    { return pvIsConnected;};
    /**
     * Get the severity of each channel.
     * @return PVIntArrayPtr which may be null.
     */
    epics::pvData::PVIntArrayPtr getSeverity() const 
    {return pvSeverity;}
    /**
     * Get the status of each channel.
     * @return PVIntArrayPtr which may be null.
     */
    epics::pvData::PVIntArrayPtr getStatus() const 
    {return pvStatus;}
    /**
     * Get the message of each chnnel.
     * @return PVStringArrayPtr which may be null.
     */
    epics::pvData::PVStringArrayPtr getMessage() const 
    {return pvMessage;}
    /**
     * Get the secondsPastEpoch of each channel.
     * @return PVLongArrayPtr which may be null.
     */
    epics::pvData::PVLongArrayPtr getSecondsPastEpoch() const 
    {return pvSecondsPastEpoch;}
    /**
     * Get the nanoseconds of each channel.
     * @return PVIntArrayPtr which may be null.
     */
    epics::pvData::PVIntArrayPtr getNanoseconds() const 
    {return pvNanoseconds;}
    /**
     * Get the userTag of each channel.
     * @return PVIntArrayPtr which may be null.
     */
    epics::pvData::PVIntArrayPtr getUserTag() const 
    {return pvUserTag;}
    /**
     * Get the descriptor.
     * @return PVStringPtr which may be null.
     */
    epics::pvData::PVStringPtr getDescriptor() const 
    {return pvDescriptor;}
private:
    NTMultiChannel(epics::pvData::PVStructurePtr const & pvStructure);
    epics::pvData::PVStructurePtr pvNTMultiChannel;
    epics::pvData::PVStructurePtr pvTimeStamp;
    epics::pvData::PVStructurePtr pvAlarm;
    epics::pvData::PVUnionArrayPtr pvValue;
    epics::pvData::PVStringArrayPtr pvChannelName;
    epics::pvData::PVBooleanArrayPtr pvIsConnected;
    epics::pvData::PVIntArrayPtr pvSeverity;
    epics::pvData::PVIntArrayPtr pvStatus;
    epics::pvData::PVStringArrayPtr pvMessage;
    epics::pvData::PVLongArrayPtr pvSecondsPastEpoch;
    epics::pvData::PVIntArrayPtr pvNanoseconds;
    epics::pvData::PVIntArrayPtr pvUserTag;
    epics::pvData::PVStringPtr pvDescriptor;
    friend class detail::NTMultiChannelBuilder;
};

}}
#endif  /* NTMULTICHANNEL_H */
