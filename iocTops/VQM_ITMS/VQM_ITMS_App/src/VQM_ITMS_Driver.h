//////////////////////////////////////////////////////////////////////////////////////////////////////
//																									//
//	Module:																							//
//		VQM_ITMSPortDriver.h																	//
//																									//
//	Description:																					//
//		Declares the CVQM_ITMS_Driver class.													//
//		This uses AsynPortDriver and asynOctetSyncIO to connect with either:						//
//			a. One or more serial-connected VQM_ITMS  pump controllers.							//
//			b. One or more TCP-connected simulatied controllers.									//
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

#ifndef VQM_ITMS__PORT_DRIVER_H
#define VQM_ITMS__PORT_DRIVER_H

#include "VQM_ITMS_Base.h"

#include <map>
#include <string>
#include <vector>

struct IServiceWrapper;

class CVQM_ITMS_Driver : public CVQM_ITMS_Base {
public:
	// NB, an MBBI string is limited to 40 charachters in EPICS.
	static const size_t MaxEPICSMBBIStrLen = 16;
	static const size_t BitsPerUInt16 = 16;

    CVQM_ITMS_Driver(const char *AsynPortName, int numTraps);
    ~CVQM_ITMS_Driver();
	void disconnect();
    virtual asynStatus readInt32(asynUser *pasynUser, epicsInt32 *value);
    virtual asynStatus writeInt32(asynUser *pasynUser, epicsInt32 value);
    virtual asynStatus writeFloat64(asynUser *pasynUser, epicsFloat64 value);
    virtual asynStatus writeFloat64Array(asynUser *pasynUser, epicsFloat64 *value,
                                        size_t nElements);
    virtual asynStatus readFloat32Array(asynUser *pasynUser, epicsFloat32 *value,
                                        size_t nElements, size_t *nIn);
    virtual asynStatus readOctet(asynUser *pasynUser, char *value, size_t maxChars,
                                        size_t *nActual, int *eomReason);
	void addIOPort(const char* IOPortName, const char* DeviceAddress);
	static CVQM_ITMS_Driver* Instance() {
		return m_Instance;
	}
	asynStatus ErrorHandler(int TableIndex, CException const& E);
	size_t NrInstalled() {
		return m_Connections.size()-1;
	}

                 
protected:
	static int UsedParams();

private:
	struct SDeviceConnectionInfo;
	IServiceWrapper* m_serviceWrapper;
	// Each of these is associated with an octet I/O connection (i.e. serial or TCP port).
	std::vector<SDeviceConnectionInfo*> m_Connections;
	static CVQM_ITMS_Driver* m_Instance;
};

#endif // VQM_ITMS__PORT_DRIVER_H
