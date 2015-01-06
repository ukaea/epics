//////////////////////////////////////////////////////////////////////////////////////////////////////
//																									//
//	Module:																							//
//		LeyboldTurboPortDriver.h																	//
//																									//
//	Description:																					//
//		Declares the CLeyboldTurboPortDriver class.													//
//		This uses AsynPortDriver and asynOctetSyncIO to connect with either:						//
//			a. One or more serial-connected Leybold turbo pump controllers.							//
//			b. One or more TCP-connected simulatied controllers.									//
//		The class communicates by means of the Universal Serial Interface (USS)						//
//		protocol (http://www.automation.siemens.com/WW/forum/guests/PostShow.aspx?PostID=104133).	//
//																									//
//	Author:  Peter Heesterman (Tessella plc). Date: 05 Jan 2015.									//
//																									//
//	LeyboldTurbo is distributed subject to a Software License Agreement								//
//	found in file LICENSE that is included with this distribution.									//
//																									//
//////////////////////////////////////////////////////////////////////////////////////////////////////

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
    virtual asynStatus readInt32(asynUser *pasynUser, epicsInt32 *value);
    virtual asynStatus writeInt32(asynUser *pasynUser, epicsInt32 value);
	void addIOPort(const char* IOPortName);
                 
protected:

private:
	// Each parameter is associated with an int handle.
	// This structure is used in order to address them by name, which is more convenient.
	std::map<std::string, int> m_Parameters;

	// Each of these is associated with an octet I/O connection (i.e. serial or TCP port).
	std::vector<asynUser*> m_AsynUsers;
};

#endif // LEYBOLD_TURBO_PORT_DRIVER_H
