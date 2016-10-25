/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
#ifndef PVACLIENT_H
#define PVACLIENT_H

#ifdef epicsExportSharedSymbols
#   define pvaClientEpicsExportSharedSymbols
#   undef epicsExportSharedSymbols
#endif

#include <list>
#include <iostream>
#include <compilerDependencies.h>
#include <pv/requester.h>
#include <pv/status.h>
#include <pv/event.h>
#include <pv/lock.h>
#include <pv/pvData.h>
#include <pv/pvCopy.h>
#include <pv/pvTimeStamp.h>
#include <pv/timeStamp.h>
#include <pv/pvAlarm.h>
#include <pv/alarm.h>
#include <pv/pvAccess.h>
#include <pv/standardField.h>
#include <pv/standardPVField.h>
#include <pv/createRequest.h>
#include <pv/nt.h>

#ifdef pvaClientEpicsExportSharedSymbols
#   define epicsExportSharedSymbols
#	undef pvaClientEpicsExportSharedSymbols
#endif

#include <shareLib.h>


namespace epics { namespace pvaClient { 

class PvaClient;
typedef std::tr1::shared_ptr<PvaClient> PvaClientPtr;
class PvaClientGetData;
typedef std::tr1::shared_ptr<PvaClientGetData> PvaClientGetDataPtr;
class PvaClientPutData;
typedef std::tr1::shared_ptr<PvaClientPutData> PvaClientPutDataPtr;
class PvaClientMonitorData;
typedef std::tr1::shared_ptr<PvaClientMonitorData> PvaClientMonitorDataPtr;
class PvaClientChannelStateChangeRequester;
typedef std::tr1::shared_ptr<PvaClientChannelStateChangeRequester> PvaClientChannelStateChangeRequesterPtr;
typedef std::tr1::weak_ptr<PvaClientChannelStateChangeRequester> PvaClientChannelStateChangeRequesterWPtr;
class PvaClientChannel;
typedef std::tr1::shared_ptr<PvaClientChannel> PvaClientChannelPtr;
class PvaClientField;
typedef std::tr1::shared_ptr<PvaClientField> PvaClientFieldPtr;
class PvaClientProcess;
typedef std::tr1::shared_ptr<PvaClientProcess> PvaClientProcessPtr;
class PvaClientGet;
typedef std::tr1::shared_ptr<PvaClientGet> PvaClientGetPtr;
class PvaClientPut;
typedef std::tr1::shared_ptr<PvaClientPut> PvaClientPutPtr;
class PvaClientPutGet;
typedef std::tr1::shared_ptr<PvaClientPutGet> PvaClientPutGetPtr;
class PvaClientMonitor;
typedef std::tr1::shared_ptr<PvaClientMonitor> PvaClientMonitorPtr;
class PvaClientMonitorRequester;
typedef std::tr1::shared_ptr<PvaClientMonitorRequester> PvaClientMonitorRequesterPtr;
typedef std::tr1::weak_ptr<PvaClientMonitorRequester> PvaClientMonitorRequesterWPtr;
class PvaClientArray;
typedef std::tr1::shared_ptr<PvaClientArray> PvaClientArrayPtr;
class PvaClientRPC;
typedef std::tr1::shared_ptr<PvaClientRPC> PvaClientRPCPtr;
class PvaClientRPCRequester;
typedef std::tr1::shared_ptr<PvaClientRPCRequester> PvaClientRPCRequesterPtr;
typedef std::tr1::weak_ptr<PvaClientRPCRequester> PvaClientRPCRequesterWPtr;

// following are private to pvaClient
class PvaClientChannelCache;
typedef std::tr1::shared_ptr<PvaClientChannelCache> PvaClientChannelCachePtr;

/**
 * @brief  pvaClient is a synchronous wrapper for the pvAccess API, which is a callback based API.
 *
 * Thus it is easier to use than pvAccess itself.
 * In addition pvaClient provides many convenience methods.
 * @author mrk
 * @date 2015.02
 */
class epicsShareClass PvaClient :
     public epics::pvData::Requester,
     public std::tr1::enable_shared_from_this<PvaClient>
{
public:
    POINTER_DEFINITIONS(PvaClient);
    /**
     * @brief  Destructor
     */
    ~PvaClient();
    /** @brief Get the single instance of PvaClient.
     * @param providerNames Space separated list of provider names.
     * @return shared pointer to the single instance.
     */
    static PvaClientPtr get(std::string const & providerNames = "pva ca");
    /** @brief Create an instance of PvaClient with providerName "pva ca".
     * \deprecated This method will go away in future versions. Use get instead.
     * @return shared pointer to the single instance
     */
    static PvaClientPtr create() EPICS_DEPRECATED
    {
        return get();
    }
    /** @brief Get the requester name.
     * @return The name.
     */
    std::string getRequesterName();
    /** @brief A new message.
     *
     * If a requester is set then it is called otherwise message is displayed
     * on standard out.
     * @param message The message.
     * @param messageType The type.
     */
    void message(
        std::string const & message,
        epics::pvData::MessageType messageType);
    /** @brief Get a cached channel or create and connect to a new channel.
     * @param channelName The channelName.
     * @param providerName The providerName.
     * @param timeOut The number of seconds to wait for connection. 0.0 means forever.
     * @return The interface.
     * @throw runtime_error if connection fails.
     */
    PvaClientChannelPtr channel(
        std::string const & channelName,
        std::string const &providerName = "pva",
        double timeOut = 5.0);
    /** @brief Create an PvaClientChannel with the specified provider.
     * @param channelName The channelName.
     * @param providerName The provider.
     * @return The interface.
     * @throw runtime_error if connection fails.
     */
    PvaClientChannelPtr createChannel(
       std::string const & channelName,
       std::string const & providerName = "pva");
    
    /** @brief Set a requester.
     *
     * The default is for PvaClient to handle messages by printing to System.out.
     * @param requester The requester.
     */
    void setRequester(epics::pvData::RequesterPtr const & requester);
    /** @brief Clear the requester. PvaClient will handle messages.
     */
    void clearRequester();
    /** @brief Show the list of cached channels.
     */
    void showCache();
     /** @brief Get the number of cached channels.
     */
    size_t cacheSize();
    /** Should debug info be shown?
     * @param value true or false
     */
    static void setDebug(bool value) {debug = value;}
    /** @brief Is debug set?
     * @return true or false
     */
    static bool getDebug() {return debug;}

    /** @brief Deprecated method
     * \deprecated This method will go away in future versions. 
     */
    void destroy()  EPICS_DEPRECATED {}

private:
    static bool debug;
    PvaClient(std::string const & providerNames);
    PvaClientChannelCachePtr pvaClientChannelCache;
    epics::pvData::Requester::weak_pointer requester;
    bool pvaStarted;
    bool caStarted;
    epics::pvData::Mutex mutex;
};

// folowing private to PvaClientChannel
class PvaClientGetCache;
typedef std::tr1::shared_ptr<PvaClientGetCache> PvaClientGetCachePtr;
class PvaClientPutCache;
typedef std::tr1::shared_ptr<PvaClientPutCache> PvaClientPutCachePtr;

// NOTE: must use separate class that implements ChannelRequester,
// because pvAccess holds a shared_ptr to ChannelRequester instead of weak_pointer
class ChannelRequesterImpl;
typedef std::tr1::shared_ptr<ChannelRequesterImpl> ChannelRequesterImplPtr;

/** 
 * @brief A callback for change in connection status.
 *
 * @author mrk
 */
class PvaClientChannelStateChangeRequester
{
public:
    POINTER_DEFINITIONS(PvaClientChannelStateChangeRequester);
    /**
     * Destructor
     */
    virtual ~PvaClientChannelStateChangeRequester(){}
    /**
     * @brief A channel connection state change has occurred.
     * @param channel The channel.
     * @param isConnected The new connection status.
     */
    virtual void channelStateChange(PvaClientChannelPtr const & channel, bool isConnected) = 0;
};

/** 
 * @brief An easy to use alternative to directly calling the Channel methods of pvAccess.
 *
 * @author mrk
 */

class epicsShareClass PvaClientChannel :
   public std::tr1::enable_shared_from_this<PvaClientChannel>
{
public:
    POINTER_DEFINITIONS(PvaClientChannel);
    /**
     * @brief Destructor
     */
    ~PvaClientChannel();
    void setStateChangeRequester(PvaClientChannelStateChangeRequesterPtr const &stateChangeRequester);
    /** @brief Get the name of the channel to which PvaClientChannel is connected.
     * @return The channel name.
     */
    std::string getChannelName();
    /** @brief Get the the channel to which PvaClientChannel is connected.
     * @return The channel interface.
     */
    epics::pvAccess::Channel::shared_pointer getChannel();
    /** @brief Connect to the channel.
     *
     * This calls issueConnect and waitConnect.
     * @param timeout The time to wait for connecting to the channel. The defaut is 5 seconds.
     * @throw runtime_error if connection fails.
     */
    void connect(double timeout=5.0);
    /** @brief Issue a connect request and return immediately.
     */
    void issueConnect();
    /** @brief Wait until the connection completes or for timeout.
     * @param timeout The time in seconds to wait. A value of 0 means forever.
     * @return status.
     * @throw runtime_error if failure.
     */
    epics::pvData::Status waitConnect(double timeout = 5.0);
    /**  @brief Create a PvaClientField for the specified subField.
     * @param subField The desired subField, i. e. "field.field...."
     * An empty string, i. e. "", asks for the entire top level struture as defined by the server.
     * @return The interface.
     */
    PvaClientFieldPtr createField(std::string const & subField = "");
    /** @brief First call createRequest as implemented by pvDataCPP and then call the next method.
     * @param request The syntax of request is defined by the copy facility of pvData.
     * @return The interface.
     * @throw runtime_error if failure.
     */
    PvaClientProcessPtr createProcess(std::string const & request = "");
    /** Creates an PvaClientProcess. 
     * @param pvRequest The syntax of pvRequest is defined by the copy facility of pvData.
     * @return The interface.
     * @throw runtime_error if failure.
     */
    PvaClientProcessPtr createProcess(epics::pvData::PVStructurePtr const &  pvRequest);
    /** @brief Get a cached PvaClientGet or create and connect to a new PvaClientGet.
     *
     * If connection can not be made an exception is thrown.
     * @param request The syntax of request is defined by the copy facility of pvData.
     * @return The interface.
     * @throw runtime_error if failure.
     */
    PvaClientGetPtr get(std::string const & request = "field(value,alarm,timeStamp)");
    /** @brief create a PvaClientGet.
     *
     * First call createRequest as implemented by pvDataJava and then call the next method.
     * Then get a cached PvaClientGet or create and connect to a new PvaClientGet.
     * @param request The syntax of request is defined by the copy facility of pvData.
     * @return The interface.
     * @throw runtime_error if failure.
     */
    PvaClientGetPtr createGet(std::string const & request = "field(value,alarm,timeStamp)");
    /** @brief Creates an PvaClientGet.
     * @param pvRequest The syntax of pvRequest is defined by the copy facility of pvData.
     * @return The interface.
     * @throw runtime_error if failure.
     */
    PvaClientGetPtr createGet(epics::pvData::PVStructurePtr const &  pvRequest);
    /** @brief create a PvaClientPut.
     * 
     * First call createRequest as implemented by pvDataJava.
     * Then get a cached PvaClientPut or create and connect to a new PvaClientPut.
     * Then call it's get method.
     * @param request The syntax of request is defined by the copy facility of pvData.
     * @return The interface.
     * @throw runtime_error if connection fails
     */
    PvaClientPutPtr put(std::string const & request = "field(value)");
    /** @brief create a PvaClientPut.
     *
     * First call createRequest as implemented by pvDataJava and then call the next method.
     * @param request The syntax of request is defined by the copy facility of pvData.
     * @return The interface.
     * @throw runtime_error if failure.
     */
    PvaClientPutPtr createPut(std::string const & request = "field(value)");
    /** @brief Create a PvaClientPut.
     * @param pvRequest The syntax of pvRequest is defined by the copy facility of pvData.
     * @return The interface.
     */
    PvaClientPutPtr createPut(epics::pvData::PVStructurePtr const & pvRequest);
    /** @brief create a PvaClientPutGet.
     *
     * First call createRequest as implemented by pvDataJava and then calls the next method.
     * @param request The syntax of request is defined by the copy facility of pvData.
     * @return The interface.
     * @throw runtime_error if failure.
     */
    PvaClientPutGetPtr createPutGet(
         std::string const & request = "putField(argument)getField(result)");
    /** @brief Create a PvaClientPutGet.
     * @param pvRequest The syntax of pvRequest is defined by the copy facility of pvData.
     * @return The interface.
     */
    PvaClientPutGetPtr createPutGet(epics::pvData::PVStructurePtr const & pvRequest);
    /** @brief Create a PvaClientPutGet.
     *
     * First call createRequest as implemented by pvDataJava and then call the next method.
     * @param request The syntax of request is defined by the copy facility of pvData.
     * @return The interface.
     * @throw runtime_error if failure.
     */
    PvaClientArrayPtr createArray(std::string const & request = "field(value)");
    /** @brief Create a PvaClientArray.
     * @param pvRequest The syntax of pvRequest is defined by the copy facility of pvData.
     * @return The interface.
     * @throw runtime_error if failure.
     */
    PvaClientArrayPtr createArray(epics::pvData::PVStructurePtr const &  pvRequest);
    /** @brief Create a PvaClientMonitor.
     *
     * Create and connect to a new PvaClientMonitor.
     * Then call it's start method.
     * If connection can not be made an exception is thrown.
     * @param request The syntax of request is defined by the copy facility of pvData.
     * @return The interface.
     * @throw runtime_error if failure.
     */
    PvaClientMonitorPtr monitor(std::string const & request = "field(value,alarm,timeStamp)");
    /** @brief Call the next method with request =  "field(value,alarm,timeStamp)" 
      * @param pvaClientMonitorRequester The client callback.
      * @return The interface.
      * @throw runtime_error if failure.
      */
    PvaClientMonitorPtr monitor(PvaClientMonitorRequesterPtr const & pvaClientMonitorRequester);
    /** @brief Create and connect to a new PvaClientMonitor.
     *
     * Then call it's start method.
     * If connection can not be made an exception is thrown.
     * @param request The syntax of request is defined by the copy facility of pvData.
     * @param pvaClientMonitorRequester The client callback.
     * @return The interface.
     * @throw runtime_error if failure.
     */
    PvaClientMonitorPtr monitor(
        std::string const & request,
        PvaClientMonitorRequesterPtr const & pvaClientMonitorRequester);
    /**
     * @brief First call createRequest as implemented by pvDataJava and then calls the next method.
     * @param request The syntax of request is defined by the copy facility of pvData.
     * @return The interface.
     * @throw runtime_error if failure.
     */
    PvaClientMonitorPtr createMonitor(
         std::string const & request = "field(value,alarm,timeStamp)");
    /** Create an PvaClientMonitor.
     * @param pvRequest  The syntax of pvRequest is defined by the copy facility of pvData.
     * @return The interface.
     * @throw runtime_error if failure.
     */
    PvaClientMonitorPtr createMonitor(epics::pvData::PVStructurePtr const &  pvRequest);
    /** @brief Issue a channelRPC request
     * @param pvRequest  The pvRequest that is passed to createRPC.
     * @param pvArgument  The argument for a request.
     * @return The result.
     * @throw runtime_error if failure.
     */
     epics::pvData::PVStructurePtr rpc(
          epics::pvData::PVStructurePtr const &  pvRequest,
          epics::pvData::PVStructurePtr const & pvArgument);
    /** @brief Issue a channelRPC request
     * @param pvArgument  The argument for the request.
     * @return The result.
     * @throw runtime_error if failure.
     */
     epics::pvData::PVStructurePtr rpc(
          epics::pvData::PVStructurePtr const & pvArgument);
    /** @brief Create a PvaClientRPC.
     * @return The interface.
     * @throw runtime_error if failure.
     */
    PvaClientRPCPtr createRPC();
    /** @brief Create a PvaClientRPC.
     * @param pvRequest  The pvRequest that must have the same interface
     *  as a pvArgument that is passed to an rpc request.
     * @return The interface.
     * @throw runtime_error if failure.
     */
    PvaClientRPCPtr createRPC(epics::pvData::PVStructurePtr const &  pvRequest);
     /** @brief Show the list of cached gets and puts.
     */
    void showCache();
     /** @brief Get the number of cached gets and puts.
     */
    size_t cacheSize();
    /** Deprecated method
     * \deprecated This method will go away in future versions. 
     */
    void destroy()  EPICS_DEPRECATED {}
private:
    //ChannelRequester methods
    void channelCreated(
        const epics::pvData::Status& status,
        epics::pvAccess::Channel::shared_pointer const & channel);
    void channelStateChange(
        epics::pvAccess::Channel::shared_pointer const & channel,
        epics::pvAccess::Channel::ConnectionState connectionState);
    std::string getRequesterName();
    void message(
        std::string const & message,
        epics::pvData::MessageType messageType);

    static PvaClientChannelPtr create(
         PvaClientPtr const &pvaClient,
         std::string const & channelName,
         std::string const & providerName);
    PvaClientChannel(
        PvaClientPtr const &pvaClient,
        std::string const & channelName,
        std::string const & providerName);

    enum ConnectState {connectIdle,connectActive,notConnected,connected};

    PvaClient::weak_pointer pvaClient;
    std::string channelName;
    std::string providerName;
    ConnectState connectState;
    
    epics::pvData::CreateRequest::shared_pointer createRequest;
    PvaClientGetCachePtr pvaClientGetCache;
    PvaClientPutCachePtr pvaClientPutCache;

    epics::pvData::Mutex mutex;
    epics::pvData::Event waitForConnect;
    epics::pvAccess::Channel::shared_pointer channel;
    PvaClientChannelStateChangeRequesterWPtr stateChangeRequester;
    ChannelRequesterImplPtr channelRequester;
    friend class PvaClient;
    friend class ChannelRequesterImpl;

};

/** 
 *  @brief A class that holds data returned by PvaClientGet or PvaClientPutGet
 *
 */
class epicsShareClass PvaClientGetData
{
public:
    POINTER_DEFINITIONS(PvaClientGetData);
    /**
     * @brief Destructor
     */
    ~PvaClientGetData() {}
    /** @brief Set a prefix for throw messages.
     *
     * This is called by other pvaClient classes.
     * @param value The prefix.
     */
    void setMessagePrefix(std::string const & value);
   /** @brief Get the structure.
    * @return The Structure
    * @throw runtime_error if failure.
    */
   epics::pvData::StructureConstPtr getStructure();
   /** @brief Get the pvStructure.
    * @return the pvStructure.
    * @throw runtime_error if failure.
    */
   epics::pvData::PVStructurePtr getPVStructure();
   /** @brief Get the changed BitSet for the pvStructure
    *
    * This shows which fields have changed value since the last get.
    * @return The bitSet
    * @throw runtime_error if failure
    */
   epics::pvData::BitSetPtr getChangedBitSet();
   /** @brief Show the fields that have changed value since the last get.
    * @param out The stream that shows the changed fields.
    * @return The stream that was passed as out.
    */
   std::ostream & showChanged(std::ostream & out);
    /** @brief New data is present.
     *
     * This is called by other pvaClient classes, i. e. not by client.
     * @param pvStructureFrom The new data.
     * @param bitSetFrom the bitSet showing which values have changed.
     */
    void setData(
        epics::pvData::PVStructurePtr const & pvStructureFrom,
        epics::pvData::BitSetPtr const & bitSetFrom);
    /** @brief Is there a top level field named value.
     * @return The answer.
     */
    bool hasValue();
    /** @brief Is the value field a scalar?
     * @return The answer.
     */
    bool isValueScalar();
    /** @brief Is the value field a scalar array?
     * @return The answer.
     */
    bool isValueScalarArray();
    /** @brief Get the interface to the value field.
     * @return The interface.
     * @throw runtime_error if failure.
     */
    epics::pvData::PVFieldPtr getValue();
    /**
     * @brief Return the interface to a scalar value field.
     * @return The interface for a scalar value field.
     * @throw runtime_error if failure.
     */
    epics::pvData::PVScalarPtr getScalarValue();
    /** @brief  Get the interface to an array value field.
     * @return The interface.
     * @throw runtime_error if failure.
     */
    std::tr1::shared_ptr<epics::pvData::PVArray> getArrayValue();
    /** @brief Get the interface to a scalar array value field.
     * @return Return the interface.
     * @throw runtime_error if failure.
     */
    std::tr1::shared_ptr<epics::pvData::PVScalarArray> getScalarArrayValue();
    /** @brief Get the value as a double.
     * @return The value.
     * @throw runtime_error if failure.
     */
    double getDouble();
    /** Get the value as a string.
     * @return The value.
     * @throw runtime_error if failure.
     */
    std::string getString();
    /** @brief Get the value as a double array.
     * @return The value.
     * @throw runtime_error if failure.
     */
    epics::pvData::shared_vector<const double>  getDoubleArray();
    /** @brief Get the value as a string array.
     * @return The value.
     * @throw runtime_error if failure.
     */
    epics::pvData::shared_vector<const std::string>  getStringArray();
    /** @brief Get the alarm.
     * If the pvStructure has an alarm field it's values are returned.
     * Otherwise an exception is thrown.
     * @return The alarm.
     * @throw runtime_error if failure.
     */
    epics::pvData::Alarm getAlarm();
    /** @brief Get the timeStamp.
     * If the pvStructure has a timeStamp field, it's values are returned.
     * Otherwise an exception is thrown.
     * @return The timeStamp.
     */
    epics::pvData::TimeStamp getTimeStamp();
    /** @brief Factory method for creating an instance of PvaClientGetData.
     *
     * NOTE: Not normally called by clients
     * @param structure Introspection interface
     * @throw runtime_error if failure.
     */
    static PvaClientGetDataPtr create(epics::pvData::StructureConstPtr const & structure);
private:
    PvaClientGetData(epics::pvData::StructureConstPtr const & structure);
    void checkValue();
    epics::pvData::StructureConstPtr structure;
    epics::pvData::PVStructurePtr pvStructure;
    epics::pvData::BitSetPtr bitSet;

