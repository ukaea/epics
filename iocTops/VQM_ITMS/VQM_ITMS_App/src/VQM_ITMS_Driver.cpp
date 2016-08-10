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
#include <IHeaderData.H>
#include <SVQM_800_Error.h>
#include "VQM/Include/SAnalyzedData.h"
#include "VQM/Include/SAverageData.h"

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

CVQM_ITMS_Driver::CException::CException(asynUser* AsynUser, SVQM_800_Error const& Error, const char* functionName) :
	::CException(AsynUser, asynError, functionName, wcstombs(Error.m_ErrorString)) 
{
	std::string message = "%s:%s ERROR: " + std::string(what()) + "%s\n";
	asynPrint(AsynUser, ASYN_TRACE_ERROR, message.c_str(), __FILE__, functionName, AsynUser->errorMessage);
}

CVQM_ITMS_Driver::CException::CException(asynUser* AsynUser, asynStatus Status, const char* functionName, std::string const& what) :
	::CException(AsynUser, asynError, functionName, what) 
{
		std::string message = "%s:%s ERROR: " + std::string(what) + "%s\n";
		asynPrint(AsynUser, ASYN_TRACE_ERROR, message.c_str(), __FILE__, functionName, AsynUser->errorMessage);
}

class CVQM_ITMS_Driver::CThreadRunable : public epicsThreadRunable
{
	public:
		CThreadRunable(CVQM_ITMS_Driver* This, const char* DeviceAddress, int Connection)
		{
			m_This = This;
			m_Connection = Connection;
			m_Exiting = false;
			m_Started = false;
			m_Thread = new epicsThread(*this, DeviceAddress, epicsThreadGetStackSize(epicsThreadStackSmall));
		}
		~CThreadRunable() {
			delete m_Thread;
		}

		virtual void run () {
			try {
				while (!m_Exiting)
					if (m_This->GetScanData(m_Connection, m_RawData, m_PartialPressure))
						m_This->callParamCallbacks(m_Connection, m_RawData, m_PartialPressure);
			}
			catch(::CException const& E) {
				// make sure we return an error state if there are comms problems
				m_This->ErrorHandler(m_Connection, E);
			}
		}
		void setExiting() {
			m_Exiting = true;
		}
		void setRawDataSize(size_t RawDataSize) {
			m_RawData.assign(RawDataSize, 0);
			if (m_PartialPressure.size() != 0)
				start();
		}
		size_t RawDataSize() const {
			return m_RawData.size();
		}
		void setPartialPressureSize(size_t PartialPressureSize) {
			m_PartialPressure.assign(PartialPressureSize, 0);
			if (m_RawData.size() != 0)
				start();
		}
		size_t PartialPressureSize() const {
			return m_PartialPressure.size();
		}
		void lock() {
			m_Mutex.lock();
		}
		void unlock() {
			m_Mutex.unlock();
		}
		void start() {
			if (!m_Started)
			{
				m_Started = true;
				m_Thread->start();
			}
		}
		operator epicsMutex&() {
			return m_Mutex;
		}
	private:
		CVQM_ITMS_Driver* m_This;
		epicsThread* m_Thread;
		epicsMutex m_Mutex;
		int m_Connection;
		volatile bool m_Started;
		volatile bool m_Exiting;		// Signals the listening thread to exit.
		std::vector<float> m_PartialPressure;
		std::vector<float> m_RawData;
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
	m_Instance = this;
#ifdef BUILD_WITH_SDK
	m_serviceWrapper = CLIWrapperDLL::CreateServiceHandle();
	ThrowException(m_serviceWrapper->GetValidConnections(m_nConnections, m_Connections), __FUNCTION__);
#else
	m_serviceWrapper = new IServiceWrapper();
#endif
}

CVQM_ITMS_Driver::~CVQM_ITMS_Driver()
{
	m_Instance = NULL;
	disconnect();
}

void CVQM_ITMS_Driver::disconnect()
{
	asynStatus overallstatus = asynSuccess;
	for(size_t Index = 0; Index < NrInstalled(); Index++)
		m_Threads[Index]->setExiting();
	for(size_t Index = 0; Index < NrInstalled(); Index++)
		m_Threads[Index]->lock();
	for(size_t Index = 0; Index < NrInstalled(); Index++)
	{
		SetGaugeState(Index, EnumGaugeState_OFF, false);
#ifndef BUILD_WITH_SDK
		asynStatus status = pasynOctetSyncIO->disconnect(m_Connections[Index]);
		if (overallstatus == asynSuccess)
			overallstatus = status;
#endif
	}
	for(size_t Index = 0; Index < NrInstalled(); Index++)
		m_Threads[Index]->unlock();
	for(size_t Index = 0; Index < NrInstalled(); Index++)
		delete m_Threads[Index];
	delete m_serviceWrapper;
}

