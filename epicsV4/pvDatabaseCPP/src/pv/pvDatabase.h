/* pvDatabase.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 * @date 2012.11.20
 */
#ifndef PVDATABASE_H
#define PVDATABASE_H

#ifdef epicsExportSharedSymbols
#   define pvdatabaseEpicsExportSharedSymbols
#   undef epicsExportSharedSymbols
#endif

#include <list>
#include <map>
#include <deque>

#include <pv/pvData.h>
#include <pv/pvCopy.h>
#include <pv/pvTimeStamp.h>
#include <pv/rpcService.h>

#ifdef pvdatabaseEpicsExportSharedSymbols
#   define epicsExportSharedSymbols
#	undef pvdatabaseEpicsExportSharedSymbols
#endif

#include <shareLib.h>


namespace epics { namespace pvDatabase { 

class PVRecord;
typedef std::tr1::shared_ptr<PVRecord> PVRecordPtr;
typedef std::map<std::string,PVRecordPtr> PVRecordMap;

class PVRecordField;
typedef std::tr1::shared_ptr<PVRecordField> PVRecordFieldPtr;
typedef std::vector<PVRecordFieldPtr> PVRecordFieldPtrArray;
typedef std::tr1::shared_ptr<PVRecordFieldPtrArray> PVRecordFieldPtrArrayPtr;

class PVRecordStructure;
typedef std::tr1::shared_ptr<PVRecordStructure> PVRecordStructurePtr;

class PVRecordClient;
typedef std::tr1::shared_ptr<PVRecordClient> PVRecordClientPtr;
typedef std::tr1::weak_ptr<PVRecordClient> PVRecordClientWPtr;

class PVListener;
typedef std::tr1::shared_ptr<PVListener> PVListenerPtr;
typedef std::tr1::weak_ptr<PVListener> PVListenerWPtr;

class PVDatabase;
typedef std::tr1::shared_ptr<PVDatabase> PVDatabasePtr;

/**
 * @brief Base interface for a record.
 *
 * @author mrk
 */
class epicsShareClass PVRecord :
     public epics::pvData::PVCopyTraverseMasterCallback,
     public std::tr1::enable_shared_from_this<PVRecord>
{
public:
    POINTER_DEFINITIONS(PVRecord);

    /**
     * Virtual initialization method.
     * Must be implemented by derived classes.
     * This method <b>Must</b> call initPVRecord.
     * @return <b>true</b> for success and <b>false</b> for failure.
     */
    virtual bool init() {initPVRecord(); return true;}
    /**
     * Optional method for derived class.
     * It is called before record is added to database.
     */
    virtual void start() {}
    /**
     *  Optional method.
     *  It is the method that makes a record smart.
     *  If it encounters errors it should raise alarms and/or
     *  call the <b>message</b> method provided by the base class.
     */
    virtual void process();
    /**
     *  Destroy the PVRecord. Release any resources used and 
     *  get rid of listeners and requesters.
     *  If derived class overrides this then it must call PVRecord::destroy()
     *  after it has destroyed rewsorces it uses.
     */
    virtual void destroy();
    /**
     * Creates a <b>dump</b> record, i.e. a record where process does nothing. 
     * @param recordName The name of the record, which is also the channelName.
     * @param pvStructure The top level structure.
     * @return A shared pointer to the newly created record.
     */
    static PVRecordPtr create(
        std::string const & recordName,
        epics::pvData::PVStructurePtr const & pvStructure);
    /**
     * The Destructor. Must be virtual.
     */
    virtual ~PVRecord();
    /**
     * Get the name of the record.
     * @return The name.
     */
    std::string getRecordName() const;
    /**
     * Get the top level PVStructure.
     * @return The shared pointer.
     */
    PVRecordStructurePtr getPVRecordStructure() const;
    /**
     * Find the PVRecordField for the PVField.
     * @param pvField The PVField.
     * @return The shared pointer to the PVRecordField.
     */
    PVRecordFieldPtr findPVRecordField(
        epics::pvData::PVFieldPtr const & pvField);
    /**
     * Lock the record.
     * Any code must lock while accessing a record.
     */
    void lock();
    /**
     * Unlock the record.
     */
    void unlock();
    /**
     * If <b>true</b> then just like <b>lock</b>.
     * If <b>false</b>client can not access record.
     * Code can try to simultaneously hold the lock for more than two records
     * by calling this method but must be willing to accept failure.
     * @return <b>true</b> if the record is locked.
     */
    bool tryLock();
    /**
     * A client that holds the lock for one record can lock one other record.
     * A client <b>must</b> not call this if the client already has the lock for
     * more then one record.
     *
     * @param otherRecord The other record to lock.
     */
    void lockOtherRecord(PVRecordPtr const & otherRecord);
    /**
     * Every client that accesses the record must call this so that the
     * client can be notified when the record is deleted.
     * @param pvRecordClient The client.
     * @return <b>true</b> if the client is added.
     */
    bool addPVRecordClient(PVRecordClientPtr const & pvRecordClient);
    /**
     * Remove a client.
     * @param pvRecordClient The client.
     * @return <b>true</b> if the client is removed.
     */
    bool removePVRecordClient(PVRecordClientPtr const & pvRecordClient);
    /**
     * remove all attached clients.
     */
    void detachClients();
    /**
     * Add a PVListener.
     * This must be called before calling pvRecordField.addListener.
     * @param pvListener The listener.
     * @param pvCopy The pvStructure that has the client fields.
     * @return <b>true</b> if the listener was added.
     */
    bool addListener(
        PVListenerPtr const & pvListener,
        epics::pvData::PVCopyPtr const & pvCopy);
    /* 
     * PVCopyTraverseMasterCallback method
     * @param pvField The next client field.
     */
    void nextMasterPVField(epics::pvData::PVFieldPtr const & pvField);
    /**
     * Remove a listener.
     * @param pvListener The listener.
     * @param pvCopy The pvStructure that has the client fields.
     * @return <b>true</b> if the listener was removed.
     */
    bool removeListener(
        PVListenerPtr const & pvListener,
        epics::pvData::PVCopyPtr const & pvCopy);
    /**
     * Return a service corresponding to the specified request PVStructure.
     * @param pvRequest The request PVStructure 
     * @return The corresponding service
     */
    virtual epics::pvAccess::Service::shared_pointer getService(
        epics::pvData::PVStructurePtr const & pvRequest)
    {
        return epics::pvAccess::Service::shared_pointer();
    }

    /**
     * Begins a group of puts.
     */
    void beginGroupPut();
    /**
     * Ends a group of puts.
     */
    void endGroupPut();
    /**
     * get trace level (0,1,2) means (nothing,lifetime,process)
     * @return the level
     */
    int getTraceLevel() {return traceLevel;}
    /**
     * set trace level (0,1) means (lifetime,process)
     * @param level The level
     */
    void setTraceLevel(int level) {traceLevel = level;}
protected:
    /**
     * Constructor
     * @param recordName The name of the record
     * @param pvStructure The top level PVStructutre
     */
    PVRecord(
        std::string const & recordName,
        epics::pvData::PVStructurePtr const & pvStructure);
    /**
     * Initializes the base class. Must be called by derived classes.
     */
    void initPVRecord();
    /**
     * Convience method for derived classes.
     * @return The shared pointer to the top level PVStructure.
     */
    epics::pvData::PVStructurePtr getPVStructure();
    PVRecordPtr getPtrSelf()
    {
        return shared_from_this();
    }
private:
    PVRecordFieldPtr findPVRecordField(
        PVRecordStructurePtr const & pvrs,
        epics::pvData::PVFieldPtr const & pvField);

    std::string recordName;
    epics::pvData::PVStructurePtr pvStructure;
    PVRecordStructurePtr pvRecordStructure;
    std::list<PVListenerWPtr> pvListenerList;
    std::list<PVRecordClientWPtr> pvRecordClientList;
    epics::pvData::Mutex mutex;
    std::size_t depthGroupPut;
    int traceLevel;
    bool isDestroyed;

    epics::pvData::PVTimeStamp pvTimeStamp;
    epics::pvData::TimeStamp timeStamp;

    // following only valid while addListener or removeListener is active.
    bool isAddListener;
    PVListenerWPtr pvListener;
};

epicsShareFunc std::ostream& operator<<(std::ostream& o, const PVRecord& record);

/**
 * @brief Interface for a field of a record.
 *
 * One exists for each field of the top level PVStructure.
 * @author mrk
 */
class epicsShareClass PVRecordField :
     public virtual epics::pvData::PostHandler,
     public std::tr1::enable_shared_from_this<PVRecordField>
{
public:
    POINTER_DEFINITIONS(PVRecordField);
    /**
     * Constructor.
     * @param pvField The field from the top level structure.
     * @param parent The parent.
     * @param pvRecord The PVRecord.
     */
    PVRecordField(
        epics::pvData::PVFieldPtr const & pvField,
        PVRecordStructurePtr const &parent,
        PVRecordPtr const & pvRecord);
    /**
     * Destructor.
     */
    virtual ~PVRecordField();
    /**
     *   Release any resources used
     */
    virtual void destroy();
    /**
     * Get the parent.
     * @return The parent.
     */
    PVRecordStructurePtr getParent();
    /**
     * Get the PVField.
     * @return The shared pointer.
     */
    epics::pvData::PVFieldPtr getPVField();
    /**
     * Get the full name of the field, i.e. field,field,..
     * @return The full name.
     */
    std::string getFullFieldName();
    /**
     * Get the recordName plus the full name of the field, i.e. recordName.field,field,..
     * @return The name.
     */
    std::string getFullName();
    /**
     * Returns the PVRecord to which this field belongs.
     * @return The shared pointer,
     */
    PVRecordPtr getPVRecord();
    /**
     * This is called by the code that implements the data interface.
     * It is called whenever the put method is called.
     */
    virtual void postPut();
protected:
    PVRecordFieldPtr getPtrSelf()
    {
        return shared_from_this();
    }
    virtual void init();
    virtual void postParent(PVRecordFieldPtr const & subField);
    virtual void postSubField();
private:
    bool addListener(PVListenerPtr const & pvListener);
    virtual void removeListener(PVListenerPtr const & pvListener);
    void callListener();

    std::list<PVListenerWPtr> pvListenerList;
    epics::pvData::PVFieldPtr pvField;
    bool isStructure;
    PVRecordStructurePtr parent;
    PVRecordPtr pvRecord;
    std::string fullName;
    std::string fullFieldName;
    friend class PVRecordStructure;
    friend class PVRecord;
};

/**
 * @brief Interface for a field that is a structure.
 *
 * One exists for each structure field of the top level PVStructure.
 * @author mrk
 */
class epicsShareClass PVRecordStructure : public PVRecordField {
public:
    POINTER_DEFINITIONS(PVRecordStructure);
    /**
     * Constructor.
     * @param pvStructure The data.
     * @param parent The parent
     * @param pvRecord The record that has this field.
     */
    PVRecordStructure(
        epics::pvData::PVStructurePtr const &pvStructure,
        PVRecordStructurePtr const &parent,
        PVRecordPtr const & pvRecord);
    /**
     * Destructor.
     */
    virtual ~PVRecordStructure();
    /**
     *   Release any resources used
     */
    virtual void destroy();
    /**
     * Get the sub fields.
     * @return the array of PVRecordFieldPtr.
     */
    PVRecordFieldPtrArrayPtr getPVRecordFields();
    /**
     * Get the data structure/
     * @return The shared pointer.
     */
    epics::pvData::PVStructurePtr getPVStructure();
    /**
     * Called by implementation code of PVRecord.
     */
    virtual void postPut();
protected:
    /**
     * Called by implementation code of PVRecord.
     */
    virtual void init();
private:
    virtual void removeListener(PVListenerPtr const & pvListener);