    std::string messagePrefix;
    epics::pvData::PVFieldPtr pvValue;
    epics::pvData::PVAlarm pvAlarm;
    epics::pvData::PVTimeStamp pvTimeStamp;
    friend class PvaClientGet;
    friend class PvaClientPutGet;
};

class PvaClientPostHandlerPvt; // private to PvaClientPutData
/** 
 *  @brief A class that holds data given to  by PvaClientPut or PvaClientPutGet
 *
 */
class epicsShareClass PvaClientPutData 
{
public:
    POINTER_DEFINITIONS(PvaClientPutData);
    /**
     * @brief Destructor
     */    
    ~PvaClientPutData() {}
    /** @brief Set a prefix for throw messages.
     * @param value The prefix.
     */
    void setMessagePrefix(std::string const & value);
    /** @brief Get the structure.
    * @return The Structure
    * @throw runtime_error if failure.
    */
   epics::pvData::StructureConstPtr getStructure();
   /** @brief Get the pvStructure.
    * @return the pvStructure.
    * @throw runtime_error if failure.
    */
   epics::pvData::PVStructurePtr getPVStructure();
    /**  @brief Get the changed BitSet for the pvStructure
     * This shows which fields have changed values.
     * @return The bitSet
     * @throw runtime_error if failure.
     */
    epics::pvData::BitSetPtr getChangedBitSet();
    /**  @brief Show the fields that have changed values.
     * @param out The stream that shows the changed fields.
     * @return The stream that was passed as out.
     */
    std::ostream & showChanged(std::ostream & out);
    /**
     * @brief Is there a top level field named value.
     * @return The answer.
     */
    bool hasValue();
    /** @brief Is the value field a scalar?
     * @return The answer.
     */
    bool isValueScalar();
    /** @brief Is the value field a scalar array?
     * @return The answer.
     */
    bool isValueScalarArray();
    /** Get the interface to the value field.
     * @return The interface. an excetion is thrown if a value field does not exist.
     */
    epics::pvData::PVFieldPtr getValue();
    /** @brief Get the interface to a scalar value field.
     * @return The interface for a scalar value field.
     * @throw runtime_error if failure.
     */
    epics::pvData::PVScalarPtr getScalarValue();
    /** @brief Get the interface to an array value field.
     * @return The interface.
     * An exception is thown if no array value field.
     */
    std::tr1::shared_ptr<epics::pvData::PVArray> getArrayValue();
    /** @brief Get the interface to a scalar array value field.
     * @return Return the interface.
     * @throw runtime_error if failure.
     */
    std::tr1::shared_ptr<epics::pvData::PVScalarArray> getScalarArrayValue();
    /** @brief Get the value as a double.
     *
     * If value is not a numeric scalar an exception is thrown.
     * @return The value.
     */
    double getDouble();
    /** @brief Get the value as a string.
     * @return The value.
     * @throw runtime_error if failure.
     */
    std::string getString();
    /** @brief Get the value as a double array.
     * If the value is not a numeric array an exception is thrown.
     * @return The value.
     */
    epics::pvData::shared_vector<const double>  getDoubleArray();
    /** @brief Get the value as a string array.
     * @return The value.
     * @throw runtime_error if failure.
     */
    epics::pvData::shared_vector<const std::string>  getStringArray();
    /** @brief Put the value as a double.
     * @param value The new value.
     * An exception is also thrown if the actualy type can cause an overflow.
     * @throw runtime_error if failure.
     */
    void putDouble(double value);
    /** @brief Put the value as a string.
     *
     * If value is not a  scalar an exception is thrown.
     */
    void putString(std::string const & value);
    /** @brief Copy the array to the value field.
     * @param value The place where data is copied.
     * @throw runtime_error if failure.
     */
    void putDoubleArray(epics::pvData::shared_vector<const double> const & value);
    /** @brief Copy array to the value field.
     * @param value data source
     * @throw runtime_error if failure.
     */
    void putStringArray(epics::pvData::shared_vector<const std::string> const & value);
    /** @brief Copy array to the value field.
     * @param value data source
     * @throw runtime_error if failure.
     */
    void putStringArray(std::vector<std::string> const & value);
    /** Factory method for creating an instance of PvaClientGetData.
     * NOTE: Not normally called by clients
     * @param structure Introspection interface
     */
     static PvaClientPutDataPtr create(epics::pvData::StructureConstPtr const & structure);
private:
    PvaClientPutData(epics::pvData::StructureConstPtr const &structure);
    void checkValue();
    void postPut(size_t fieldNumber);

