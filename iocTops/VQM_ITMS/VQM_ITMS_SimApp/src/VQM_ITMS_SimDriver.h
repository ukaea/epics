//////////////////////////////////////////////////////////////////////////////////////////////////////
//																									//
//	Module:																							//
//		VQM_ITMS_SimDriver.h																			//
//																									//
//	Description:																					//
//		Declares the CVQM_ITMS_SimDriver class.														//
//		This uses AsynPortDriver and asynOctetSyncIO to provide a simulated connection with			//
//		the CVQM_ITMSPortDriver class instance.													//
//																									//
//		The class communicates by means of the Universal Serial Interface (USS)						//
//		protocol (http://www.automation.siemens.com/WW/forum/guests/PostShow.aspx?PostID=104133).	//
//																									//
//	Author:  Peter Heesterman (Tessella plc). Date: 05 Jan 2015.									//
//	Written for CCFE (Culham Centre for Fusion Energy).												//
//																									//
//	VQM_ITMS is distributed subject to a Software License Agreement								//
//	found in file LICENSE that is included with this distribution.									//
//																									//
//////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef VQM_ITMS_SIM_DRIVER_H
#define VQM_ITMS_SIM_DRIVER_H

#include <VQM_ITMS_Base.h>

#include <epicsMutex.h>
#include <iocsh.h>

#include <map>
#include <vector>
#include <string>

class epicsThread;

class CVQM_ITMS_SimPortDriver : public CVQM_ITMS_Base {
public:
    CVQM_ITMS_SimPortDriver(const char *AsynPortName, int numTraps);
    ~CVQM_ITMS_SimPortDriver();
	void addIOPort(const char* IOPortName);
	static CVQM_ITMS_SimPortDriver* Instance() {
		return m_Instance;
	}
	static void VQM_ITMS_SimAddIOPort(const iocshArgBuf *args);
	static void VQM_ITMS_SimPortDriverConfigure(const iocshArgBuf *args);

protected:
	static void octetConnectionCallback(void *userPvt, asynUser *pasynUser,
                      char *data,size_t numchars, int eomReason);
	class CThreadRunable;
	friend class CThreadRunable;

private:
	void setDefaultValues(size_t TableIndex);
    static epicsMutex m_Mutex;

	static int UsedParams();

	CThreadRunable* m_ThreadRunable;
	std::vector<epicsThread*> m_Threads;
	std::map<std::string, size_t> m_TableLookup;	// The application does not launch threads in linear sequence. Need this lookup.
	std::vector<asynUser*> m_asynUsers;				// An Asyn user for each simulated pump.
	static CVQM_ITMS_SimPortDriver* m_Instance;
};

#endif // VQM_ITMS_SIM_DRIVER_H
