/* pvCopyMonitor.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author Marty Kraimer
 * @date 2013.04
 */
#ifndef PVCOPYMONITOR_H
#define PVCOPYMONITOR_H

#ifdef epicsExportSharedSymbols
#   define pvCopyMonitorEpicsExportSharedSymbols
#   undef epicsExportSharedSymbols
#endif

#include <string>
#include <stdexcept>
#include <memory>
#include <list>

#include <pv/monitorPlugin.h>
#include <pv/pvCopy.h>
#include <pv/pvAccess.h>

#ifdef pvCopyMonitorEpicsExportSharedSymbols
#   define epicsExportSharedSymbols
#	undef pvCopyMonitorEpicsExportSharedSymbols
#endif

#include <shareLib.h>

#include <pv/pvDatabase.h>

namespace epics { namespace pvDatabase { 

class PVCopyMonitor;
typedef std::tr1::shared_ptr<PVCopyMonitor> PVCopyMonitorPtr;
class PVCopyMonitorRequester;
typedef std::tr1::shared_ptr<PVCopyMonitorRequester> PVCopyMonitorRequesterPtr;

struct PVCopyMonitorFieldNode;
typedef std::tr1::shared_ptr<PVCopyMonitorFieldNode> PVCopyMonitorFieldNodePtr;


/** 
 * PVCopyMonitor
 * This class manages changes to fields being monitored in a PVRecord.
 */
class epicsShareClass PVCopyMonitor :
    public PVListener,
    public epics::pvData::PVCopyTraverseMasterCallback,
    public std::tr1::enable_shared_from_this<PVCopyMonitor>
{
public:
    POINTER_DEFINITIONS(PVCopyMonitor);
    /**
     * Factory method to create a PVCopyMonoitor
     * @param pvCopyMonitorRequester This is usually MonitorLocal.
     * @param pvRecord The record being monitored.
     * @param pvCopy An instance of pvCopy
     * @return A shared pointer to a PVCopyMonitor.
     */
    static PVCopyMonitorPtr create(
        PVCopyMonitorRequesterPtr const &pvCopyMonitorRequester,
        PVRecordPtr const &pvRecord,
        epics::pvData::PVCopyPtr const & pvCopy);
    /**
     * Destructor
     */
    virtual ~PVCopyMonitor();
    /**
     * Destroy the PVCopyMonitor
     */
    virtual void destroy();
    /**
     * Calls pvRecord methods to start monitoring for calls to postPut.
     * @param monitorElement the initial monotorElement
     * This holds the change and overrun bitSets that are updated
     * when postPut is called.
     */
    void startMonitoring(
         epics::pvData::MonitorElementPtr const & monitorElement);
    /**
     * Calls pvRecord methods to stop monitoring for calls to postPut.
     */
    void stopMonitoring();
    // following are PVListener methods
    /**
     * The record is being removed from the PVDatabase.
     * @param The record being removed.
     */
    virtual void detach(PVRecordPtr const & pvRecord);
    /**
     * A postPut has been issued to a field being monitored.
     * @param pvRecordField The field.
     */
    virtual void dataPut(PVRecordFieldPtr const & pvRecordField);
    /**
     * A postPut has been issued to a subfield of a field being monitored.
     * @param requested The field being monitored.
     * @param pvRecordField The field being modified.
     */
    virtual void dataPut(
        PVRecordStructurePtr const & requested,
        PVRecordFieldPtr const & pvRecordField);
    /**
     * A group of puts are being changed.
     * No monitors should be issued until endGroupPut is called.
     * @param pvRecord The record.
     */
    virtual void beginGroupPut(PVRecordPtr const & pvRecord);
    /**
     * The end of a group of puts.
     * If any fields have changed value since beginGroupPut a monitor
     * can be issued.
     * @param pvRecord The record.
     */
    virtual void endGroupPut(PVRecordPtr const & pvRecord);
    /**
     * The record is being removed from the database.
     * @param pvRecord The record.
     */
    virtual void unlisten(PVRecordPtr const & pvRecord);
    // following is PVCopyTraverseMasterCallback method
    /**
     * The PVCopyTraverseMasterCallback callback
     * Called for every field of PVRecord that is being monoitored.
     * @param pvField The field in the PVRecord.
     */
    virtual void nextMasterPVField(epics::pvData::PVFieldPtr const &pvField);
private:
    PVCopyMonitorPtr getPtrSelf()
    {
        return shared_from_this();
    }
    PVCopyMonitor(
        PVRecordPtr const &pvRecord,
        epics::pvData::PVCopyPtr const &pvCopy,
        PVCopyMonitorRequesterPtr const &pvCopyMonitorRequester);
    void init(epics::pvData::PVFieldPtr const &pvField);
    epics::pvData::MonitorPluginPtr getMonitorPlugin(size_t offset);
    PVRecordPtr pvRecord;
    epics::pvData::PVCopyPtr pvCopy;
    PVCopyMonitorRequesterPtr pvCopyMonitorRequester;
    epics::pvData::MonitorElementPtr monitorElement;
    bool isGroupPut;
    bool dataChanged;
    bool isMonitoring;
    bool isDestroyed;
    epics::pvData::Mutex mutex;
    std::list<PVCopyMonitorFieldNodePtr> monitorFieldNodeList;
};

class epicsShareClass PVCopyMonitorRequester
{
public:
    POINTER_DEFINITIONS(PVCopyMonitorRequester);
    virtual ~PVCopyMonitorRequester() {}
    virtual epics::pvData::MonitorElementPtr releaseActiveElement() = 0;
    virtual void unlisten() = 0;
};

}}

#endif  /* PVCOPYMONITOR_H */