    std::vector<epics::pvData::PostHandlerPtr> postHandler;
    epics::pvData::StructureConstPtr structure;
    epics::pvData::PVStructurePtr pvStructure;
    epics::pvData::BitSetPtr bitSet;
    friend class PvaClientPostHandlerPvt;

    std::string messagePrefix;
    epics::pvData::PVFieldPtr pvValue;
    friend class PvaClientPut;
    friend class PvaClientPutGet;
};

/**
 * @brief A class that holds data returned by PvaClientMonitor
 *
 */
class epicsShareClass PvaClientMonitorData
{
public:
    POINTER_DEFINITIONS(PvaClientMonitorData);
    /**
     * @brief Destructor
     */    
    ~PvaClientMonitorData() {}
    /** @brief Set a prefix for throw messages.
     * @param value The prefix.
     */
    void setMessagePrefix(std::string const & value);
    /** @brief Get the structure.
     * @return The Structure
     * @throw runtime_error if failure.
     */
    epics::pvData::StructureConstPtr getStructure();
    /** @brief Get the pvStructure.
     * @return the pvStructure.
     * @throw runtime_error if failure.
     */
    epics::pvData::PVStructurePtr getPVStructure();
    /**  @brief Get the changed BitSet for the pvStructure,
     *
     * This shows which fields have changed value.
     * @return The bitSet
     * @throw runtime_error if failure.
     */
    epics::pvData::BitSetPtr getChangedBitSet();
    /** @brief Get the overrun BitSet for the pvStructure
     * This shows which fields have had more than one change.
     * @return The bitSet
     * @throw runtime_error if failure.
     */
    epics::pvData::BitSetPtr getOverrunBitSet();
    /** @brief Show the fields that have changed.
     * @param out The stream that shows the changed fields.
     * @return The stream that was passed as out.
     */
    std::ostream & showChanged(std::ostream & out);
    /** @brief Show the fields that have overrun.
     * @param out The stream that shows the overrun fields.
     * @return The stream that was  passed as out
     */
    std::ostream & showOverrun(std::ostream & out);
    /** @brief Is there a top level field named value.
     * @return The answer.
     */
    bool hasValue();
    /** @brief Is the value field a scalar?
     * @return The answer.
     */
    bool isValueScalar();
    /** @brief Is the value field a scalar array?
     * @return The answer.
     */
    bool isValueScalarArray();
    /** @brief Get the interface to the value field.
     * @return The interface. an excetion is thrown if a value field does not exist.
     */
    epics::pvData::PVFieldPtr getValue();
    /** @brief Get the interface to a scalar value field.
     * @return The interface for a scalar value field.
     * @throw runtime_error if failure.
     * An exception is thown if no scalar value field.
     */
    epics::pvData::PVScalarPtr getScalarValue();
    /** @brief Get the interface to an array value field.
     * @return The interface.
     * @throw runtime_error if failure.
     * An exception is thown if no array value field.
     */
    std::tr1::shared_ptr<epics::pvData::PVArray> getArrayValue();
    /** @brief Get the interface to a scalar array value field.
     * @return Return the interface.
     * @throw runtime_error if failure.
     * An exception is thown if no scalar array value field.
     */
    std::tr1::shared_ptr<epics::pvData::PVScalarArray> getScalarArrayValue();
    /** @brief Get the value as a double.
     *
     * If value is not a numeric scalar an exception is thrown.
     * @return The value.
     */
    double getDouble();
    /** @brief Get the value as a string.
     *
     * If value is not a scalar an exception is thrown
     * @return The value.
     * @throw runtime_error if failure.
     */
    std::string getString();
    /** @brief Get the value as a double array.
     *
     * If the value is not a numeric array an exception is thrown.
     * @return The value.
     * @throw runtime_error if failure.
     */
    epics::pvData::shared_vector<const double>  getDoubleArray();
    /** @brief Get the value as a string array.
     *
     * If the value is not a string array an exception is thrown.
     * @return The value.
     * @throw runtime_error if failure.
     */
    epics::pvData::shared_vector<const std::string>  getStringArray();
    /** @brief Get the alarm.
     *
     * If the pvStructure as an alarm field it's values are returned.
     * If no then alarm shows that not alarm defined.
     * @return The alarm.
     * @throw runtime_error if failure.
     */
    epics::pvData::Alarm getAlarm();
    /** @brief Get the timeStamp.
     *
     * If the pvStructure has a timeStamp field, it's values are returned.
     * If no then all fields are 0.
     * @return The timeStamp.
     */
    epics::pvData::TimeStamp getTimeStamp();
    /** Factory method for creating an instance of PvaClientGetData.
     * NOTE: Not normally called by clients
     * @param structure Introspection interface
     */
     static PvaClientMonitorDataPtr create(epics::pvData::StructureConstPtr const & structure);
     /** Put data into PVStructure from monitorElement
      *  NOTE: Not normally called by clients
      * @param monitorElement the monitorElement that has new data.
      */
     void setData(epics::pvData::MonitorElementPtr const & monitorElement);
private:
    PvaClientMonitorData(epics::pvData::StructureConstPtr const & structure);
    void checkValue();
    
