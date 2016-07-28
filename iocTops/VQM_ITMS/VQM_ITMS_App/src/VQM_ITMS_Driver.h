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

#ifdef BUILD_WITH_SDK
struct IServiceWrapper;
#define DeviceWrapper IServiceWrapper
struct SDeviceConnectionInfo;
#else
struct DeviceWrapper;
#endif
struct SVQM_800_Error;

class CVQM_ITMS_Driver : public CVQM_ITMS_Base {
	class CThreadRunable;
	friend class CThreadRunable;
public:
	//////////////////////////////////////////////////////////////////////////////////////////////////
	//																								//
	//	class CVQM_ITMS_Driver::CException : public CVQM_ITMS_Base::CException						//
	//	Description:																				//
	//		If an error ocurrs, an object of this type is thrown.									//
	//																								//
	//////////////////////////////////////////////////////////////////////////////////////////////////
	class CException : public ::CException
	{
		asynStatus m_Status;
	public:
		CException(asynUser* AsynUser, SVQM_800_Error const& Error, const char* functionName);
		CException(asynUser* AsynUser, asynStatus Status, const char* functionName, std::string const& what);
		asynStatus Status() const {
			return m_Status;
		}
	};

    CVQM_ITMS_Driver(const char *AsynPortName, int numTraps);
    ~CVQM_ITMS_Driver();
	void disconnect();
    virtual asynStatus writeInt32(asynUser *pasynUser, epicsInt32 value);
    virtual asynStatus readFloat64(asynUser *pasynUser, epicsFloat64 *value);
    virtual asynStatus writeFloat64(asynUser *pasynUser, epicsFloat64 value);
    virtual asynStatus readFloat32Array(asynUser *pasynUser, epicsFloat32 *value,
                                        size_t nElements, size_t *nIn);
	bool GetScanData(int TableIndex, std::vector<float>& RawData, std::vector<float>& PartialPressure);
	void addIOPort(const char* DeviceAddress);
	static CVQM_ITMS_Driver* Instance() {
		return m_Instance;
	}
	asynStatus ErrorHandler(int TableIndex, ::CException const& E);
	size_t NrInstalled() {
		return m_Threads.size();
	}

                 
protected:
	static int UsedParams();
	void ThrowException(SVQM_800_Error const& Error, const char* Function, bool ThrowIt = true);
#ifdef BUILD_WITH_SDK
	void SetGaugeState(int Connection, bool Scanning, bool ThrowIt = true);
#else
	void ThrowException(asynUser* pasynUser, asynStatus Status, const char* Function, std::string const& what, bool ThrowIt = true) const;
	void SetGaugeState(asynUser* IOUser, bool Scanning, bool ThrowIt = true);
#endif
	bool GetGaugeState(int TableIndex);

private:
	DeviceWrapper* m_serviceWrapper;
	// Each of these is associated with an octet I/O connection (i.e. serial or TCP port).
	std::vector<CThreadRunable*> m_Threads;
	int m_nConnections;
#ifdef BUILD_WITH_SDK
	SDeviceConnectionInfo* m_Connections;
	std::map<int, int> m_ConnectionMap;
#else
	std::vector<asynUser*> m_Connections;
#endif
	static CVQM_ITMS_Driver* m_Instance;
};

#endif // VQM_ITMS__PORT_DRIVER_H
