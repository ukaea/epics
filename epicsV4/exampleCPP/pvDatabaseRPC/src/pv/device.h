/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author Dave Hickin
 *
 */

#ifndef EXAMPLERPC_DEVICE_H
#define EXAMPLERPC_DEVICE_H


#ifdef epicsExportSharedSymbols
#   define deviceExportSharedSymbols
#   undef epicsExportSharedSymbols
#endif

#include <pv/pvDatabase.h>
#include <pv/point.h>
#include <epicsThread.h>

#ifdef deviceExportSharedSymbols
#   define epicsExportSharedSymbols
#	undef deviceExportSharedSymbols
#endif

#include <shareLib.h>


namespace epics { namespace exampleCPP { namespace exampleRPC {


class Device;
typedef std::tr1::shared_ptr<Device> DevicePtr;

class epicsShareClass Device : public epicsThreadRunable,
    public std::tr1::enable_shared_from_this<Device>
{
public:
    static DevicePtr create();

    POINTER_DEFINITIONS(Device);

    enum State {
        IDLE, READY, RUNNING, PAUSED
    };

    static std::string toString(State state);

    State getState();

    virtual bool init() { return false;}
    virtual void run();
    void startThread() { thread->start(); }
    void stop() {}

   class Callback : public std::tr1::enable_shared_from_this<Callback>
   {
   public:
       POINTER_DEFINITIONS(Callback);

       virtual void update(int flags) = 0;

       const static int SETPOINT_CHANGED  = 0x1;
       const static int READBACK_CHANGED  = 0x2;
       const static int STATE_CHANGED     = 0x4;
       const static int SCAN_COMPLETE     = 0x8;
   };

    void registerCallback(Callback::shared_pointer const & callback);

    bool unregisterCallback(Callback::shared_pointer const & callback);

    void update();

    Point getPositionSetpoint();

    Point getPositionReadback();

    void setSetpoint(Point sp);

    void abort();

    void configure(const std::vector<Point> & newPoints);

    void runScan();

    void pause();

    void resume();

    void stopScan();

    void rewind(int n);

private:
    Device();

    void setSetpointImpl(Point sp);

    void setReadbackImpl(Point rb);

    void setStateImpl(State state);

    void scanComplete();

    State state;
    int flags;

    Point positionSP;
    Point positionRB;

    std::vector<Callback::shared_pointer> callbacks;

    size_t index;

    std::vector<Point> points;

    epics::pvData::Mutex mutex;
    std::auto_ptr<epicsThread> thread;
};

}}}

#endif // EXAMPLERPC_DEVICE_H