    epics::pvData::StructureConstPtr structure;
    epics::pvData::PVStructurePtr pvStructure;
    epics::pvData::BitSetPtr changedBitSet;
    epics::pvData::BitSetPtr overrunBitSet;

    std::string messagePrefix;
    epics::pvData::PVFieldPtr pvValue;
    epics::pvData::PVAlarm pvAlarm;
    epics::pvData::PVTimeStamp pvTimeStamp;
    friend class PvaClientMonitor;
};


// NOTE: must use separate class that implements ChannelProcessRequester,
// because pvAccess holds a shared_ptr to ChannelProcessRequester instead of weak_pointer
class ChannelProcessRequesterImpl;
typedef std::tr1::shared_ptr<ChannelProcessRequesterImpl> ChannelProcessRequesterImplPtr;

/**
 * @brief An easy to use alternative to ChannelProcess.
 *
 * @author mrk
 */
class epicsShareClass PvaClientProcess 
{
public:
    POINTER_DEFINITIONS(PvaClientProcess);
    /** @brief  Create a PvaClientProcess.
     * @param &pvaClient Interface to PvaClient
     * @param channel Interface to Channel
     * @param pvRequest The request structure.
     * @return The interface to the PvaClientProcess.
     */
    static PvaClientProcessPtr create(
        PvaClientPtr const &pvaClient,
        epics::pvAccess::Channel::shared_pointer const & channel,
        epics::pvData::PVStructurePtr const &pvRequest
    );
    /** @brief Destructor
     */
    ~PvaClientProcess();
    /** @brief Call issueConnect and then waitConnect.
     *
     * An exception is thrown if connect fails.
     * @throw runtime_error if failure.
     */
    void connect();
    /** @brief Issue the channelProcess connection to the channel.
     *
     * This can only be called once.
     */
    void issueConnect();
    /** @brief Wait until the channelProcess connection to the channel is complete.
     * @return status;
     */
    epics::pvData::Status waitConnect();
    /** @brief Call issueProcess and then waitProcess.
     *
     * An exception is thrown if get fails.
     */
    void process();
    /** @brief Issue a process and return immediately.
     */
    void issueProcess();
    /** @brief Wait until process completes.
     * @return status.
     */
    epics::pvData::Status waitProcess();
     /** Deprecated method
     * \deprecated This method will go away in future versions. 
     */
    void destroy()  EPICS_DEPRECATED {}
private:
    std::string getRequesterName();
    void message(std::string const & message,epics::pvData::MessageType messageType);
    void channelProcessConnect(
        const epics::pvData::Status& status,
        epics::pvAccess::ChannelProcess::shared_pointer const & channelProcess);
    void processDone(
        const epics::pvData::Status& status,
        epics::pvAccess::ChannelProcess::shared_pointer const & channelProcess);

