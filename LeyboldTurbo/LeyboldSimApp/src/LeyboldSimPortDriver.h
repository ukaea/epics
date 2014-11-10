#ifndef LEYBOLD_TURBO_PORT_DRIVER_H
#define LEYBOLD_TURBO_PORT_DRIVER_H

#include <asynPortDriver.h>

#include <map>
#include <string>
#include <vector>

class CLeyboldSimPortDriver : public asynPortDriver {
public:
	class CException;
    CLeyboldSimPortDriver(const char *AsynPortName, int NumPumps);
    ~CLeyboldSimPortDriver();
    virtual asynStatus readUInt32Digital(asynUser *pasynUser, epicsUInt32 *value, epicsUInt32 mask);
	void addIOPort(const char* IOPortName);
                 
protected:

private:
	std::map<std::string, int> m_Parameters;
	std::vector<asynUser*> m_AsynUsers;
};

#endif // LEYBOLD_TURBO_PORT_DRIVER_H