void CVQM_ITMS_Driver::ThrowException(asynUser* pasynUser, asynStatus Status, const char* Function, std::string const& what, bool ThrowIt) const
{
	if (Status == asynSuccess)
		return;
	if (ThrowIt)
		throw CException(pasynUserSelf, Status, Function, what);
	else
		asynPrint(pasynUserSelf, ASYN_TRACE_ERROR, what.c_str(), __FILE__, Function);

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
		throw CException(pasynUserSelf, asynError, __FUNCTION__, "Too many pumps connected=" + std::string(DeviceAddress));

	for (size_t ParamIndex = 0; ParamIndex < size_t(NUM_PARAMS); ParamIndex++)
	{
		if (ParameterDefns[ParamIndex].m_UseCase == Single)
			// Single instance parameter
			continue;
		// Create parameters from the definitions.
		// These variables end up being addressed as e.g. :1:RUNNING.
		createParam(NrInstalled(), ParamIndex);
	}

#ifdef BUILD_WITH_SDK
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
#else
	bool isMaster = true;
	int NewConnection = NrInstalled();
	asynUser* IOUser;
	// Connect to the I/O port.
	ThrowException(pasynUserSelf, pasynOctetSyncIO->connect(DeviceAddress, int(NrInstalled()), &IOUser, NULL), __FUNCTION__, "connecting to IO port=" + std::string(DeviceAddress));
	m_Connections.push_back(IOUser);
#endif
	ThrowException(m_serviceWrapper->ConnectToDevice(m_Connections[NewConnection], isMaster), __FUNCTION__);
	// TODO
	bool isExtendedRange = false;
	ThrowException(m_serviceWrapper->GetExtendedRangeCapabilities(isExtendedRange, m_Connections[NewConnection]), __FUNCTION__);

	CVQM_ITMS_Base::callParamCallbacks(NrInstalled());
	// If the new connection fails, the size of the vectors will still have increased.
	// This means only the failing connection will be lost, and not all subsequent connections.
	m_Threads.push_back(new CThreadRunable(this, DeviceAddress, m_Threads.size()));
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
		// But the Single parameter list is required.
		UsedParams++;
	}
	return UsedParams;
}

