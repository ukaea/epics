//////////////////////////////////////////////////////////////////////////////////////////////////////
//																									//
//	Module:																							//
//		LeyboldSimDriver.h																			//
//																									//
//	Description:																					//
//		Declares the CLeyboldSimDriver class.														//
//		This uses AsynPortDriver and asynOctetSyncIO to provide a simulated connection with			//
//		the CLeyboldTurboPortDriver class instance.													//
//																									//
//		The class communicates by means of the Universal Serial Interface (USS)						//
//		protocol (http://www.automation.siemens.com/WW/forum/guests/PostShow.aspx?PostID=104133).	//
//																									//
//	Author:  Peter Heesterman (Tessella plc). Date: 05 Jan 2015.									//
//																									//
//	LeyboldTurbo is distributed subject to a Software License Agreement								//
//	found in file LICENSE that is included with this distribution.									//
//																									//
//////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef LEYBOLD_SIM_DRIVER_H
#define LEYBOLD_SIM_DRIVER_H

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
	// Each parameter is associated with an int handle.
	// This structure is used in order to address them by name, which is more convenient.
	std::map<std::string, int> m_Parameters;
	void setDefaultValues(int TableIndex);

	int m_NumConnected;				// how many sockets have actually connected?
	volatile bool m_Exiting;		// Signals the listening thread to exit.
	std::vector<bool> m_WasRunning;	// For each simulated pump, was it in the Running state, on the previous iteration?
};

#endif // LEYBOLD_SIM_DRIVER_H
