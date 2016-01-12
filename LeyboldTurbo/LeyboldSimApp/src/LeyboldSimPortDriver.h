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
//	Written for CCFE (Culham Centre for Fusion Energy).												//
//																									//
//	LeyboldTurbo is distributed subject to a Software License Agreement								//
//	found in file LICENSE that is included with this distribution.									//
//																									//
//////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef LEYBOLD_SIM_DRIVER_H
#define LEYBOLD_SIM_DRIVER_H

#include <USSPacket.h>

#include <LeyboldBase.h>

#include <epicsMutex.h>
#include <iocsh.h>

#include <map>
#include <vector>
#include <string>

class epicsThread;

class CLeyboldSimPortDriver : public CLeyboldBase {
public:
    CLeyboldSimPortDriver(const char *AsynPortName, int NumPumps, int NoOfPZD);
    ~CLeyboldSimPortDriver();
	void addIOPort(const char* IOPortName);
	static CLeyboldSimPortDriver* Instance() {
		return m_Instance;
	}
	static void LeyboldSimAddIOPort(const iocshArgBuf *args);
	static void LeyboldSimPortDriverConfigure(const iocshArgBuf *args);

protected:
    template<size_t NoOfPZD> bool read(asynUser *pasynUser, asynUser *IOUser, USSPacket<NoOfPZD>& USSReadPacket, size_t TableIndex);
    template<size_t NoOfPZD> bool process(asynUser *pasynUser, asynUser *IOUser, USSPacket<NoOfPZD> const& USSReadPacket, USSPacket<NoOfPZD>& USSWritePacket, size_t TableIndex);
    void process(USSPacket<NoOfPZD6>& USSWritePacket, int TableIndex);
	static void octetConnectionCallback(void *userPvt, asynUser *pasynUser,
                      char *data,size_t numchars, int eomReason);
	class CThreadRunable;
	friend class CThreadRunable;

private:
	void setDefaultValues(size_t TableIndex);
    static epicsMutex m_Mutex;

	static int UsedParams();

	struct RunRecord {
		RunRecord(RunStates RunState, unsigned TimeStamp) {
			m_RunState = RunState;
			m_TimeStamp = TimeStamp;
		}
		RunStates m_RunState;
		unsigned m_TimeStamp;
	};

	CThreadRunable* m_ThreadRunable;
	std::vector<epicsThread*> m_Threads;
	std::map<std::string, size_t> m_TableLookup;	// The application does not launch threads in linear sequence. Need this lookup.
	std::vector<RunRecord> m_RunRecord;				// For each simulated pump, a record of the Running state, for the previous iteration.
	std::vector<asynUser*> m_asynUsers;				// An Asyn user for each simulated pump.
	static CLeyboldSimPortDriver* m_Instance;
};

#endif // LEYBOLD_SIM_DRIVER_H