bool CVQM_ITMS_Driver::GetScanData(int Connection, std::vector<float>& RawData, std::vector<float>& PartialPressure)
{
	if (m_Instance == NULL)
		// The IOC is exiting
		return false;

	if ((Connection < 0) || (Connection >= int(NrInstalled())))
		throw CException(pasynUserSelf, asynError, __FUNCTION__, "User / ITMS not configured");

	epicsGuard < epicsMutex > guard (*m_Threads[Connection]);
	if (m_Instance == NULL)
		// The IOC is exiting
		return false;

//	if (getIntegerParam(Connection, ParameterDefn::SCANNING) == 0)
//		return false;

	IHeaderData const* headerDataPtr;
	bool isValidData;
	EnumGaugeState controllerState;
	int LastScanNumber;
	SAnalyzedData AnalyzedData;
	SAverageData AverageData;
	ThrowException(m_serviceWrapper->GetScanData(LastScanNumber, AnalyzedData, const_cast<IHeaderData**>(&headerDataPtr),
		AverageData, m_Connections[Connection], isValidData, controllerState), __FUNCTION__, "GetScanData");

	setIntegerParam(Connection, ParameterDefn::SCANNING, controllerState);
	setIntegerParam(Connection, ParameterDefn::AVERAGES, AverageData.BuffersAverage());
	setIntegerParam(Connection, ParameterDefn::AVERAGEMODE, AverageData.AveragingModeInput());
	setIntegerParam(Connection, ParameterDefn::LASTSCANNUMBER, LastScanNumber);
	setDoubleParam(Connection, ParameterDefn::EMISSION, headerDataPtr->EmissionCurrent());
	setDoubleParam(Connection, ParameterDefn::FILAMENTBIAS, headerDataPtr->FilamentBiasVoltage());
	setDoubleParam(Connection, ParameterDefn::FILAMENTPOWER, headerDataPtr->FilamentPower());
	setDoubleParam(Connection, ParameterDefn::REPELLERBIAS, headerDataPtr->RepellerVoltage());
	setDoubleParam(Connection, ParameterDefn::ENTRYPLATE, headerDataPtr->EntryPlateVoltage());
	setDoubleParam(Connection, ParameterDefn::PRESSUREPLATE, headerDataPtr->PressurePlateVoltage());
	setDoubleParam(Connection, ParameterDefn::CUPS, headerDataPtr->CupsVoltage());
	setDoubleParam(Connection, ParameterDefn::TRANSITION, headerDataPtr->TransitionVoltage());
	setDoubleParam(Connection, ParameterDefn::EXITPLATE, headerDataPtr->ExitPlateVoltage());
	setDoubleParam(Connection, ParameterDefn::EMSHIELD, headerDataPtr->ElectronMultiplierShieldVoltage());
	setDoubleParam(Connection, ParameterDefn::EMBIAS, headerDataPtr->ElectronMultiplierVoltage());
	setDoubleParam(Connection, ParameterDefn::RFAMP, headerDataPtr->DDSAmplitude());
	setDoubleParam(Connection, ParameterDefn::MASSCAL, headerDataPtr->MassAxisCalibrationFactor());
	setDoubleParam(Connection, ParameterDefn::ELECTROMETERGAIN, headerDataPtr->ElectronMultiplierElectrometerGain());
	setStringParam(Connection, ParameterDefn::FIRMWAREVERSION, wcstombs(headerDataPtr->FirmwareRevision()));
	setStringParam(Connection, ParameterDefn::HARDWAREVERSION, wcstombs(headerDataPtr->HardwareRevision()));
	setStringParam(Connection, ParameterDefn::MACHINEID, wcstombs(headerDataPtr->MID()));
	setDoubleParam(Connection, ParameterDefn::TOTALPRESSURE, headerDataPtr->TotalPressure());
	double lowerRange, upperRange;
	ThrowException(m_serviceWrapper->GetScanRange(lowerRange, upperRange, m_Connections[Connection]), __FUNCTION__, "GetScanRange");
	setDoubleParam(Connection, ParameterDefn::MASSFROM, lowerRange);
	setDoubleParam(Connection, ParameterDefn::MASSTO, upperRange);
	/*	INoiseData const* NoiseDataPtr = headerDataPtr->NoiseData();
	int NoiseSamples = NoiseDataPtr->NSamples();
	std::vector<float> NoiseData(NoiseSamples);*/

	if (AnalyzedData.PeakArea().size() < PartialPressure.size())
		PartialPressure.resize(AnalyzedData.PeakArea().size());
	for(size_t Index = 0; Index < PartialPressure.size(); Index++)
		PartialPressure[Index] = float(AnalyzedData.PeakArea()[Index]);

	if (AnalyzedData.DenoisedRawData().size() < RawData.size())
		RawData.resize(AnalyzedData.DenoisedRawData().size());
	for(size_t Index = 0; Index < RawData.size(); Index++)
		RawData[Index] = float(AnalyzedData.DenoisedRawData()[Index]);
	return true;
}

void CVQM_ITMS_Driver::callParamCallbacks(size_t Connection, std::vector<float>& RawData, std::vector<float>& PartialPressure)
{
	CVQM_ITMS_Base::callParamCallbacks(Connection);
	if (PartialPressure.size() > 0)
		doCallbacksFloat32Array(PartialPressure, ParameterDefn::PARTIALPRESSURE, Connection);
	if (RawData.size() > 0)
		doCallbacksFloat32Array(RawData, ParameterDefn::RAWDATA, Connection);
}