    PvaClientProcess(
        PvaClientPtr const &pvaClient,
        epics::pvAccess::Channel::shared_pointer const & channel,
        epics::pvData::PVStructurePtr const &pvRequest);
    
    enum ProcessConnectState {connectIdle,connectActive,connected};

    PvaClient::weak_pointer pvaClient;
    epics::pvAccess::Channel::shared_pointer channel;
    epics::pvData::PVStructurePtr pvRequest;
    epics::pvData::Mutex mutex;
    epics::pvData::Event waitForConnect;
    epics::pvData::Event waitForProcess;

    epics::pvData::Status channelProcessConnectStatus;
    epics::pvData::Status channelProcessStatus;
    epics::pvAccess::ChannelProcess::shared_pointer channelProcess;

    ProcessConnectState connectState;

    enum ProcessState {processIdle,processActive,processComplete};
    ProcessState processState;
    ChannelProcessRequesterImplPtr channelProcessRequester;
    friend class ChannelProcessRequesterImpl;
};

// NOTE: must use separate class that implements ChannelGetRequester,
// because pvAccess holds a shared_ptr to ChannelGetRequester instead of weak_pointer
class ChannelGetRequesterImpl;
typedef std::tr1::shared_ptr<ChannelGetRequesterImpl> ChannelGetRequesterImplPtr;
/**
 * @brief An easy to use alternative to ChannelGet.
 *
 * @author mrk
 */
class epicsShareClass PvaClientGet 
{
public:
    POINTER_DEFINITIONS(PvaClientGet);
    /** @brief Create a PvaClientGet.
     * @param &pvaClient Interface to PvaClient
     * @param channel Interface to Channel
     * @param pvRequest The request structure.
     * @return The interface to the PvaClientGet.
     */
    static PvaClientGetPtr create(
        PvaClientPtr const &pvaClient,
        epics::pvAccess::Channel::shared_pointer const & channel,
        epics::pvData::PVStructurePtr const &pvRequest
    );
    /** @brief Destructor
     */
    ~PvaClientGet();
    /** @brief Call issueConnect and then waitConnect.
     *
     * An exception is thrown if connect fails.
     * @throw runtime_error if failure.
     */
    void connect();
    /**
     * @brief create the channelGet connection to the channel.
     *
     * This can only be called once.
     */
    void issueConnect();
    /** @brief Wait until the channelGet connection to the channel is complete.
     * @return status;
     */
    epics::pvData::Status waitConnect();
    /** @brief Call issueGet and then waitGet.
     * An exception is thrown if get fails.
     */
    void get();
    /** @brief Issue a get and return immediately.
     */
    void issueGet();
    /** @brief Wait until get completes.
     * @return status;
     */
    epics::pvData::Status waitGet();
    /**
     * @brief Get the data/
     * @return The interface.
     */
    PvaClientGetDataPtr getData();
    /** Deprecated method
     * \deprecated This method will go away in future versions. 
     */
    void destroy()  EPICS_DEPRECATED {}
private:
        std::string getRequesterName();
    void message(std::string const & message,epics::pvData::MessageType messageType);
    void channelGetConnect(
        const epics::pvData::Status& status,
        epics::pvAccess::ChannelGet::shared_pointer const & channelGet,
        epics::pvData::StructureConstPtr const & structure);
    void getDone(
        const epics::pvData::Status& status,
        epics::pvAccess::ChannelGet::shared_pointer const & channelGet,
        epics::pvData::PVStructurePtr const & pvStructure,
        epics::pvData::BitSetPtr const & bitSet);

