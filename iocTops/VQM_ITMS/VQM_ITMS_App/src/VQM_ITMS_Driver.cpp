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
#include <IHeaderData.H>

#define epicsExportSharedSymbols
#include <epicsExport.h>

#include <stdlib.h>
#include <stdexcept>
#include <fstream>
#include <sstream>

#ifndef ASYN_TRACE_WARNING
// Added with asyn4-22
static const int ASYN_TRACE_WARNING = ASYN_TRACE_ERROR;
#endif

CVQM_ITMS_Driver* CVQM_ITMS_Driver::m_Instance;

std::string wcstombs(std::wstring const& WideCharString)
{
	char NBuf[CVQM_ITMS_Base::MaxEPICSStrLen];
	memset(NBuf, 0, sizeof(NBuf));
	::wcstombs(NBuf, WideCharString.c_str(), sizeof(NBuf)-1);
	return NBuf;
}

std::wstring mbstowcs(std::string const& MultiByteString)
{
	wchar_t WBuf[CVQM_ITMS_Base::MaxEPICSStrLen];
	memset(WBuf, 0, sizeof(WBuf));
	::mbstowcs(WBuf, MultiByteString.c_str(), (sizeof(WBuf) - sizeof(wchar_t)) / sizeof(wchar_t));
	return WBuf;
}

CVQM_ITMS_Driver::CException::CException(asynUser* AsynUser, SVQM_800_Error const& Error, const char* functionName) :
	CVQM_ITMS_Base::CException(AsynUser, asynError, functionName, wcstombs(Error.m_ErrorString)) {
			std::string message = "%s:%s ERROR: " + std::string(what()) + "%s\n";
			asynPrint(AsynUser, ASYN_TRACE_ERROR, message.c_str(), __FILE__, functionName, AsynUser->errorMessage);
		}
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
	ThrowException(m_serviceWrapper->GetValidConnections(m_nConnections, m_Connections), __FUNCTION__);
	m_Instance = this;
}

CVQM_ITMS_Driver::~CVQM_ITMS_Driver()
{
	m_Instance = NULL;
	disconnect();
	delete[] m_Connections;
	CLIWrapperDLL::ReleaseServiceHandle(m_serviceWrapper);
}

void CVQM_ITMS_Driver::disconnect()
{
	asynStatus overallstatus = asynSuccess;
	for(size_t Index = 0; Index < NrInstalled(); Index++)
		m_Mutexes[Index]->lock();
	for(size_t Index = 0; Index < NrInstalled(); Index++)
	{
		bool isMaster = (m_Connections[Index].m_Availability == AVAILABLE);
		if (isMaster)
			SetGaugeState(Index, false, false);
		ThrowException(m_serviceWrapper->DisconnectFromDevice(m_Connections[Index]), __FUNCTION__, false);
	}
	for(size_t Index = 0; Index < NrInstalled(); Index++)
		m_Mutexes[Index]->unlock();
	for(size_t Index = 0; Index < NrInstalled(); Index++)
		delete m_Mutexes[Index];
}