asynStatus CVQM_ITMS_Driver::readFloat32Array(asynUser *pasynUser, epicsFloat32 *value,
                                        size_t nElements, size_t *nIn)
{
	int function = pasynUser->reason;
	int Connection;
	asynStatus Status = asynSuccess;
	if (m_Instance == NULL)
		// The IOC is exiting
		return asynTimeout;
	try {
		CVQM_ITMS_Base::ThrowException(pasynUser, getAddress(pasynUser, &Connection), __FUNCTION__, "Could not get address");
		if ((Connection < 0) || (Connection >= int(NrInstalled())))
			throw CException(pasynUser, asynError, __FUNCTION__, "User / ITMS not configured");
		epicsGuard < epicsMutex > guard (*m_Threads[Connection]);
		if (m_Instance == NULL)
			// The IOC is exiting
			return asynTimeout;

		if (function == Parameters(ParameterDefn::RAWDATA))
		{
			m_Threads[Connection]->setRawDataSize(nElements);
			*nIn = m_Threads[Connection]->RawDataSize();
		}
		if (function == Parameters(ParameterDefn::PARTIALPRESSURE))
		{
			m_Threads[Connection]->setPartialPressureSize(nElements);
			*nIn = m_Threads[Connection]->PartialPressureSize();
		}
	}
	catch(::CException const& E) {
		// make sure we return an error state if there are comms problems
		Status = ErrorHandler(Connection, E);
	}
	return Status;
}

asynStatus CVQM_ITMS_Driver::ErrorHandler(int Connection, ::CException const& E)
{
	if (m_Instance == NULL)
		// The IOC is exiting
		return asynTimeout;

	// Internal communication failure
	// NB, Asyn 4-27 requires that the parameter status is success before the value can be set through callback.
	setParamStatus(Connection, ParameterDefn::FAULT, asynSuccess, false);
	CVQM_ITMS_Base::callParamCallbacks(Connection, false);

	setParamStatus(Connection, ParameterDefn::FAULT, E.Status(), false);
	return E.Status();
}

void CVQM_ITMS_Driver::SetGaugeState(int Connection, int GaugeState, bool ThrowIt /*= true */)
{
	ThrowException(m_serviceWrapper->SetGaugeState(EnumGaugeState(GaugeState), m_Connections[Connection]), __FUNCTION__, ThrowIt);
}