    PvaClientGet(
        PvaClientPtr const &pvaClient,
        epics::pvAccess::Channel::shared_pointer const & channel,
        epics::pvData::PVStructurePtr const &pvRequest);

    void checkGetState();
    enum GetConnectState {connectIdle,connectActive,connected};

    PvaClient::weak_pointer pvaClient;
    epics::pvAccess::Channel::weak_pointer channel;
    epics::pvData::PVStructurePtr pvRequest;
    epics::pvData::Mutex mutex;
    epics::pvData::Event waitForConnect;
    epics::pvData::Event waitForGet;
    PvaClientGetDataPtr pvaClientData;
    std::string messagePrefix;

    epics::pvData::Status channelGetConnectStatus;
    epics::pvData::Status channelGetStatus;
    epics::pvAccess::ChannelGet::shared_pointer channelGet;

    GetConnectState connectState;

    enum GetState {getIdle,getActive,getComplete};
    GetState getState;
    ChannelGetRequesterImplPtr channelGetRequester;
    friend class ChannelGetRequesterImpl;
};


// NOTE: must use separate class that implements ChannelPutRequester,
// because pvAccess holds a shared_ptr to ChannelPutRequester instead of weak_pointer
class ChannelPutRequesterImpl;
typedef std::tr1::shared_ptr<ChannelPutRequesterImpl> ChannelPutRequesterImplPtr;

/**
 * @brief An easy to use alternative to ChannelPut.
 *
 * @author mrk
 */
class epicsShareClass PvaClientPut 
{
public:
    POINTER_DEFINITIONS(PvaClientPut);
    /** @brief Create a PvaClientPut.
     * @param &pvaClient Interface to PvaClient
     * @param channel Interface to Channel
     * @param pvRequest The request structure.
     * @return The interface to the PvaClientPut.
     */
    static PvaClientPutPtr create(
        PvaClientPtr const &pvaClient,
        epics::pvAccess::Channel::shared_pointer const & channel,
        epics::pvData::PVStructurePtr const &pvRequest
    );
    /** @brief Destructor
     */
    ~PvaClientPut();

    /** @brief Call issueConnect and then waitConnect.
     *
     * An exception is thrown if connect fails.
     * @throw runtime_error if failure.
     */
    void connect();
    /** @brief Issue the channelPut connection to the channel.
     *
     * This can only be called once.
     */
    void issueConnect();
    /** @brief Wait until the channelPut connection to the channel is complete.
     * @return status;
     */
    epics::pvData::Status waitConnect();
    /** @brief Call issueGet and then waitGet.
     *
     * An exception is thrown if get fails.
     * @throw runtime_error if failure.
     */
    void get();
    /** @brief Issue a get and return immediately.
     */
    void issueGet();
    /** @brief Wait until get completes.
     * @return status
     */
    epics::pvData::Status waitGet();
    /** @brief Call issuePut and then waitPut.
     * An exception is thrown if get fails.
     */
    void put();
    /** @brief Issue a put and return immediately.
     */
    void issuePut();
    /** @brief Wait until put completes.
     * @return status
     */
    epics::pvData::Status waitPut();
    /**
     * @brief Get the data/
     * @return The interface.
     */
    PvaClientPutDataPtr getData();
     /** Deprecated method
     * \deprecated This method will go away in future versions. 
     */
    void destroy()  EPICS_DEPRECATED {}   
private :
    std::string getRequesterName();
    void message(std::string const & message,epics::pvData::MessageType messageType);
    void channelPutConnect(
        const epics::pvData::Status& status,
        epics::pvAccess::ChannelPut::shared_pointer const & channelPut,
        epics::pvData::StructureConstPtr const & structure);
    void getDone(
        const epics::pvData::Status& status,
        epics::pvAccess::ChannelPut::shared_pointer const & channelPut,
        epics::pvData::PVStructurePtr const & pvStructure,
        epics::pvData::BitSetPtr const & bitSet);
    void putDone(
        const epics::pvData::Status& status,
        epics::pvAccess::ChannelPut::shared_pointer const & channelPut);
    PvaClientPut(
        PvaClientPtr const &pvaClient,
        epics::pvAccess::Channel::shared_pointer const & channel,
        epics::pvData::PVStructurePtr const &pvRequest);
    
    void checkPutState();
    enum PutConnectState {connectIdle,connectActive,connected};

    PvaClient::weak_pointer pvaClient;
    epics::pvAccess::Channel::weak_pointer channel;
    epics::pvData::PVStructurePtr pvRequest;
    epics::pvData::Mutex mutex;
    epics::pvData::Event waitForConnect;
    epics::pvData::Event waitForGetPut;
    PvaClientPutDataPtr pvaClientData;

    epics::pvData::Status channelPutConnectStatus;
    epics::pvData::Status channelGetPutStatus;
    epics::pvAccess::ChannelPut::shared_pointer channelPut;
    PutConnectState connectState;

    enum PutState {putIdle,getActive,putActive};
    PutState putState;
    ChannelPutRequesterImplPtr channelPutRequester;
    friend class ChannelPutRequesterImpl;
};

// NOTE: must use separate class that implements ChannelPutGetRequester,
// because pvAccess holds a shared_ptr to ChannelPutGetRequester instead of weak_pointer
class ChannelPutGetRequesterImpl;
typedef std::tr1::shared_ptr<ChannelPutGetRequesterImpl> ChannelPutGetRequesterImplPtr;

/** 
 * @brief An easy to use alternative to ChannelPutGet.
 *
 * @author mrk
 */
class epicsShareClass PvaClientPutGet 
{
public:
    POINTER_DEFINITIONS(PvaClientPutGet);
    /** @brief Create a PvaClientPutGet.
     * @param &pvaClient Interface to PvaClient
     * @param channel Interface to Channel
     * @param pvRequest The request structure.
     * @return The interface to the PvaClientPutGet.
     */
    static PvaClientPutGetPtr create(
        PvaClientPtr const &pvaClient,
        epics::pvAccess::Channel::shared_pointer const & channel,
        epics::pvData::PVStructurePtr const &pvRequest
    );
    /** @brief Destructor
     */
    ~PvaClientPutGet();

