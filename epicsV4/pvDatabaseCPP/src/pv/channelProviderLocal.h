/* channelProviderLocal.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author Marty Kraimer
 * @date 2013.04
 */
#ifndef CHANNELPROVIDERLOCAL_H
#define CHANNELPROVIDERLOCAL_H

#ifdef epicsExportSharedSymbols
#   define channelProviderLocalEpicsExportSharedSymbols
#   undef epicsExportSharedSymbols
#endif

#include <string>
#include <cstring>
#include <stdexcept>
#include <memory>
#include <set>

#include <pv/lock.h>
#include <pv/pvType.h>
#include <pv/pvData.h>
#include <pv/pvCopy.h>
#include <pv/pvAccess.h>
#include <pv/status.h>
#include <pv/serverContext.h>

#ifdef channelProviderLocalEpicsExportSharedSymbols
#   define epicsExportSharedSymbols
#	undef channelProviderLocalEpicsExportSharedSymbols
#endif

#include <shareLib.h>

#include <pv/pvDatabase.h>


namespace epics { namespace pvDatabase { 


class MonitorFactory;
typedef std::tr1::shared_ptr<MonitorFactory> MonitorFactoryPtr;

class MonitorLocal;
typedef std::tr1::shared_ptr<MonitorLocal> MonitorLocalPtr;


class ChannelProviderLocal;
typedef std::tr1::shared_ptr<ChannelProviderLocal> ChannelProviderLocalPtr;
class ChannelLocal;
typedef std::tr1::shared_ptr<ChannelLocal> ChannelLocalPtr;

epicsShareFunc MonitorFactoryPtr getMonitorFactory();

/**
 * @brief MonitorFactory
 *
 * This class provides a static method to create a monitor for a PVRecord
 */
class epicsShareClass MonitorFactory 
{
public:
    POINTER_DEFINITIONS(MonitorFactory);
    /**
     * Destructor
     */
    virtual ~MonitorFactory();
    /**
     * Destroy the monitor factory.
     */
    virtual void destroy();
    /** 
     * Create a monitor on a record.
     * This is called by the local channel provider.
     * @param pvRecord The record to monitor.
     * @param monitorRequester The client callback.
     * @param pvRequest Options specified by the client.
     * This includes the subset of the fields in the record to monitor.
     * @return A shared pointer to the newly created monitor.
     * If the monitor can not be created a null monitor is returned.
     * This means the pvRequest specified options that could not be satisfied.
     */
    epics::pvData::MonitorPtr createMonitor(
        PVRecordPtr const & pvRecord,
        epics::pvData::MonitorRequester::shared_pointer const & monitorRequester,
        epics::pvData::PVStructurePtr const & pvRequest);
private:
    MonitorFactory();
    friend class MonitorLocal;
    friend epicsShareFunc MonitorFactoryPtr getMonitorFactory();
    bool isDestroyed;
    epics::pvData::Mutex mutex;
};


epicsShareFunc ChannelProviderLocalPtr getChannelProviderLocal();

/**
 * @brief ChannelProvider for PVDatabase.
 *
 * An implementation of channelProvider that provides access to records in PVDatabase.
 */
class epicsShareClass ChannelProviderLocal :
    public epics::pvAccess::ChannelProvider,
    public std::tr1::enable_shared_from_this<ChannelProviderLocal>
{
public:
    POINTER_DEFINITIONS(ChannelProviderLocal);
    /**
     * Destructor
     */
    virtual ~ChannelProviderLocal();
    /**
     * Destroy the channel provider.
     * Probably never called.
     */
    virtual void destroy();
    /**
     * Returns the channel provider name.
     * @return <b>local</b>
     */
    virtual  std::string getProviderName();
    /**
     * Returns either a null channelFind or a channelFind for records in the PVDatabase.
     * @param channelName The name of the channel desired.
     * @param channelFindRequester The client callback.
     * @return shared pointer to ChannelFind.
     * This is null if the channelName is not the name of a record in the PVDatabase.
     * It is an implementation of SyncChannelFind if the channelName is the name
     * of a record in the PVDatabase.
     * The interface for SyncChannelFind is defined by pvAccessCPP.
     * The channelFindResult method of channelFindRequester is called before the
     * method returns.
     */
    virtual epics::pvAccess::ChannelFind::shared_pointer channelFind(
        std::string const &channelName,
        epics::pvAccess::ChannelFindRequester::shared_pointer const & channelFindRequester);
    /** 
     * Calls method channelListRequester::channelListResult which provides the
     * caller with a list of the record names on the PVDatabase.
     * A record name is the same as a channel name.
     * @param channelListRequester The client callback.
     * @return shared pointer to ChannelFind.
     * The interface for SyncChannelFind is defined by pvAccessCPP.
     */ 
    virtual epics::pvAccess::ChannelFind::shared_pointer channelList(
        epics::pvAccess::ChannelListRequester::shared_pointer const & channelListRequester);
    /**
     * Create a channel for a record.
     * This method just calls the next method with a address of "".
     * @param channelName The name of the channel desired.
     * @param channelRequester The client callback.
     * @param priority The priority.
     * @return shared pointer to Channel.
     */
    virtual epics::pvAccess::Channel::shared_pointer createChannel(
        std::string const &channelName,
        epics::pvAccess::ChannelRequester::shared_pointer const &channelRequester,
        short priority);
    /**
     * Create a channel for a record.
     * @param channelName The name of the channel desired.
     * @param channelRequester The callback to call with the result.
     * @param priority The priority.
     * This is ignored.
     * @param address The address.
     * This is ignored.
     * @return shared pointer to Channel.
     * This is null if the channelName is not the name of a record in the PVDatabase.
     * Otherwise it is a newly created channel inteface.
     * ChannelRequester::channelCreated is called to give the result.
     */
    virtual epics::pvAccess::Channel::shared_pointer createChannel(
        std::string const &channelName,
        epics::pvAccess::ChannelRequester::shared_pointer const &channelRequester,
        short priority,
        std::string const &address);
private:
    shared_pointer getPtrSelf()
    {
        return shared_from_this();
    }
    ChannelProviderLocal();
    friend epicsShareFunc ChannelProviderLocalPtr getChannelProviderLocal();
    PVDatabasePtr pvDatabase;
    epics::pvData::Mutex mutex;
    bool beingDestroyed;
    epics::pvAccess::ChannelFind::shared_pointer channelFinder;
    friend class ChannelProviderLocalRun;
};

/**
 * @brief Channel for accessing a PVRecord.
 *
 * A Channel for accessing a record in the PVDatabase.
 * It is a complete implementation of Channel
 */
class epicsShareClass ChannelLocal :
  public epics::pvAccess::Channel,
  public PVRecordClient,
  public std::tr1::enable_shared_from_this<ChannelLocal>
{
public:
    POINTER_DEFINITIONS(ChannelLocal);
    /** Constructor
     * @param channelProvider The channel provider.
     * @param requester The client callback.
     * @param pvRecord The record the channel will access.
     */
    ChannelLocal(
        ChannelProviderLocalPtr const &channelProvider,
        epics::pvAccess::ChannelRequester::shared_pointer const & requester,
        PVRecordPtr const & pvRecord
    );
    /** 
     * Destructor
     */
    virtual ~ChannelLocal();
    /** 
     * Destroy the channel.
     * It cleans up all resources used to access the record.
     * Note that this assumes that client has destroyed any objects that
     * have been created for the  channel like channelGet, etc.
     * The remote pvAccess server does this cleanup.
     */
    virtual void destroy();
    /** 
     * Get the requester name.
     * @return returns the name of the channel requester.
     */
    virtual std::string getRequesterName();
    /** 
     * Passes the message to the channel requester.
     * @param message The message.
     * @param messageType The message type.
     */
    virtual void message(
        std::string const & message,
        epics::pvData::MessageType messageType);
    /** 
     * Get the channel provider
     * @return The provider.
     */
    virtual epics::pvAccess::ChannelProvider::shared_pointer getProvider()
    {
        return provider;
    }
    /** 
     * Get the remote address
     * @return <b>local</b>
     */
    virtual std::string getRemoteAddress();
    /** 
     * Get the connection state.
     * @return Channel::CONNECTED.
     */
    virtual epics::pvAccess::Channel::ConnectionState getConnectionState();
    /** 
     * Get the channel name.
     * @return the record name.
     */
    virtual std::string getChannelName();
    /** 
     * Get the channel requester
     * @return The channel requester.
     */
    virtual epics::pvAccess::ChannelRequester::shared_pointer getChannelRequester();
    /** 
     * Is the channel connected?
     * @return true
     */
    virtual bool isConnected();
    /** 
     * Get the introspection interface for subField.
     * The introspection interface is given via GetFieldRequester::getDone.
     * @param requester The client callback.
     * @param subField The subField of the record.
     * If an empty string then the interface for the top level structure of
     * the record is provided.
     */
    virtual void getField(
        epics::pvAccess::GetFieldRequester::shared_pointer const &requester,
        std::string const & subField);
    /** 
     * Get the access rights for the record.
     * This throws an exception because it is assumed that access rights are
     * handled by a higher level.
     */
    virtual epics::pvAccess::AccessRights getAccessRights(
        epics::pvData::PVField::shared_pointer const &pvField);
    /** 
     * Create a channelProcess.
     * See pvAccess.html for details.
     * @param requester The client callback.
     * @param pvRequest The options specified by the client.
     * @return A shared pointer to the newly created implementation.
     * The implementation is null if pvRequest has invalid options.
     */
    virtual epics::pvAccess::ChannelProcess::shared_pointer createChannelProcess(
        epics::pvAccess::ChannelProcessRequester::shared_pointer const &requester,
        epics::pvData::PVStructurePtr const &pvRequest);
    /** 
     * Create a channelGet.
     * See pvAccess.html for details.
     * @param requester The client callback.
     * @param pvRequest The options specified by the client.
     * @return A shared pointer to the newly created implementation.
     * The implementation is null if pvRequest has invalid options.
     */
    virtual epics::pvAccess::ChannelGet::shared_pointer createChannelGet(
        epics::pvAccess::ChannelGetRequester::shared_pointer const &requester,
        epics::pvData::PVStructurePtr const &pvRequest);
    /** 
     * Create a channelPut.
     * See pvAccess.html for details.
     * @param requester The client callback.
     * @param pvRequest The options specified by the client.
     * @return A shared pointer to the newly created implementation.
     * The implementation is null if pvRequest has invalid options.
     */
    virtual epics::pvAccess::ChannelPut::shared_pointer createChannelPut(
        epics::pvAccess::ChannelPutRequester::shared_pointer const &requester,
        epics::pvData::PVStructurePtr const &pvRequest);
    /** 
     * Create a channelPutGet.
     * See pvAccess.html for details.
     * @param requester The client callback.
     * @param pvRequest The options specified by the client.
     * @return A shared pointer to the newly created implementation.
     * The implementation is null if pvRequest has invalid options.
     */
    virtual epics::pvAccess::ChannelPutGet::shared_pointer createChannelPutGet(
        epics::pvAccess::ChannelPutGetRequester::shared_pointer const &requester,
        epics::pvData::PVStructurePtr const &pvRequest);
    /** 
     * Create a channelRPC.
     * This is not implemented because pvAccessCPP implements channelRPC.
     * The server side of remote pvAccess implements a channel provider
     * just for channelRPC.
     * @param requester The client callback
     * @param pvRequest The options specified by the client.
     * @return null.
     */
    virtual epics::pvAccess::ChannelRPC::shared_pointer createChannelRPC(
        epics::pvAccess::ChannelRPCRequester::shared_pointer const &requester,
        epics::pvData::PVStructurePtr const &pvRequest);
    /** 
     * Create a monitor.
     * See pvAccess.html for details.
     * @param requester The client callback.
     * @param pvRequest The options specified by the client.
     * @return A shared pointer to the newly created implementation.
     * The implementation is null if pvRequest has invalid options.
     */
    virtual epics::pvData::Monitor::shared_pointer createMonitor(
        epics::pvData::MonitorRequester::shared_pointer const &requester,
        epics::pvData::PVStructurePtr const &pvRequest);
    /** 
     * Create a channelArray.
     * See pvAccess.html for details.
     * @param requester The client callback.
     * @param pvRequest The options specified by the client.
     * @return A shared pointer to the newly created implementation.
     * The implementation is null if pvRequest has invalid options.
     */
    virtual epics::pvAccess::ChannelArray::shared_pointer createChannelArray(
        epics::pvAccess::ChannelArrayRequester::shared_pointer const &requester,
        epics::pvData::PVStructurePtr const &pvRequest);
    /** 
     *  calls printInfo(std::cout);
     */
    virtual void printInfo();
    /** 
     * displays a message
     * "ChannelLocal provides access to a record in the local PVDatabase".
     * @param out the stream on which the message is displayed.
     */
    virtual void printInfo(std::ostream& out);
    /** 
     * This is called when a record is being removed from the database.
     * Calls destroy.
     * @param pvRecord The record being destroyed.
     */
    virtual void detach(PVRecordPtr const &pvRecord);
protected:
    shared_pointer getPtrSelf()
    {
        return shared_from_this();
    }
private:
    ChannelProviderLocalPtr provider;
    epics::pvAccess::ChannelRequester::weak_pointer requester;
    PVRecordPtr pvRecord;
    bool beingDestroyed;
    epics::pvData::Mutex mutex;
};

}}
#endif  /* CHANNELPROVIDERLOCAL_H */