int CVQM_ITMS_Driver::GetGaugeState(int Connection)
{
	EnumGaugeState gaugeState;
	ThrowException(m_serviceWrapper->GetGaugeState(gaugeState, m_Connections[Connection]), __FUNCTION__, "GetGaugeState");
	return gaugeState;
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
	int Connection = 0;
	try {
		CVQM_ITMS_Base::ThrowException(pasynUser, getAddress(pasynUser, &Connection), __FUNCTION__, "Could not get address");
		if ((Connection < 0) || (Connection >= int(NrInstalled())))
			throw CException(pasynUser, asynError, __FUNCTION__, "User / ITMS not configured");
		epicsGuard < epicsMutex > guard (*m_Threads[Connection]);
		if (m_Instance == NULL)
			// The IOC is exiting
			return asynTimeout;

		if (function == Parameters(ParameterDefn::SCANNING))
			SetGaugeState(Connection, value);
		else if (function == Parameters(ParameterDefn::AVERAGES))
			ThrowException(m_serviceWrapper->DataAnalysisSetNumAvgs(value, m_Connections[Connection]), __FUNCTION__, "DataAnalysisSetNumAvgs");
		else if (function == Parameters(ParameterDefn::AVERAGEMODE))
			ThrowException(m_serviceWrapper->DataAnalysisSetAvgMode(EnumAvgMode(value), m_Connections[Connection]), __FUNCTION__, "DataAnalysisSetAvgMode");
	}
	catch(::CException const& E) {
		// make sure we return an error state if there are comms problems
		Status = ErrorHandler(Connection, E);
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
	int Connection = 0;
	try {
		CVQM_ITMS_Base::ThrowException(pasynUser, getAddress(pasynUser, &Connection), __FUNCTION__, "Could not get address");
		if ((Connection < 0) || (Connection >= int(NrInstalled())))
			throw CException(pasynUser, asynError, __FUNCTION__, "User / ITMS not configured");
		epicsGuard < epicsMutex > guard (*m_Threads[Connection]);
		if (m_Instance == NULL)
			// The IOC is exiting
			return asynTimeout;

		if (function == Parameters(ParameterDefn::ELECTROMETERGAIN))
		{
			ThrowException(m_serviceWrapper->SetElectrometerGainSetpoint(value, m_Connections[Connection]), __FUNCTION__, "SetElectrometerGainSetpoint");
		}
		else if (function == Parameters(ParameterDefn::EMISSION))
		{
			ThrowException(m_serviceWrapper->SetFilamentEmissionCurrentSetpoint(value, m_Connections[Connection]), __FUNCTION__, "SetFilamentEmissionCurrentSetpoint");
		}
		else if (function == Parameters(ParameterDefn::FILAMENTBIAS))
		{
			ThrowException(m_serviceWrapper->SetLogicalInstrumentVoltageSetpoint(FILAMENTBIAS, value, m_Connections[Connection]), __FUNCTION__, "SetLogicalInstrumentVoltageSetpoint FILAMENTBIAS");
		}
		else if (function == Parameters(ParameterDefn::REPELLERBIAS))
		{
			ThrowException(m_serviceWrapper->SetLogicalInstrumentVoltageSetpoint(REPELLERBIAS, value, m_Connections[Connection]), __FUNCTION__, "SetLogicalInstrumentVoltageSetpoint REPELLERBIAS");
		}
		else if (function == Parameters(ParameterDefn::ENTRYPLATE))
		{
			ThrowException(m_serviceWrapper->SetLogicalInstrumentVoltageSetpoint(ENTRYPLATE, value, m_Connections[Connection]), __FUNCTION__, "SetLogicalInstrumentVoltageSetpoint ENTRYPLATE");
		}
		else if (function == Parameters(ParameterDefn::PRESSUREPLATE))
		{
			ThrowException(m_serviceWrapper->SetLogicalInstrumentVoltageSetpoint(PRESSUREPLATE, value, m_Connections[Connection]), __FUNCTION__, "SetLogicalInstrumentVoltageSetpoint PRESSUREPLATE");
		}
		else if (function == Parameters(ParameterDefn::CUPS))
		{
			ThrowException(m_serviceWrapper->SetLogicalInstrumentVoltageSetpoint(CUPS, value, m_Connections[Connection]), __FUNCTION__, "SetLogicalInstrumentVoltageSetpoint CUPS");
		}
		else if (function == Parameters(ParameterDefn::TRANSITION))
		{
			ThrowException(m_serviceWrapper->SetLogicalInstrumentVoltageSetpoint(TRANSITION, value, m_Connections[Connection]), __FUNCTION__, "SetLogicalInstrumentVoltageSetpoint TRANSITION");
		}
		else if (function == Parameters(ParameterDefn::EXITPLATE))
		{
			ThrowException(m_serviceWrapper->SetLogicalInstrumentVoltageSetpoint(EXITPLATE, value, m_Connections[Connection]), __FUNCTION__, "SetLogicalInstrumentVoltageSetpoint EXITPLATE");
		}
		else if (function == Parameters(ParameterDefn::EMSHIELD))
		{
			ThrowException(m_serviceWrapper->SetLogicalInstrumentVoltageSetpoint(EMSHIELD, value, m_Connections[Connection]), __FUNCTION__, "SetLogicalInstrumentVoltageSetpoint EMSHIELD");
		}
		else if (function == Parameters(ParameterDefn::EMBIAS))
		{
			ThrowException(m_serviceWrapper->SetLogicalInstrumentVoltageSetpoint(EMBIAS, value, m_Connections[Connection]), __FUNCTION__, "SetLogicalInstrumentVoltageSetpoint EMBIAS");
		}
		else if (function == Parameters(ParameterDefn::RFAMP))
		{
			ThrowException(m_serviceWrapper->SetLogicalInstrumentVoltageSetpoint(RFAMP, value, m_Connections[Connection]), __FUNCTION__, "SetLogicalInstrumentVoltageSetpoint RFAMP");
		}
		else if (function == Parameters(ParameterDefn::MASSFROM))
		{
			double ToValue = getDoubleParam(Connection, ParameterDefn::MASSTO);
			ThrowException(m_serviceWrapper->SetScanRange(value, ToValue, m_Connections[Connection]), __FUNCTION__, "SetScanRange TO");
		}
		else if (function == Parameters(ParameterDefn::MASSTO))
		{
			double FromValue = getDoubleParam(Connection, ParameterDefn::MASSFROM);
			ThrowException(m_serviceWrapper->SetScanRange(FromValue, value, m_Connections[Connection]), __FUNCTION__, "SetScanRange FROM");
		}
	}
	catch(::CException const& E) {
		// make sure we return an error state if there are comms problems
		Status = ErrorHandler(Connection, E);
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
	catch(::CException const&) {
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
	catch(::CException const& E) {
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
