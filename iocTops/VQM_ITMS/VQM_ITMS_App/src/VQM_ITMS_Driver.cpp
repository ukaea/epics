//////////////////////////////////////////////////////////////////////////////////////////////////////
//																									//
//	Module:																							//
//		VQM_ITMSPortDriver.cpp																	//
//																									//
//	Description:																					//
//		Implements the CVQM_ITMS_Driver class.												//
//		This uses AsynPortDriver and asynOctetSyncIO to connect with either:						//
//			a. One or more serial-connected VQM_ITMS  ITMS controllers.							//
//			b. One or more TCP-connected simulatied controllers.									//
//																									//
//	Author:  Peter Heesterman (Tessella plc). Date: 05 Jan 2015.									//
//																									//
//	VQM_ITMS is distributed subject to a Software License Agreement								//
//	found in file LICENSE that is included with this distribution.									//
//																									//
//////////////////////////////////////////////////////////////////////////////////////////////////////

#include "VQM_ITMS_Driver.h"

#include <iocsh.h>
#include <epicsExit.h>
#include <epicsAssert.h>
#include <epicsGuard.h>
#include <asynOctetSyncIO.h>

#include "ParameterDefns.h"

#include <epicsThread.h>

#include <IServiceWrapper.h>
#include <SVQM_800_Error.h>
#include <SAnalyzedData.h>
#include <SAverageData.h>

#define epicsExportSharedSymbols
#include <epicsExport.h>

#include <stdlib.h>
#include <stdexcept>

#ifndef ASYN_TRACE_WARNING
// Added with asyn4-22
static const int ASYN_TRACE_WARNING = ASYN_TRACE_ERROR;
#endif

CVQM_ITMS_Driver* CVQM_ITMS_Driver::m_Instance;

struct CVQM_ITMS_Driver::SDeviceConnectionInfo : public ::SDeviceConnectionInfo
{
	epicsMutex* m_Mutex;
};

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
//	CVQM_ITMS_Driver::CVQM_ITMS_Driver(const char *asynPortName, int numTraps)	//
//	CVQM_ITMS_Driver::~CVQM_ITMS_Driver()											//
//																								//
//	Description:																				//
//		Class constructor & destructor.															//
//	Parameters:																					//
//		asynPortName - the IOC port name to be used (e.g. ).								//
//		numTraps - how many pumps will be attached?												//
//				 - The expectation is that addIOPort will be called this many times				//
//				 - from the st.cmd script.														//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
CVQM_ITMS_Driver::CVQM_ITMS_Driver(const char *asynPortName, int numTraps)
   : CVQM_ITMS_Base(asynPortName, 
                    numTraps,	// maxAddr
                    UsedParams()
					)
{
	m_serviceWrapper = CLIWrapperDLL::CreateServiceHandle();
	m_Instance = this;
}

CVQM_ITMS_Driver::~CVQM_ITMS_Driver()
{
	m_Instance = NULL;
	CLIWrapperDLL::ReleaseServiceHandle(m_serviceWrapper);
	disconnect();
	for(size_t Index = 0; Index < NrInstalled(); Index++)
		delete m_Connections[Index];
}