    epics::pvData::PVStructurePtr pvStructure;
    PVRecordFieldPtrArrayPtr pvRecordFields;
    friend class PVRecord;
};

/**
 * @brief An interface implemented by code that accesses the record.
 *
 * @author mrk
 */
class epicsShareClass PVRecordClient {
public:
    POINTER_DEFINITIONS(PVRecordClient);
    /**
     * Destructor.
     */
    virtual ~PVRecordClient() {}
    /**
     * Detach from the record because it is being removed.
     * @param pvRecord The record.
     */
    virtual void detach(PVRecordPtr const & pvRecord) = 0;
};

/**
 * @brief Listener for PVRecord::message.
 *
 * An interface that is implemented by code that traps calls to PVRecord::message.
 * @author mrk
 */
class epicsShareClass PVListener :
    virtual public PVRecordClient
{
public:
    POINTER_DEFINITIONS(PVListener);
    /**
     * Destructor.
     */
    virtual ~PVListener() {}
    /**
     * pvField has been modified.
     * This is called if the listener has called PVRecordField::addListener for pvRecordField.
     * @param pvRecordField The modified field.
     */
    virtual void dataPut(PVRecordFieldPtr const & pvRecordField) = 0;
    /**
     * A subfield has been modified.
     * @param requested The structure that was requested.
     * @param pvRecordField The field that was modified.
     */
    virtual void dataPut(
        PVRecordStructurePtr const & requested,
        PVRecordFieldPtr const & pvRecordField) = 0;
    /**
     * Begin a set of puts.
     * @param pvRecord The record.
     */
    virtual void beginGroupPut(PVRecordPtr const & pvRecord) = 0;
    /**
     * End a set of puts.
     * @param pvRecord The record.
     */
    virtual void endGroupPut(PVRecordPtr const & pvRecord) = 0;
    /**
     * Connection to record is being terminated.
     * @param pvRecord The record.
     */
    virtual void unlisten(PVRecordPtr const & pvRecord) = 0;
};

/**
 * @brief The interface for a database of PVRecords.
 *
 * @author mrk
 */
class epicsShareClass PVDatabase {
public:
    POINTER_DEFINITIONS(PVDatabase);
    /**
     * Get the master database.
     * @return The shared pointer.
     */
    static PVDatabasePtr getMaster();
    /**
     * Destructor
     */
    virtual ~PVDatabase();
    /**
     *  Destroy the PVDatabase.
     */
    virtual void destroy();
    /**
     * Find a record.
     * An empty pointer is returned if the record is not in the database.
     * @param recordName The record to find.
     * @return The shared pointer.
     */
    PVRecordPtr findRecord(std::string const& recordName);
    /**
     * Add a record.
     * @param The record to add.
     * @return <b>true</b> if record was added.
     */
    bool addRecord(PVRecordPtr const & record);
    /**
     * Remove a record.
     * @param The record to remove.
     * @return <b>true</b> if record was removed.
     */
    bool removeRecord(PVRecordPtr const & record);
    /**
     * Get the names of all the records in the database.
     * @return The names.
     */
    epics::pvData::PVStringArrayPtr getRecordNames();
private:
    PVDatabase();
    void lock();
    void unlock();
    PVRecordMap  recordMap;
    epics::pvData::Mutex mutex;
    bool isDestroyed;
};

}}

#endif  /* PVDATABASE_H */

