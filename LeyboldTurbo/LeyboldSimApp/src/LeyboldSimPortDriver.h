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
	void addIOPort(const char* IOPortName);
                 
protected:
    bool process(asynUser *pasynUser);
	static void octetConnectionCallback(void *drvPvt, asynUser *pasynUser, char *portName, 
                  size_t len, int eomReason);
	static void CLeyboldSimPortDriver::ListenerThread(void* parm);

private:
	std::map<std::string, int> m_Parameters;
	epicsMutexId m_mutexId;
	size_t m_NumConnected;
	volatile bool m_Exiting;
};

#endif // LEYBOLD_TURBO_PORT_DRIVER_H
