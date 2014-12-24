#ifndef LEYBOLD_TURBO_PORT_DRIVER_H
#define LEYBOLD_TURBO_PORT_DRIVER_H

#include <asynPortDriver.h>

#include <map>
#include <vector>
#include <string>

class CLeyboldSimPortDriver : public asynPortDriver {
public:
	class CException;
    CLeyboldSimPortDriver(const char *AsynPortName, int NumPumps);
    ~CLeyboldSimPortDriver();
	void addIOPort(const char* IOPortName);
                 
protected:
    bool process(asynUser *pasynUser, int TableIndex);
	static void octetConnectionCallback(void *userPvt, asynUser *pasynUser,
                      char *data,size_t numchars, int eomReason);
	static void ListenerThread(void* parm);

private:
	std::map<std::string, int> m_Parameters;

	int m_NumConnected;
	volatile bool m_Exiting;
	std::vector<bool> m_WasRunning;
};

#endif // LEYBOLD_TURBO_PORT_DRIVER_H
