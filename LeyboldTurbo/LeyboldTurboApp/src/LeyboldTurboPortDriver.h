#ifndef LEYBOLD_TURBO_PORT_DRIVER_H
#define LEYBOLD_TURBO_PORT_DRIVER_H

#include <asynPortDriver.h>

#include <map>
#include <string>
#include <vector>

class CLeyboldTurboPortDriver : public asynPortDriver {
public:
	class CException;
    CLeyboldTurboPortDriver(const char *AsynPortName, int NumPumps);
    ~CLeyboldTurboPortDriver();
    virtual asynStatus readFloat64(asynUser *pasynUser, epicsFloat64 *value);
    virtual asynStatus writeUInt32Digital(asynUser *pasynUser, epicsUInt32 value, epicsUInt32 mask);
	void addIOPort(const char* IOPortName);
                 
protected:

private:
	std::map<std::string, int> m_Parameters;
	std::vector<asynUser*> m_AsynUsers;
};

#endif // LEYBOLD_TURBO_PORT_DRIVER_H