void CVQM_ITMS_Driver::disconnect()
{
	asynStatus overallstatus = asynSuccess;
	for(size_t Index = 0; Index < NrInstalled(); Index++)
		m_Connections[Index]->m_Mutex->lock();
	for(size_t Index = 0; Index < NrInstalled(); Index++)
		m_serviceWrapper->DisconnectFromDevice(*(m_Connections[Index]));
	for(size_t Index = 0; Index < NrInstalled(); Index++)
		m_Connections[Index]->m_Mutex->unlock();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
//	void CVQM_ITMS_Driver::addIOPort(const char* IOPortName)								//
//																								//
//	Description:																				//
//		Called once (from VQM_ITMSAddIOPort) for each ITMS,									//
//		in response to the st.cmd startup script.												//
//		Adds a ITMS, and the parameters to support it, to the configuration.					//
//																								//
//	Parameters:																					//
//		IOPortName. This is the I/O port name (e.g. ITMS:1).									//
//					This will have been mapped by drvAsynIPPortConfigure to a physical port,	//
//					e.g. COM1: (or /dev/ttyS0).													//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
void CVQM_ITMS_Driver::addIOPort(const char* IOPortName, const char* DeviceAddress)
{
	if (int(NrInstalled()) >= maxAddr)
		throw CException(pasynUserSelf, asynError, __FUNCTION__, "Too many pumps connected=" + std::string(IOPortName));

	// If the new connection fails, the size of the vectors will still have increased.
	// This means only the failing connection will be lost, and not all subsequent connections.
	m_Connections.push_back(NULL);
		
	for (size_t ParamIndex = 0; ParamIndex < size_t(NUM_PARAMS); ParamIndex++)
	{
		if (ParameterDefns[ParamIndex].m_UseCase == NotForReal)
			// Not used by the 'real' IOC
			continue;
		if (ParameterDefns[ParamIndex].m_UseCase == Single)
			// Single instance parameter
			continue;
		// Create parameters from the definitions.
		// These variables end up being addressed as e.g. :1:RUNNING.
		createParam(NrInstalled(), ParamIndex);
	}

	SDeviceConnectionInfo* DeviceConnectionInfo = new SDeviceConnectionInfo();
	DeviceConnectionInfo->m_DeviceAddress = DeviceAddress;
	bool isMaster = false;
	SVQM_800_Error error = m_serviceWrapper->ConnectToDevice(*DeviceConnectionInfo, isMaster);
	setStringParam(NrInstalled(), ParameterDefn::SERIALNUMBER, DeviceConnectionInfo->m_SerialNumber);

	callParamCallbacks(NrInstalled());
	m_Connections.back() = DeviceConnectionInfo;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
//	int CVQM_ITMS_Driver::UsedParams()													//
//																								//
//	Description:																				//
//		Gives a count of how many parameters are required for this IOC.							//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
int CVQM_ITMS_Driver::UsedParams()
{
	int UsedParams = 0;
	for (size_t ParamIndex = 0; ParamIndex < size_t(NUM_PARAMS); ParamIndex++)
	{
		if (ParameterDefns[ParamIndex].m_UseCase == NotForReal)
			// Not implemented, because not meaningful for the simulater.
			continue;
		// But the Single parameter list is required.
		UsedParams++;
	}
	return UsedParams;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
//	asynStatus CVQM_ITMS_Driver::readInt32(asynUser *pasynUser, epicsInt32 *value)		//
//																								//
//	Description:																				//
//		This method is invoked by the Asyn framework to read integer values from the hardware.	//
//		It will normally be called for the FAULT variable which is set to poll once every		//
//		5 seconds.																				//
//																								//
//		However, all the variables are read back from the I/O port in one go because			//
//		that's how the ITMS controller interface works.											//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
asynStatus CVQM_ITMS_Driver::readInt32(asynUser *pasynUser, epicsInt32 *value)
{
	int function = pasynUser->reason;
	int TableIndex;
	asynStatus Status = asynSuccess;
	if (m_Instance == NULL)
		// The IOC is exiting
		return asynTimeout;

	try {
		ThrowException(pasynUser, getAddress(pasynUser, &TableIndex), __FUNCTION__, "Could not get address");
		if ((TableIndex < 0) || (TableIndex > int(NrInstalled())))
			throw CException(pasynUser, asynError, __FUNCTION__, "User / ITMS not configured");

		epicsGuard < epicsMutex > guard ( *(m_Connections[TableIndex]->m_Mutex) );
		if (m_Instance == NULL)
			// The IOC is exiting
			return asynTimeout;
		
	}
	catch(CException const& E) {
		// make sure we return an error state if there are comms problems
		Status = E.Status();
	}
	callParamCallbacks(TableIndex);
	return Status;
}

asynStatus CVQM_ITMS_Driver::readFloat64(asynUser *pasynUser, epicsFloat64 *value)
{
	int function = pasynUser->reason;
	int TableIndex;
	asynStatus Status = asynSuccess;
	if (m_Instance == NULL)
		// The IOC is exiting
		return asynTimeout;

	try {
		ThrowException(pasynUser, getAddress(pasynUser, &TableIndex), __FUNCTION__, "Could not get address");
		if ((TableIndex < 0) || (TableIndex > int(NrInstalled())))
			throw CException(pasynUser, asynError, __FUNCTION__, "User / ITMS not configured");

		epicsGuard < epicsMutex > guard ( *(m_Connections[TableIndex]->m_Mutex) );
		if (m_Instance == NULL)
			// The IOC is exiting
			return asynTimeout;

		if (function == Parameters(ParameterDefn::MULTIPLIERGAIN))
		{
			SVQM_800_Error Error = m_serviceWrapper->GetElectrometerGain(*value, *(m_Connections[TableIndex]));
		}		
	}
	catch(CException const& E) {
		// make sure we return an error state if there are comms problems
		Status = E.Status();
	}
	callParamCallbacks(TableIndex);
	return Status;
}

asynStatus CVQM_ITMS_Driver::readFloat32Array(asynUser *pasynUser, epicsFloat32 *value,
                                        size_t nElements, size_t *nIn)
{
	int function = pasynUser->reason;
	int TableIndex;
	asynStatus Status = asynSuccess;
	if (m_Instance == NULL)
		// The IOC is exiting
		return asynTimeout;
	try {
		ThrowException(pasynUser, getAddress(pasynUser, &TableIndex), __FUNCTION__, "Could not get address");
		if ((TableIndex < 0) || (TableIndex > int(NrInstalled())))
			throw CException(pasynUser, asynError, __FUNCTION__, "User / ITMS not configured");
		epicsGuard < epicsMutex > guard ( *(m_Connections[TableIndex]->m_Mutex) );
		if (m_Instance == NULL)
			// The IOC is exiting
			return asynTimeout;

		if (function == Parameters(ParameterDefn::RAWDATA))
		{
			int lastScanNumber;
			SAnalyzedData analyzedData;
			IHeaderData* headerDataPtr;
			SAverageData averageData;
			bool isValidData;
			EnumGaugeState controllerState;
			SVQM_800_Error Error = m_serviceWrapper->GetScanData(lastScanNumber, analyzedData, &headerDataPtr,
                                            averageData, *(m_Connections[TableIndex]), isValidData, controllerState);

			std::vector<double> const& DenoisedRawData = analyzedData.DenoisedRawData();
			for(size_t Index = 0; Index < DenoisedRawData.size(); Index++)
				value[Index] = float(DenoisedRawData[Index]);
			*nIn = DenoisedRawData.size();
		}
		Status = CVQM_ITMS_Base::readFloat32Array(pasynUser, value, nElements, nIn);
	}
	catch(CException const& E) {
		// make sure we return an error state if there are comms problems
		Status = E.Status();
	}
	callParamCallbacks(TableIndex);
	return Status;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
//	asynStatus CVQM_ITMS_Driver::writeInt32(asynUser *pasynUser, epicsInt32 value)		//
//																								//
//	Description:																				//
//		This method is invoked by the Asyn framework to write values to the hardware.			//
//		There are only 2 parameters that have write behaviour, RUNNING and RESET.				//
//		NB, RUNNING *also* has read-back behaviour.												//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
asynStatus CVQM_ITMS_Driver::writeInt32(asynUser *pasynUser, epicsInt32 value)
{
	if (m_Instance == NULL)
		// The IOC is exiting
		return asynTimeout;

	asynStatus Status = asynSuccess;
	int function = pasynUser->reason;
	// Invoke the base class method to store the value in the database.
	ThrowException(pasynUser, CVQM_ITMS_Base::writeInt32(pasynUser, value), __FUNCTION__, "CVQM_ITMS_Base::writeInt32");
	int TableIndex = 0;
	try {
		ThrowException(pasynUser, getAddress(pasynUser, &TableIndex), __FUNCTION__, "Could not get address");
		if ((TableIndex < 0) || (TableIndex >= int(NrInstalled())))
			throw CException(pasynUser, asynError, __FUNCTION__, "User / ITMS not configured");
		epicsGuard < epicsMutex > guard ( *(m_Connections[TableIndex]->m_Mutex) );
		if (m_Instance == NULL)
			// The IOC is exiting
			return asynTimeout;
		if (function == Parameters(ParameterDefn::FILAMENT))
		{
			double dValue = value;
			SVQM_800_Error Error = m_serviceWrapper->SetLogicalInstrumentVoltageSetpoint(FILAMENT, dValue, *(m_Connections[TableIndex]));
		}
	}
	catch(CException const& E) {
		// make sure we return an error state if there are comms problems
		Status = E.Status();
	}
	return Status;
}
   
asynStatus CVQM_ITMS_Driver::writeFloat64(asynUser *pasynUser, epicsFloat64 value)
{
	if (m_Instance == NULL)
		// The IOC is exiting
		return asynTimeout;

	asynStatus Status = asynSuccess;
	int function = pasynUser->reason;
	// Invoke the base class method to store the value in the database.
	ThrowException(pasynUser, CVQM_ITMS_Base::writeFloat64(pasynUser, value), __FUNCTION__, "CVQM_ITMS_Base::writeFloat64");
	int TableIndex = 0;
	try {
		ThrowException(pasynUser, getAddress(pasynUser, &TableIndex), __FUNCTION__, "Could not get address");
		if ((TableIndex < 0) || (TableIndex >= int(NrInstalled())))
			throw CException(pasynUser, asynError, __FUNCTION__, "User / ITMS not configured");
		epicsGuard < epicsMutex > guard ( *(m_Connections[TableIndex]->m_Mutex) );
		if (m_Instance == NULL)
			// The IOC is exiting
			return asynTimeout;
		if (function == Parameters(ParameterDefn::MULTIPLIERVOLTS))
		{
			SVQM_800_Error Error = m_serviceWrapper->SetLogicalInstrumentVoltageSetpoint(EMBIAS, value, *(m_Connections[TableIndex]));
		}
		if (function == Parameters(ParameterDefn::EMISSION))
		{
			SVQM_800_Error Error = m_serviceWrapper->SetFilamentEmissionCurrentSetpoint(value, *(m_Connections[TableIndex]));
		}

	}
	catch(CException const& E) {
		// make sure we return an error state if there are comms problems
		Status = E.Status();
	}
	return Status;
}

asynStatus CVQM_ITMS_Driver::writeFloat64Array(asynUser *pasynUser, epicsFloat64 *value,
                                        size_t nElements)
{
	if (m_Instance == NULL)
		// The IOC is exiting
		return asynTimeout;

	asynStatus Status = asynSuccess;
	int function = pasynUser->reason;
	// Invoke the base class method to store the value in the database.
	ThrowException(pasynUser, CVQM_ITMS_Base::writeFloat64Array(pasynUser, value, nElements), __FUNCTION__, "CVQM_ITMS_Base::writeFloat64Array");
	int TableIndex = 0;
	try {
		ThrowException(pasynUser, getAddress(pasynUser, &TableIndex), __FUNCTION__, "Could not get address");
		if ((TableIndex < 0) || (TableIndex >= int(NrInstalled())))
			throw CException(pasynUser, asynError, __FUNCTION__, "User / ITMS not configured");
		epicsGuard < epicsMutex > guard ( *(m_Connections[TableIndex]->m_Mutex) );
		if (m_Instance == NULL)
			// The IOC is exiting
			return asynTimeout;
		if (function == Parameters(ParameterDefn::MASSRANGE))
		{
			if ((nElements != 2) || (value[0] >= value[1]))
				throw CException(pasynUser, asynError, __FUNCTION__, "Mass range: from, to");

			SVQM_800_Error Error = m_serviceWrapper->SetScanRange(value[0], value[1], *(m_Connections[TableIndex]));
		}
	}
	catch(CException const& E) {
		// make sure we return an error state if there are comms problems
		Status = E.Status();
	}
	return Status;
}

static const iocshArg initArg0 = { "asynPortName", iocshArgString};
static const iocshArg initArg1 = { "numTraps", iocshArgString};
static const iocshArg * const initArgs[] = {&initArg0, &initArg1};
static const iocshFuncDef initFuncDef = {"VQM_ITMSPortDriverConfigure",3,initArgs};

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
//	void VQM_ITMSExitFunc(void * param)														//
//																								//
//	Description:																				//
//		This function will be invoked when the IOC exits.										//
//		In order to not leak resources, it destroys the object that's been created.				//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
void VQM_ITMSExitFunc(void * param)
{
	CVQM_ITMS_Driver* Instance = static_cast<CVQM_ITMS_Driver*>(param);
	delete Instance;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
//	static void VQM_ITMSPortDriverConfigure(const iocshArgBuf *args)						//
//																								//
//	Description:																				//
//		This function will be invoked when from the st.cmd starup script.						//
//		It creates the object and also schedules the exit event.								//
//																								//
//	Parameters:																					//
//		asynPortName. This is the Asyn port name (e.g. VQM_ITMS).							//
//					This will have been mapped by drvAsynIPPortConfigure to a physical port,	//
//					e.g. COM1: (or /dev/ttyS0).													//
//		numTraps - how many pumps will be attached?												//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
static void VQM_ITMSPortDriverConfigure(const iocshArgBuf *args)
{
	try {
		const char* asynPortName = args[0].sval;
		int numTraps = atoi(args[1].sval);
		CVQM_ITMS_Driver* Instance = new CVQM_ITMS_Driver(asynPortName, numTraps);
		epicsAtExit(VQM_ITMSExitFunc, Instance);
	}
	catch(CVQM_ITMS_Driver::CException const&) {
	}
}

static const iocshArg addArg0 = { "IOPortName", iocshArgString};
static const iocshArg addArg1 = { "DeviceName", iocshArgString};
static const iocshArg * const addArgs[] = {&addArg0, &addArg1};
static const iocshFuncDef addFuncDef = {"VQM_ITMSAddIOPort",2,addArgs};

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
//	static void VQM_ITMSAddIOPort(const iocshArgBuf *args)									//
//	Description:																				//
//		EPICS iocsh callable function to add a (simulated or real) ITMS to the IOC.				//
//																								//
//	Parameters:																					//
//		IOPortName - the IOC port name (e.g. ITMS:1) to be used.								//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
static void VQM_ITMSAddIOPort(const iocshArgBuf *args)
{
	try {
		const char* IOPortName = args[0].sval;
		const char* DeviceName = args[1].sval;
		// Test the driver has been configured
		if (CVQM_ITMS_Driver::Instance())
			CVQM_ITMS_Driver::Instance()->addIOPort(IOPortName, DeviceName);
	}
	catch(CVQM_ITMS_Driver::CException const&) {
		// make sure we return an error state if there are comms problems
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
//	static void VQM_ITMSRegistrar(void)														//
//																								//
//	Description:																				//
//		Registers the functions to be called within the IOC.									//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
static void VQM_ITMSRegistrar(void)
{
    iocshRegister(&initFuncDef, VQM_ITMSPortDriverConfigure);
    iocshRegister(&addFuncDef, VQM_ITMSAddIOPort);
}

extern "C" {

epicsExportRegistrar(VQM_ITMSRegistrar);

}
