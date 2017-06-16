/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author Dave Hickin
 *
 */



#include <epicsThread.h>

#include <cmath>
#include <sstream>

#define epicsExportSharedSymbols
#include <pv/device.h>

namespace epics { namespace exampleCPP { namespace exampleRPC {


DevicePtr Device::create()
{
    return DevicePtr(new Device());
}

std::string Device::toString(Device::State state)
{
    switch(state)
    {
    case IDLE:
        return "IDLE";
    case READY:
        return "READY";
    case RUNNING:
        return "RUNNING";
    case PAUSED:
        return "PAUSED";
     default:
        throw std::runtime_error("Unknown state");
    }
}

Device::State Device::getState()
{
    epics::pvData::Lock lock(mutex);
    return state;
}

void Device::registerCallback(Callback::shared_pointer const & callback)
{
    epics::pvData::Lock lock(mutex);
    if (find(callbacks.begin(),callbacks.end(), callback) != callbacks.end()) return;

    callbacks.push_back(callback);
}

bool Device::unregisterCallback(Device::Callback::shared_pointer const & callback)
{
    epics::pvData::Lock lock(mutex);
    std::vector<Callback::shared_pointer>::iterator foundCB
        = find(callbacks.begin(),callbacks.end(), callback);
    bool found = foundCB == callbacks.end();
    callbacks.erase(foundCB);
    return found;
}

void Device::scanComplete()
{
    flags |= Device::Callback::SCAN_COMPLETE;
}


void Device::update()
{
    epics::pvData::Lock lock(mutex);
    std::vector<Callback::shared_pointer> callbacks = this->callbacks;

    if (flags != 0)
    {
        for (std::vector<Callback::shared_pointer>::iterator
                 it = callbacks.begin();
             it != callbacks.end(); ++it)
        {
            (*it)->update(flags);
        }

        flags = 0;
    }
}

Device::Device()
: state(IDLE), index(0)
{
   thread =  std::auto_ptr<epicsThread>(new epicsThread(
        *this,
        "device",
        epicsThreadGetStackSize(epicsThreadStackSmall),
        epicsThreadPriorityLow));
        startThread();
}

void Device::run()
{
    while (true)
    {
        try {
            epicsThreadSleep(0.1);
            epics::pvData::Lock lock(mutex);
            if (state == IDLE || state == RUNNING)
            {
                if (positionRB != positionSP)
                {
                    double dx = positionSP.x - positionRB.x;
                    double dy = positionSP.y - positionRB.y;

                    const double ds = sqrt(dx*dx+dy*dy);
                    const double maxds = 0.01;
                    // avoid very small final steps
                    const double maxds_x = maxds + 1.0e-5;

                    if (ds > maxds_x)
                    {
                        double scale = maxds/ds;
                        dx *= scale;
                        dy *= scale;
                        setReadbackImpl(Point(
                            positionRB.x + dx, positionRB.y + dy));
                    }
                    else
                    {
                        setReadbackImpl(positionSP);
                    }
                }
            }

            if (state == RUNNING && positionRB == positionSP)
            {
                if (index < points.size())
                {
                    setSetpointImpl(points[index]);
                    ++index;
                }
                else
                {
                    scanComplete();
                    stopScan();
                }
            }
        }
        catch (...) { abort(); }
        update();
    }
}

Point Device::getPositionSetpoint()
{
    return positionSP;
}

Point Device::getPositionReadback()
{
    return positionRB;
}


void Device::setSetpoint(Point sp)
{
    if (state != IDLE)
    {
        std::stringstream ss;
        ss << "Cannot set position setpoint unless device is IDLE. State is " << toString(state);
        throw std::runtime_error(ss.str());
    }
    setSetpointImpl(sp);
}

void Device::setSetpointImpl(Point sp)
{
    positionSP = sp;
    flags |= Device::Callback::SETPOINT_CHANGED;
}

void Device::setReadbackImpl(Point rb)
{
    positionRB = rb;
    flags |= Device::Callback::READBACK_CHANGED;
}

void Device::setStateImpl(State state)
{
    this->state = state;
    flags |= Device::Callback::STATE_CHANGED;  
}


void Device::abort()
{
    epics::pvData::Lock lock(mutex);
    std::cout << "Abort" << std::endl;
    setStateImpl(IDLE);
    points.clear();
    if (positionSP != positionRB)
        setSetpointImpl(positionRB);
}

void Device::configure(const std::vector<Point> & newPoints)
{
    epics::pvData::Lock lock(mutex);
    if (state != IDLE)
    {
        std::stringstream ss;
        ss << "Cannot configure device unless it is IDLE. State is " << toString(state);
        throw std::runtime_error(ss.str());

    }
    std::cout << "Configure" << std::endl;
    setStateImpl(READY);
    points = newPoints;

    if (positionSP != positionRB)
        setSetpointImpl(positionRB);
}

void Device::runScan()
{
    epics::pvData::Lock lock(mutex);
    if (state != READY)
    {
        std::stringstream ss;
        ss << "Cannot run device unless it is READY. State is " << toString(state);
        throw std::runtime_error(ss.str());
    }
    std::cout << "Run" << std::endl;
    index = 0;
    setStateImpl(RUNNING);
}

void Device::pause()
{
    epics::pvData::Lock lock(mutex);
    if (state != RUNNING) 
    {
        std::stringstream ss;
        ss << "Cannot pause device unless it is RUNNING. State is " << toString(state);
        throw std::runtime_error(ss.str());
    }
    std::cout << "Pause" << std::endl;
    setStateImpl(PAUSED);
 }

void Device::resume()
{
    epics::pvData::Lock lock(mutex);
    if (state != PAUSED) 
    {
        std::stringstream ss;
        ss << "Cannot resume device unless it is PAUSED. State is " << toString(state);
        throw std::runtime_error(ss.str());
    }
    std::cout << "Resume" << std::endl;
    setStateImpl(RUNNING);

}

void Device::stopScan()
{
    epics::pvData::Lock lock(mutex);
    switch (state)
    {
    case RUNNING:
    case PAUSED:
    case READY:
        std::cout << "Stop" << std::endl;
        setStateImpl(READY);
        if (positionSP != positionRB)
            setSetpointImpl(positionRB);
        break;
    default:
        {
            std::stringstream ss;
            ss << "Cannot stop device unless it is RUNNING, PAUSED or READY. State is " << toString(state);
            throw std::runtime_error(ss.str());
        }
    }
}

void Device::rewind(int n)
{
    epics::pvData::Lock lock(mutex);
    switch (state)
    {
    case RUNNING:
    case PAUSED:
        if (n < 0)
        {
            std::stringstream ss;
            ss << "Rewind argument cannot be negative. Argument is " << n;
            throw std::runtime_error(ss.str());
        }
        if (n > 0)
        {
            size_t un = static_cast<size_t>(n);
            std::cout << "Rewind(" << n << ")" << std::endl;
            if (un < index)
                index -= un+1;
            else
                index = 0;
            setSetpointImpl(points[index]);
            ++index;
        }
            break;
    default:
            {
            std::stringstream ss;
            ss << "Cannot rewind device unless it is RUNNING or PAUSED. State is " << toString(state);
            throw std::runtime_error(ss.str());
        }
    }
}

}}}