void CVQM_ITMS_Driver::ThrowException(SVQM_800_Error const& Error, const char* Function, bool ThrowIt /*= true*/)
{
	if (Error.m_ErrorType == NO_ERRORS)
		return;
	if (ThrowIt)
		throw CException(pasynUserSelf, Error, Function);
	else
		asynPrint(pasynUserSelf, ASYN_TRACE_ERROR, wcstombs(Error.m_ErrorString).c_str(), __FILE__, Function);

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
void CVQM_ITMS_Driver::addIOPort(const char* DeviceAddress)
{
	if (int(NrInstalled()) >= maxAddr)
		throw CVQM_ITMS_Base::CException(pasynUserSelf, asynError, __FUNCTION__, "Too many pumps connected=" + std::string(DeviceAddress));

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

	callParamCallbacks(NrInstalled());

	int NewConnection = -1;
	std::wstring DeviceAddressW = mbstowcs(DeviceAddress);
	for (int Connection = 0; Connection < m_nConnections; Connection++)
		if (m_Connections[Connection].m_DeviceAddress == DeviceAddressW)
		{
			NewConnection = Connection;
			break;
		}
	if (NewConnection == -1)
		CVQM_ITMS_Base::ThrowException(pasynUserSelf, asynError, __FUNCTION__, "Could not connect " + std::string(DeviceAddress));
	m_ConnectionMap[NrInstalled()] = NewConnection;

	bool isMaster = (m_Connections[NewConnection].m_Availability == AVAILABLE);
	ThrowException(m_serviceWrapper->ConnectToDevice(m_Connections[NewConnection], isMaster), __FUNCTION__);
	if (isMaster)
	{
		ThrowException(m_serviceWrapper->DataAnalysisSetAvgMode(Running_Avg, m_Connections[NewConnection]), __FUNCTION__);
		// 100 / 85 gives 1 scan / sec
		ThrowException(m_serviceWrapper->DataAnalysisSetNumAvgs(11, m_Connections[NewConnection]), __FUNCTION__);
		SetGaugeState(NewConnection, true);
		setIntegerParam(NrInstalled(), ParameterDefn::SCANNING, 1);
	}
	else
	{
		setIntegerParam(NrInstalled(), ParameterDefn::SCANNING, GetGaugeState(NewConnection));
	}

	// If the new connection fails, the size of the vectors will still have increased.
	// This means only the failing connection will be lost, and not all subsequent connections.
	m_Mutexes.push_back(new epicsMutex());
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
			// Not implemented, because only meaningful for the simulater.
			continue;
		// But the Single parameter list is required.
		UsedParams++;
	}
	return UsedParams;
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
		CVQM_ITMS_Base::ThrowException(pasynUser, getAddress(pasynUser, &TableIndex), __FUNCTION__, "Could not get address");
		if ((TableIndex < 0) || (TableIndex > int(NrInstalled())))
			throw CVQM_ITMS_Base::CException(pasynUser, asynError, __FUNCTION__, "User / ITMS not configured");
		epicsGuard < epicsMutex > guard (*m_Mutexes[TableIndex]);
		if (m_Instance == NULL)
			// The IOC is exiting
			return asynTimeout;

		int Connection = m_ConnectionMap.find(TableIndex)->second;

		if (function == Parameters(ParameterDefn::EMISSION))
		{
			ThrowException(m_serviceWrapper->GetFilamentEmissionCurrent(*value, m_Connections[Connection]), __FUNCTION__);
		}
		else if (function == Parameters(ParameterDefn::FILAMENTBIAS))
		{
			ThrowException(m_serviceWrapper->GetLogicalInstrumentCurrentVoltage(*value, FILAMENTBIAS, m_Connections[Connection]), __FUNCTION__);
		}
		else if (function == Parameters(ParameterDefn::REPELLERBIAS))
		{
			ThrowException(m_serviceWrapper->GetLogicalInstrumentCurrentVoltage(*value, REPELLERBIAS, m_Connections[Connection]), __FUNCTION__);
		}
		else if (function == Parameters(ParameterDefn::ENTRYPLATE))
		{
			ThrowException(m_serviceWrapper->GetLogicalInstrumentCurrentVoltage(*value, ENTRYPLATE, m_Connections[Connection]), __FUNCTION__);
		}
		else if (function == Parameters(ParameterDefn::PRESSUREPLATE))
		{
			ThrowException(m_serviceWrapper->GetLogicalInstrumentCurrentVoltage(*value, PRESSUREPLATE, m_Connections[Connection]), __FUNCTION__);
		}
		else if (function == Parameters(ParameterDefn::CUPS))
		{
			ThrowException(m_serviceWrapper->GetLogicalInstrumentCurrentVoltage(*value, CUPS, m_Connections[Connection]), __FUNCTION__);
		}
		else if (function == Parameters(ParameterDefn::TRANSITION))
		{
			ThrowException(m_serviceWrapper->GetLogicalInstrumentCurrentVoltage(*value, TRANSITION, m_Connections[Connection]), __FUNCTION__);
		}
		else if (function == Parameters(ParameterDefn::EXITPLATE))
		{
			ThrowException(m_serviceWrapper->GetLogicalInstrumentCurrentVoltage(*value, EXITPLATE, m_Connections[Connection]), __FUNCTION__);
		}
		else if (function == Parameters(ParameterDefn::EMSHIELD))
		{
			ThrowException(m_serviceWrapper->GetLogicalInstrumentCurrentVoltage(*value, EMSHIELD, m_Connections[Connection]), __FUNCTION__);
		}
		else if (function == Parameters(ParameterDefn::EMBIAS))
		{
			ThrowException(m_serviceWrapper->GetLogicalInstrumentCurrentVoltage(*value, EMBIAS, m_Connections[Connection]), __FUNCTION__);
		}
		else if (function == Parameters(ParameterDefn::RFAMP))
		{
			ThrowException(m_serviceWrapper->GetLogicalInstrumentCurrentVoltage(*value, RFAMP, m_Connections[Connection]), __FUNCTION__);
		}
		else if (function == Parameters(ParameterDefn::MASSCAL))
		{
			float MassCalibrationFactor;
			ThrowException(m_serviceWrapper->GetMassCalibrationFactor(MassCalibrationFactor, m_Connections[Connection]), __FUNCTION__); 
			*value = MassCalibrationFactor;
		}
		else if (function == Parameters(ParameterDefn::ELECTROMETERGAIN))
		{
			ThrowException(m_serviceWrapper->GetElectrometerGain(*value, m_Connections[Connection]), __FUNCTION__); 
		}
		else if (function == Parameters(ParameterDefn::MASSFROM))
		{
			double ToValue;
			ThrowException(m_serviceWrapper->GetScanRange(*value, ToValue, m_Connections[Connection]), __FUNCTION__);
			setDoubleParam(ParameterDefn::MASSTO, ToValue);
		}
		else if (function == Parameters(ParameterDefn::MASSTO))
		{
			double FromValue;
			ThrowException(m_serviceWrapper->GetScanRange(FromValue, *value, m_Connections[Connection]), __FUNCTION__);
			setDoubleParam(ParameterDefn::MASSFROM, FromValue);
		}
		else
			Status = CVQM_ITMS_Base::readFloat64(pasynUser, value);
	}
	catch(CVQM_ITMS_Base::CException const& E) {
		// make sure we return an error state if there are comms problems
		Status = ErrorHandler(TableIndex, E);
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
		CVQM_ITMS_Base::ThrowException(pasynUser, getAddress(pasynUser, &TableIndex), __FUNCTION__, "Could not get address");
		if ((TableIndex < 0) || (TableIndex > int(NrInstalled())))
			throw CVQM_ITMS_Base::CException(pasynUser, asynError, __FUNCTION__, "User / ITMS not configured");
		epicsGuard < epicsMutex > guard (*m_Mutexes[TableIndex]);
		if (m_Instance == NULL)
			// The IOC is exiting
			return asynTimeout;

		if (getIntegerParam(TableIndex, ParameterDefn::SCANNING) == 0)
			return asynSuccess;

		int Connection = m_ConnectionMap.find(TableIndex)->second;
		if (function == Parameters(ParameterDefn::RAWDATA))
		{
			SAnalyzedData analyzedData;
			IHeaderData* headerDataPtr;
			SAverageData averageData;
			bool isValidData;
			EnumGaugeState controllerState;
			int LastScanNumber;
			ThrowException(m_serviceWrapper->GetScanData(LastScanNumber, analyzedData, &headerDataPtr,
                                            averageData, m_Connections[Connection], isValidData, controllerState), __FUNCTION__);
			setIntegerParam(TableIndex, ParameterDefn::LASTSCANNUMBER, LastScanNumber);
			setDoubleParam(TableIndex, ParameterDefn::EMISSION, headerDataPtr->EmissionCurrent());
			setDoubleParam(TableIndex, ParameterDefn::FILAMENTBIAS, headerDataPtr->FilamentBiasVoltage());
			setDoubleParam(TableIndex, ParameterDefn::REPELLERBIAS, headerDataPtr->RepellerVoltage());
			setDoubleParam(TableIndex, ParameterDefn::ENTRYPLATE, headerDataPtr->EntryPlateVoltage());
			setDoubleParam(TableIndex, ParameterDefn::PRESSUREPLATE, headerDataPtr->PressurePlateVoltage());
			setDoubleParam(TableIndex, ParameterDefn::CUPS, headerDataPtr->CupsVoltage());
			setDoubleParam(TableIndex, ParameterDefn::TRANSITION, headerDataPtr->TransitionVoltage());
			setDoubleParam(TableIndex, ParameterDefn::EXITPLATE, headerDataPtr->ExitPlateVoltage());
			setDoubleParam(TableIndex, ParameterDefn::EMSHIELD, headerDataPtr->ElectronMultiplierShieldVoltage());
			setDoubleParam(TableIndex, ParameterDefn::EMBIAS, headerDataPtr->ElectronMultiplierVoltage());
			setDoubleParam(TableIndex, ParameterDefn::RFAMP, headerDataPtr->DDSAmplitude());
			setDoubleParam(TableIndex, ParameterDefn::ELECTROMETERGAIN, headerDataPtr->ElectronMultiplierElectrometerGain());
			setDoubleParam(TableIndex, ParameterDefn::MASSCAL, headerDataPtr->MassAxisCalibrationFactor());
			setStringParam(TableIndex, ParameterDefn::FIRMWAREVERSION, wcstombs(headerDataPtr->FirmwareRevision()));
			setStringParam(TableIndex, ParameterDefn::HARDWAREVERSION, wcstombs(headerDataPtr->HardwareRevision()));

			const IMassAxis* MassAxis = headerDataPtr->MassAxis();
			if (MassAxis)
			{
				setDoubleParam(TableIndex, ParameterDefn::MASSFROM, MassAxis->BeginAMU());
				setDoubleParam(TableIndex, ParameterDefn::MASSTO, MassAxis->EndAMU());
			}

			std::vector<float> PeakArea(analyzedData.PeakArea().size());
			for(size_t Index = 0; Index < analyzedData.PeakArea().size(); Index++)
				PeakArea[Index] = float(analyzedData.PeakArea()[Index]);
			doCallbacksFloat32Array(PeakArea, ParameterDefn::PEAKAREA, TableIndex);

			std::vector<double> const& DenoisedRawData = analyzedData.DenoisedRawData();
			size_t ArraySize = DenoisedRawData.size();
			if (ArraySize != nElements)
			{
				asynPrint(pasynUser, ASYN_TRACE_WARNING, "Raw array size disrcepant % instead of %", ArraySize, nElements);
				ArraySize = __min(ArraySize, nElements);
			}
			for(size_t Index = 0; Index < ArraySize; Index++)
				value[Index] = float(DenoisedRawData[Index]);
			*nIn = ArraySize;
		}
	}
	catch(CVQM_ITMS_Base::CException const& E) {
		// make sure we return an error state if there are comms problems
		Status = ErrorHandler(TableIndex, E);
	}
	callParamCallbacks(TableIndex);
	return Status;
}