    /** @brief Call issueConnect and then waitConnect.
     *
     * An exception is thrown if connect fails.
     * @throw runtime_error if failure.
     */
    void connect();
    /** @brief Issue the channelPutGet connection to the channel.
     *
     * This can only be called once.
     * An exception is thrown if connect fails.
     */
    void issueConnect();
    /** @brief Wait until the channelPutGet connection to the channel is complete.
     * @return status;
     */
    epics::pvData::Status waitConnect();
    /** @brief Call issuePutGet and then waitPutGet.
     *
     * An exception is thrown if putGet fails.
     */
    void putGet();
    /** @brief Issue a putGet and return immediately.
     */
    void issuePutGet();
    /** @brief Wait until putGet completes.
     *
     * If failure getStatus can be called to get reason.
     * @return status
     */
    epics::pvData::Status waitPutGet();
    /** @brief Call issueGet and then waitGetGet.
     * An exception is thrown if get fails.
     */
    void getGet();
    /** @brief Issue a getGet and return immediately.
     */
    void issueGetGet();
    /** @brief Wait until getGet completes.
     *
     * If failure getStatus can be called to get reason.
     * @return status
     */
    epics::pvData::Status waitGetGet();
    /** @brief Call issuePut and then waitGetPut.
     *
     * An exception is thrown if getPut fails.
     */
    void getPut();
    /** @brief Issue a getPut and return immediately.
     */
    void issueGetPut();
    /** @brief Wait until getPut completes.
     * @return status
     */
    epics::pvData::Status waitGetPut();
    /** @brief Get the put data.
     * @return The interface.
     */
    PvaClientPutDataPtr getPutData();   
    /** @brief Get the get data.
     * @return The interface.
     */
    PvaClientGetDataPtr getGetData(); 
    /** Deprecated method
     * \deprecated This method will go away in future versions. 
     */
    void destroy()  EPICS_DEPRECATED {}  
private :
    std::string getRequesterName();
    void message(std::string const & message,epics::pvData::MessageType messageType);
    void channelPutGetConnect(
        const epics::pvData::Status& status,
        epics::pvAccess::ChannelPutGet::shared_pointer const & channelPutGet,
        epics::pvData::StructureConstPtr const & putStructure,
        epics::pvData::StructureConstPtr const & getStructure);
    void putGetDone(
        const epics::pvData::Status& status,
        epics::pvAccess::ChannelPutGet::shared_pointer const & channelPutGet,
        epics::pvData::PVStructurePtr const & getPVStructure,
        epics::pvData::BitSetPtr const & getBitSet);
    void getPutDone(
        const epics::pvData::Status& status,
        epics::pvAccess::ChannelPutGet::shared_pointer const & channelPutGet,
        epics::pvData::PVStructurePtr const & putPVStructure,
        epics::pvData::BitSet::shared_pointer const & putBitSet);
    void getGetDone(
        const epics::pvData::Status& status,
        epics::pvAccess::ChannelPutGet::shared_pointer const & channelPutGet,
        epics::pvData::PVStructurePtr const & getPVStructure,
        epics::pvData::BitSet::shared_pointer const & getBitSet);

    PvaClientPutGet(
        PvaClientPtr const &pvaClient,
        epics::pvAccess::Channel::shared_pointer const & channel,
        epics::pvData::PVStructurePtr const &pvRequest);
    void checkPutGetState();
    enum PutGetConnectState {connectIdle,connectActive,connected};

    PvaClient::weak_pointer pvaClient;
    epics::pvAccess::Channel::weak_pointer channel;
    epics::pvData::PVStructurePtr pvRequest;
    epics::pvData::Mutex mutex;
    epics::pvData::Event waitForConnect;
    epics::pvData::Event waitForPutGet;
    PvaClientGetDataPtr pvaClientGetData;
    PvaClientPutDataPtr pvaClientPutData;

    epics::pvData::Status channelPutGetConnectStatus;
    epics::pvData::Status channelPutGetStatus;
    epics::pvAccess::ChannelPutGet::shared_pointer channelPutGet;
    PutGetConnectState connectState;

    enum PutGetState {putGetIdle,putGetActive,putGetComplete};
    PutGetState putGetState;
    ChannelPutGetRequesterImplPtr channelPutGetRequester;
    friend class ChannelPutGetRequesterImpl;
};

//class ChannelMonitorRequester; // private to PvaClientMonitor
/**
 * @brief Optional client callback.
 *
 */
class PvaClientMonitorRequester
{
public:
    POINTER_DEFINITIONS(PvaClientMonitorRequester);
    virtual ~PvaClientMonitorRequester() {}
    /** @brief A monitor event has occurred.
     * @param monitor The PvaClientMonitor that received the event.
     */
    virtual void event(PvaClientMonitorPtr const & monitor) = 0;
    /**
     * @brief The data source is no longer available.
     */