asynStatus CVQM_ITMS_Driver::ErrorHandler(int TableIndex, CVQM_ITMS_Base::CException const& E)
{
	if (m_Instance == NULL)
		// The IOC is exiting
		return asynTimeout;

	// Internal communication failure
	// NB, Asyn 4-27 requires that the parameter status is success before the value can be set through callback.
	setParamStatus(TableIndex, ParameterDefn::FAULT, asynSuccess, false);
	callParamCallbacks(TableIndex, false);

	setParamStatus(TableIndex, ParameterDefn::FAULT, E.Status(), false);
	return E.Status();
}

void CVQM_ITMS_Driver::SetGaugeState(int Connection, bool Scanning, bool ThrowIt /*= true */)
{
	ThrowException(m_serviceWrapper->SetGaugeState(Scanning ? EnumGaugeState_SCAN : EnumGaugeState_OFF, m_Connections[Connection]), __FUNCTION__, ThrowIt);
}

bool CVQM_ITMS_Driver::GetGaugeState(int Connection)
{
	EnumGaugeState gaugeState;
	ThrowException(m_serviceWrapper->GetGaugeState(gaugeState, m_Connections[Connection]), __FUNCTION__);
	return (gaugeState == EnumGaugeState_SCAN);
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
	CVQM_ITMS_Base::ThrowException(pasynUser, CVQM_ITMS_Base::writeInt32(pasynUser, value), __FUNCTION__, "CVQM_ITMS_Base::writeInt32");
	int TableIndex = 0;
	try {
		CVQM_ITMS_Base::ThrowException(pasynUser, getAddress(pasynUser, &TableIndex), __FUNCTION__, "Could not get address");
		if ((TableIndex < 0) || (TableIndex >= int(NrInstalled())))
			throw CVQM_ITMS_Base::CException(pasynUser, asynError, __FUNCTION__, "User / ITMS not configured");
		epicsGuard < epicsMutex > guard (*m_Mutexes[TableIndex]);
		if (m_Instance == NULL)
			// The IOC is exiting
			return asynTimeout;

		int Connection = m_ConnectionMap.find(TableIndex)->second;
		if (function == Parameters(ParameterDefn::SCANNING))
			SetGaugeState(Connection, value != 0);
		if (function == Parameters(ParameterDefn::AVERAGES))
			ThrowException(m_serviceWrapper->DataAnalysisSetNumAvgs(value, m_Connections[Connection]), __FUNCTION__);
	}
	catch(CVQM_ITMS_Base::CException const& E) {
		// make sure we return an error state if there are comms problems
		Status = ErrorHandler(TableIndex, E);
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
	CVQM_ITMS_Base::ThrowException(pasynUser, CVQM_ITMS_Base::writeFloat64(pasynUser, value), __FUNCTION__, "CVQM_ITMS_Base::writeFloat64");
	int TableIndex = 0;
	try {
		CVQM_ITMS_Base::ThrowException(pasynUser, getAddress(pasynUser, &TableIndex), __FUNCTION__, "Could not get address");
		if ((TableIndex < 0) || (TableIndex >= int(NrInstalled())))
			throw CVQM_ITMS_Base::CException(pasynUser, asynError, __FUNCTION__, "User / ITMS not configured");
		epicsGuard < epicsMutex > guard (*m_Mutexes[TableIndex]);
		if (m_Instance == NULL)
			// The IOC is exiting
			return asynTimeout;
		int Connection = m_ConnectionMap.find(TableIndex)->second;

		if (function == Parameters(ParameterDefn::EMISSION))
		{
			ThrowException(m_serviceWrapper->SetFilamentEmissionCurrentSetpoint(value, m_Connections[Connection]), __FUNCTION__);
		}
		else if (function == Parameters(ParameterDefn::FILAMENTBIAS))
		{
			ThrowException(m_serviceWrapper->SetLogicalInstrumentVoltageSetpoint(FILAMENTBIAS, value, m_Connections[Connection]), __FUNCTION__);
		}
		else if (function == Parameters(ParameterDefn::REPELLERBIAS))
		{
			ThrowException(m_serviceWrapper->SetLogicalInstrumentVoltageSetpoint(REPELLERBIAS, value, m_Connections[Connection]), __FUNCTION__);
		}
		else if (function == Parameters(ParameterDefn::ENTRYPLATE))
		{
			ThrowException(m_serviceWrapper->SetLogicalInstrumentVoltageSetpoint(ENTRYPLATE, value, m_Connections[Connection]), __FUNCTION__);
		}
		else if (function == Parameters(ParameterDefn::PRESSUREPLATE))
		{
			ThrowException(m_serviceWrapper->SetLogicalInstrumentVoltageSetpoint(PRESSUREPLATE, value, m_Connections[Connection]), __FUNCTION__);
		}
		else if (function == Parameters(ParameterDefn::CUPS))
		{
			ThrowException(m_serviceWrapper->SetLogicalInstrumentVoltageSetpoint(CUPS, value, m_Connections[Connection]), __FUNCTION__);
		}
		else if (function == Parameters(ParameterDefn::TRANSITION))
		{
			ThrowException(m_serviceWrapper->SetLogicalInstrumentVoltageSetpoint(TRANSITION, value, m_Connections[Connection]), __FUNCTION__);
		}
		else if (function == Parameters(ParameterDefn::EXITPLATE))
		{
			ThrowException(m_serviceWrapper->SetLogicalInstrumentVoltageSetpoint(EXITPLATE, value, m_Connections[Connection]), __FUNCTION__);
		}
		else if (function == Parameters(ParameterDefn::EMSHIELD))
		{
			ThrowException(m_serviceWrapper->SetLogicalInstrumentVoltageSetpoint(EMSHIELD, value, m_Connections[Connection]), __FUNCTION__);
		}
		else if (function == Parameters(ParameterDefn::EMBIAS))
		{
			ThrowException(m_serviceWrapper->SetLogicalInstrumentVoltageSetpoint(EMBIAS, value, m_Connections[Connection]), __FUNCTION__);
		}
		else if (function == Parameters(ParameterDefn::RFAMP))
		{
			ThrowException(m_serviceWrapper->SetLogicalInstrumentVoltageSetpoint(RFAMP, value, m_Connections[Connection]), __FUNCTION__);
		}
		else if (function == Parameters(ParameterDefn::MASSFROM))
		{
			double ToValue = getDoubleParam(TableIndex, ParameterDefn::MASSTO);
			ThrowException(m_serviceWrapper->SetScanRange(value, ToValue, m_Connections[Connection]), __FUNCTION__);
		}
		else if (function == Parameters(ParameterDefn::MASSTO))
		{
			double FromValue = getDoubleParam(TableIndex, ParameterDefn::MASSFROM);
			ThrowException(m_serviceWrapper->SetScanRange(FromValue, value, m_Connections[Connection]), __FUNCTION__);
		}
	}
	catch(CVQM_ITMS_Base::CException const& E) {
		// make sure we return an error state if there are comms problems
		Status = ErrorHandler(TableIndex, E);
	}
	return Status;
}

static const iocshArg initArg0 = { "asynPortName", iocshArgString};
static const iocshArg initArg1 = { "numTraps", iocshArgString};
static const iocshArg * const initArgs[] = {&initArg0, &initArg1};
static const iocshFuncDef initFuncDef = {"VQM_ITMSPortDriverConfigure",2,initArgs};

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

static const iocshArg addArg0 = { "DeviceName", iocshArgString};
static const iocshArg * const addArgs[] = {&addArg0};
static const iocshFuncDef addFuncDef = {"VQM_ITMSAddIOPort",1,addArgs};

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
		const char* DeviceName = args[0].sval;
		// Test the driver has been configured
		if (CVQM_ITMS_Driver::Instance())
			CVQM_ITMS_Driver::Instance()->addIOPort(DeviceName);
	}
	catch(CVQM_ITMS_Driver::CException const& E) {
		// make sure we return an error state if there are comms problems
		if (CVQM_ITMS_Driver::Instance())
			CVQM_ITMS_Driver::Instance()->ErrorHandler(int(CVQM_ITMS_Driver::Instance()->NrInstalled()), E);
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