    virtual void unlisten()
    {
         std::cerr << "PvaClientMonitorRequester::unlisten called"
                 << " but no PvaClientMonitorRequester::unlisten\n";
    }
};


// NOTE: must use separate class that implements MonitorRequester,
// because pvAccess holds a shared_ptr to MonitorRequester instead of weak_pointer
class MonitorRequesterImpl;
typedef std::tr1::shared_ptr<MonitorRequesterImpl> MonitorRequesterImplPtr;

/**
 * @brief An easy to use alternative to Monitor.
 *
 */
class epicsShareClass PvaClientMonitor :
    public std::tr1::enable_shared_from_this<PvaClientMonitor>
{
public:
    POINTER_DEFINITIONS(PvaClientMonitor);
    /** @brief Create a PvaClientMonitor.
     * @param &pvaClient Interface to PvaClient
     * @param channel Interface to Channel
     * @param pvRequest The request structure.
     * @return The interface to the PvaClientMonitor.
     */
    static PvaClientMonitorPtr create(
        PvaClientPtr const &pvaClient,
        epics::pvAccess::Channel::shared_pointer const & channel,
        epics::pvData::PVStructurePtr const &pvRequest
    );
    /** @brief Destructor
     */
    ~PvaClientMonitor();
    /** @brief Call issueConnect and then waitConnect.
     *
     * An exception is thrown if connect fails.
     */
    void connect();
    /** @brief Issue the channelMonitor connection to the channel.
     *
     * This can only be called once.
     * An exception is thrown if connect fails.
     * @throw runtime_error if failure.
     */
    void issueConnect();
    /** @brief Wait until the channelMonitor connection to the channel is complete.
     * @return status;
     */
    epics::pvData::Status waitConnect();
    /** @brief Set a user callback.
     * @param pvaClientMonitorRequester The requester which must be implemented by the caller.
     */
    void setRequester(PvaClientMonitorRequesterPtr const & pvaClientMonitorRequester);
    /** @brief Start monitoring.
     */
    void start();
    /** @brief Stop monitoring.
     */
    void stop();
    /** @brief Poll for a monitor event.
     *
     * The data will be in PvaClientData.
     * @return (false,true) means event (did not, did) occur.
     */
    bool poll();
    /** @brief Wait for a monitor event.
     *
     * The data will be in PvaClientData.
     * @param secondsToWait Time to wait for event.
     * @return (false,true) means event (did not, did) occur.
     */
    bool waitEvent(double secondsToWait = 0.0);
    /** @brief Release the monitorElement returned by poll
     */
    void releaseEvent();
    /** @brief The data in which monitor events are placed.
     * @return The interface.
     */
    PvaClientMonitorDataPtr getData();   
    /** Deprecated method
     * \deprecated This method will go away in future versions. 
     */
    void destroy()  EPICS_DEPRECATED {}
private:
    std::string getRequesterName();
    void message(std::string const & message,epics::pvData::MessageType messageType);
    void monitorConnect(
        const epics::pvData::Status& status,
        epics::pvData::MonitorPtr const & monitor,
        epics::pvData::StructureConstPtr const & structure);
    void unlisten(epics::pvData::MonitorPtr const & monitor);
    void monitorEvent(epics::pvData::MonitorPtr const & monitor);

    PvaClientMonitor(
        PvaClientPtr const &pvaClient,
        epics::pvAccess::Channel::shared_pointer const & channel,
        epics::pvData::PVStructurePtr const &pvRequest);

    void checkMonitorState();
    enum MonitorConnectState {connectIdle,connectActive,connected};

    PvaClient::weak_pointer pvaClient;
    epics::pvAccess::Channel::weak_pointer channel;
    epics::pvData::PVStructurePtr pvRequest;
    epics::pvData::Mutex mutex;
    epics::pvData::Event waitForConnect;
    epics::pvData::Event waitForEvent;
    PvaClientMonitorDataPtr pvaClientData;

    bool isStarted;
    epics::pvData::Status connectStatus;
    epics::pvData::MonitorPtr monitor;
    epics::pvData::MonitorElementPtr monitorElement;
    PvaClientMonitorRequesterWPtr pvaClientMonitorRequester;

    MonitorConnectState connectState;
    bool userPoll;
    bool userWait;
    MonitorRequesterImplPtr monitorRequester;
    friend class MonitorRequesterImpl;
};

/**
 * @brief Optional client callback.
 *
 */
class PvaClientRPCRequester
{
public:
    POINTER_DEFINITIONS(PvaClientRPCRequester);
    virtual ~PvaClientRPCRequester() {}
    /**
     * @brief The request is done. This is always called with no locks held.
     * @param status Completion status.
     * @param pvaClientRPC The pvaClientRPC interface.
     * @param pvResponse The response data for the RPC request or <code>null</code> if the request failed.
     */
    virtual void requestDone(
        const epics::pvData::Status& status,
        PvaClientRPCPtr const & pvaClientRPC,
        epics::pvData::PVStructure::shared_pointer const & pvResponse) = 0;
};
// NOTE: must use separate class that implements RPCRequester,
// because pvAccess holds a shared_ptr to RPCRequester instead of weak_pointer
class  RPCRequesterImpl;
typedef std::tr1::shared_ptr<RPCRequesterImpl> RPCRequesterImplPtr;

/**
 * @brief An easy to use alternative to RPC.
 *
 */
class epicsShareClass PvaClientRPC :
    public std::tr1::enable_shared_from_this<PvaClientRPC>
{
public:
    POINTER_DEFINITIONS(PvaClientRPC);
    /** @brief Create a PvaClientRPC.
     * @param &pvaClient Interface to PvaClient
     * @param channel Interface to Channel
     * @return The interface to the PvaClientRPC.
     */
    static PvaClientRPCPtr create(
        PvaClientPtr const &pvaClient,
        epics::pvAccess::Channel::shared_pointer const & channel);
    /** @brief Create a PvaClientRPC.
     * @param &pvaClient Interface to PvaClient
     * @param channel Interface to Channel
     * @param pvRequest The request structure.
     * @return The interface to the PvaClientRPC.
     */
    static PvaClientRPCPtr create(
        PvaClientPtr const &pvaClient,
        epics::pvAccess::Channel::shared_pointer const & channel,
        epics::pvData::PVStructurePtr const &pvRequest
    );
    /** @brief Destructor
     */
    ~PvaClientRPC();
    /**
     * @brief Set a timeout for a request.
     * @param responseTimeout The time in seconds to wait for a request to complete.
     */
    void setResponseTimeout(double responseTimeout) 
    {
        this->responseTimeout = responseTimeout;
    }
    /**
     * @brief Get the responseTimeout.
     * @return The value.
     */
    double getResponseTimeout()
    {
        return responseTimeout;
    }
    /** @brief Call issueConnect and then waitConnect.
     *
     * An exception is thrown if connect fails.
     */
    void connect();
    /** @brief Issue the channelRPC connection to the channel.
     *
     * This can only be called once.
     * An exception is thrown if connect fails.
     * @throw runtime_error if failure.
     */
    void issueConnect();
    /** @brief Wait until the channelRPC connection to the channel is complete.
     * @return status;
     */
    epics::pvData::Status waitConnect();
    /** @brief Issue a request and wait for response
      *
      * Note that if responseTimeout is ( lt 0.0, ge 0.0) then this (will, will not) block
      * until response completes or timeout.
      * @param pvArgument The data to send to the service.
      * @return The result
      * @throw runtime_error if failure.
     */
    epics::pvData::PVStructure::shared_pointer request(
        epics::pvData::PVStructure::shared_pointer const & pvArgument);
    /** @brief issue a request and return immediately.
      * @param pvArgument The data to send to the service.
      * @param pvaClientRPCRequester The requester that is called with the result.
      * @throw runtime_error if failure.
     */
    void request(
        epics::pvData::PVStructure::shared_pointer const & pvArgument,
        PvaClientRPCRequesterPtr const & pvaClientRPCRequester);
private:
    PvaClientRPC(
        PvaClientPtr const &pvaClient,
        epics::pvAccess::Channel::shared_pointer const & channel,
        epics::pvData::PVStructurePtr const &pvRequest);
    std::string getRequesterName();
    void message(std::string const & message,epics::pvData::MessageType messageType);
    void rpcConnect(
        const epics::pvData::Status& status,
        epics::pvAccess::ChannelRPC::shared_pointer const & channelRPC);
    void requestDone(
        const epics::pvData::Status& status,
        epics::pvAccess::ChannelRPC::shared_pointer const & channelRPC,
        epics::pvData::PVStructure::shared_pointer const & pvResponse);

    void checkRPCState();

    enum RPCConnectState {connectIdle,connectActive,connected};
    epics::pvData::Status connectStatus;
    RPCConnectState connectState;

    PvaClient::weak_pointer pvaClient;
    epics::pvAccess::Channel::weak_pointer channel;
    epics::pvData::PVStructurePtr pvRequest;
    
    epics::pvData::Mutex mutex;
    epics::pvData::Event waitForConnect;
    epics::pvData::Event waitForDone;

    PvaClientRPCRequesterWPtr pvaClientRPCRequester;    
    RPCRequesterImplPtr rpcRequester;
    epics::pvAccess::ChannelRPC::shared_pointer channelRPC;
    epics::pvData::PVStructurePtr pvResponse;

    enum RPCState {rpcIdle,rpcActive,rpcComplete};
    RPCState rpcState;
    double responseTimeout;
    friend class RPCRequesterImpl;
};

}}

#endif  /* PVACLIENT_H */

/** @page Overview Documentation
 *
 * <a href = "../pvaClientCPP.html">pvaClientCPP.html</a>
 *
 */

