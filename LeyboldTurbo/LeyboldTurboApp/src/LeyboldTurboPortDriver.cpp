//////////////////////////////////////////////////////////////////////////////////////////////////////
//																									//
//	Module:																							//
//		LeyboldTurboPortDriver.cpp																	//
//																									//
//	Description:																					//
//		Implements the CLeyboldTurboPortDriver class.												//
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

#include "LeyboldTurboPortDriver.h"

#include <iocsh.h>
#include <epicsExit.h>
#include <epicsAssert.h>
#include <epicsGuard.h>
#include <asynOctetSyncIO.h>

#include "USSPacket.h"
#include "ParameterDefns.h"

#include <epicsThread.h>

#define epicsExportSharedSymbols
#include <epicsExport.h>

#include <stdlib.h>
#include <stdexcept>

CLeyboldTurboPortDriver* CLeyboldTurboPortDriver::m_Instance;

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
//	CLeyboldTurboPortDriver::CLeyboldTurboPortDriver(const char *asynPortName, int numPumps)	//
//	CLeyboldTurboPortDriver::~CLeyboldTurboPortDriver()											//
//																								//
//	Description:																				//
//		Class constructor & destructor.															//
//	Parameters:																					//
//		asynPortName - the IOC port name to be used (e.g. TURBO).								//
//		numPumps - how many pumps will be attached?												//
//				 - The expectation is that addIOPort will be called this many times				//
//				 - from the st.cmd script.														//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
CLeyboldTurboPortDriver::CLeyboldTurboPortDriver(const char *asynPortName, int numPumps, int NoOfPZD)
   : CLeyboldBase(asynPortName, 
                    numPumps,	// maxAddr
                    UsedParams(),
					NoOfPZD	// Either 2 or 6, depending on the serial port and model
					)
{
	m_Instance = this;
}

CLeyboldTurboPortDriver::~CLeyboldTurboPortDriver()
{
	m_Instance = NULL;
	disconnect();
	for(size_t Index = 0; Index < m_Mutexes.size(); Index++)
		delete m_Mutexes[Index];
}

void CLeyboldTurboPortDriver::disconnect()
{
	asynStatus overallstatus = asynSuccess;
	for(size_t Index = 0; Index < m_Mutexes.size(); Index++)
		m_Mutexes[Index]->lock();
	for(size_t Index = 0; Index < m_IOUsers.size(); Index++)
	{
		asynStatus status = pasynOctetSyncIO->disconnect(m_IOUsers[Index]);
		if (overallstatus == asynSuccess)
			overallstatus = status;
	}
	for(size_t Index = 0; Index < m_Mutexes.size(); Index++)
		m_Mutexes[Index]->unlock();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
//	void CLeyboldTurboPortDriver::addIOPort(const char* IOPortName)								//
//																								//
//	Description:																				//
//		Called once (from LeyboldTurboAddIOPort) for each pump,									//
//		in response to the st.cmd startup script.												//
//		Adds a pump, and the parameters to support it, to the configuration.					//
//																								//
//	Parameters:																					//
//		IOPortName. This is the I/O port name (e.g. PUMP:1).									//
//					This will have been mapped by drvAsynIPPortConfigure to a physical port,	//
//					e.g. COM1: (or /dev/ttyS0).													//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
void CLeyboldTurboPortDriver::addIOPort(const char* IOPortName)
{
	if (int(m_IOUsers.size()) >= maxAddr)
		throw CException(pasynUserSelf, asynError, __FUNCTION__, "Too many pumps connected=" + std::string(IOPortName));

	// If the new connection fails, the size of the vectors will still have increased.
	// This means only the failing connection will be lost, and not all subsequent connections.
	m_IOUsers.push_back(NULL);
	m_Mutexes.push_back(NULL);
		
	for (size_t ParamIndex = 0; ParamIndex < size_t(NUM_PARAMS); ParamIndex++)
	{
		if (ParameterDefns[ParamIndex].m_UseCase == NotForReal)
			// Not used by the 'real' IOC
			continue;
		if (ParameterDefns[ParamIndex].m_UseCase == Single)
			// Single instance parameter
			continue;
		// Create parameters from the definitions.
		// These variables end up being addressed as e.g. TURBO:1:RUNNING.
		createParam(m_IOUsers.size(), ParamIndex);
	}

	// Expected normal operation is for the pump to be running.
	// NB, this value is NOT being applied as an output to the operating state of the pump.
	// It is only the default value of the read-back, prior to the value being set by a scan cycle.
	setIntegerParam (m_IOUsers.size(), RUNNING, On);

	asynUser* IOUser;
	// Connect to the I/O port.
	asynStatus Status = pasynOctetSyncIO->connect(IOPortName, int(m_IOUsers.size()), &IOUser, NULL);
	if (Status != asynSuccess)
		throw CException(pasynUserSelf, Status, __FUNCTION__, "connecting to IO port=" + std::string(IOPortName));

	callParamCallbacks(NrInstalled());
	m_IOUsers.back() = IOUser;
	m_Mutexes.back() = new epicsMutex();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
//	int CLeyboldTurboPortDriver::UsedParams()													//
//																								//
//	Description:																				//
//		Gives a count of how many parameters are required for this IOC.							//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
int CLeyboldTurboPortDriver::UsedParams()
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
//	asynStatus CLeyboldTurboPortDriver::readInt32(asynUser *pasynUser, epicsInt32 *value)		//
//																								//
//	Description:																				//
//		This method is invoked by the Asyn framework to read integer values from the hardware.	//
//		It will normally be called for the FAULT variable which is set to poll once every		//
//		5 seconds.																				//
//																								//
//		However, all the variables are read back from the I/O port in one go because			//
//		that's how the pump controller interface works.											//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
asynStatus CLeyboldTurboPortDriver::readInt32(asynUser *pasynUser, epicsInt32 *value)
{
	int function = pasynUser->reason;
	int TableIndex;
	asynStatus Status = asynSuccess;
	if (m_Instance == NULL)
		// The IOC is exiting
		return asynTimeout;

	try {
		Status = getAddress(pasynUser, &TableIndex);
		if (Status != asynSuccess)
			throw CException(pasynUser, Status, __FUNCTION__, "Could not get address");
		if ((TableIndex < 0) || (TableIndex > int(NrInstalled())))
			throw CException(pasynUser, asynError, __FUNCTION__, "User / pump not configured");

		epicsGuard < epicsMutex > guard ( *(m_Mutexes[TableIndex]) );
		if (m_Instance == NULL)
			// The IOC is exiting
			return asynTimeout;
		
		bool Running = (getIntegerParam(TableIndex, RUNNING) != Off);

		if (function == Parameters(STATORFREQUENCY))
		{
			// The following values are present in the 24-byte packet (NoOfPZD==6),
			// but need each to be explictly queried when the 16-byte packet (NoOfPZD==2) is being used.
			if (m_NoOfPZD == NoOfPZD2)
			{
				{
					USSPacket<NoOfPZD2> USSWritePacket(Running, 4), // Intermediate circuit voltage Uzk
						USSReadPacket;
					if (!writeRead(TableIndex, pasynUser, USSWritePacket, USSReadPacket))
						// Get outta here but don't log error.
						return asynSuccess;

					processRead(TableIndex, pasynUser, USSReadPacket);
					setDoubleParam  (TableIndex, CIRCUITVOLTAGE, 0.1 * USSReadPacket.m_USSPacketStruct.m_PWE);
				}
				{
					USSPacket<NoOfPZD2> USSWritePacket(Running, 5), // Motor current - actual value
						USSReadPacket;
					if (!writeRead(TableIndex, pasynUser, USSWritePacket, USSReadPacket))
						// Get outta here but don't log error.
						return asynSuccess;

					processRead(TableIndex, pasynUser, USSReadPacket);
					setDoubleParam  (TableIndex, MOTORCURRENT, 0.1 * USSReadPacket.m_USSPacketStruct.m_PWE);
				}
				{
					USSPacket<NoOfPZD2> USSWritePacket(Running, 7), // Converter temperature - actual value
						USSReadPacket;
					if (!writeRead(TableIndex, pasynUser, USSWritePacket, USSReadPacket))
						// Get outta here but don't log error.
						return asynSuccess;
					processRead(TableIndex, pasynUser, USSReadPacket);
					setIntegerParam (TableIndex, PUMPTEMPERATURE, USSReadPacket.m_USSPacketStruct.m_PWE);
				}
				{
					USSPacket<NoOfPZD2> USSWritePacket(Running, 11), // Converter temperature - actual value
						USSReadPacket;
					if (!writeRead(TableIndex, pasynUser, USSWritePacket, USSReadPacket))
						// Get outta here but don't log error.
						return asynSuccess;
					processRead(TableIndex, pasynUser, USSReadPacket);
					setIntegerParam (TableIndex, CONVERTERTEMPERATURE, USSReadPacket.m_USSPacketStruct.m_PWE);
				}
			}
			else
			{
				USSPacket<NoOfPZD6> USSWritePacket(Running), USSReadPacket;
				if (!writeRead(TableIndex, pasynUser, USSWritePacket, USSReadPacket))
					// Get outta here but don't log error.
					return asynSuccess;

				processRead(TableIndex, pasynUser, USSReadPacket);
				// Process the data fields 2..5. These don't exist for the smaller packet.
				setIntegerParam (TableIndex, CONVERTERTEMPERATURE, USSReadPacket.m_USSPacketStruct.m_PZD[2]);
				setDoubleParam  (TableIndex, MOTORCURRENT, 0.1 * USSReadPacket.m_USSPacketStruct.m_PZD[3]);
				setIntegerParam (TableIndex, PUMPTEMPERATURE, USSReadPacket.m_USSPacketStruct.m_PZD[4]);
				setDoubleParam  (TableIndex, CIRCUITVOLTAGE, 0.1 * USSReadPacket.m_USSPacketStruct.m_PZD[5]);
			}
		}
		asynPrint(pasynUser, ASYN_TRACE_FLOW, "Packet success %s %s\n", __FILE__, __FUNCTION__);
		Status = CLeyboldBase::readInt32(pasynUser, value);
	}
	catch(CException const& E) {
		// make sure we return an error state if there are comms problems
		Status = ErrorHandler(TableIndex, E);
	}
	callParamCallbacks(TableIndex);
	return Status;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
//	asynStatus CLeyboldTurboPortDriver::readOctet(asynUser *pasynUser, char *value,				//
//		size_t maxChars, size_t *nActual, int *eomReason)										//
//																								//
//	Description:																				//
//		This method is invoked by the Asyn framework to read string values from the hardware.	//
//		The only such is the firmware version.													//
//		It is assumed the firmware version will not change while the software is running.		//
//		So this method only needs to be invoked once on start-up.								//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
asynStatus CLeyboldTurboPortDriver::readOctet(asynUser *pasynUser, char *value, size_t maxChars,
                                        size_t *nActual, int *eomReason)
{
	int function = pasynUser->reason;
	int TableIndex;
	asynStatus Status = asynSuccess;
	if (m_Instance == NULL)
		// The IOC is exiting
		return asynTimeout;
	try {
		Status = getAddress(pasynUser, &TableIndex);
		if (Status != asynSuccess)
			throw CException(pasynUser, Status, __FUNCTION__, "Could not get address");
		if ((TableIndex < 0) || (TableIndex > int(NrInstalled())))
			throw CException(pasynUser, asynError, __FUNCTION__, "User / pump not configured");
		epicsGuard < epicsMutex > guard ( *(m_Mutexes[TableIndex]) );
		if (m_Instance == NULL)
			// The IOC is exiting
			return asynTimeout;
		bool Running = (getIntegerParam(TableIndex, RUNNING) != Off);
		if (function == Parameters(FIRMWAREVERSION))
		{
			epicsUInt32 PWE;
			// Software version (I assume this means firmware). e.g. 3.03.05
			if (m_NoOfPZD == NoOfPZD2)
			{
				USSPacket<NoOfPZD2> USSWritePacket(Running, 2),
					USSReadPacket;
				if (!writeRead(TableIndex, pasynUser, USSWritePacket, USSReadPacket))
					// Get outta here but don't log error.
					return asynSuccess;

				PWE = USSReadPacket.m_USSPacketStruct.m_PWE;
			}
			else
			{
				USSPacket<NoOfPZD6> USSWritePacket(Running, 2),
					USSReadPacket;
				if (!writeRead(TableIndex, pasynUser, USSWritePacket, USSReadPacket))
					// Get outta here but don't log error.
					return asynSuccess;

				PWE = USSReadPacket.m_USSPacketStruct.m_PWE;
			}
			char CBuf[8]; // 7 chars plus null termination
			int Major=PWE / 10000,
				Minor1 = (PWE % 10000) / 100,
				Minor2 = PWE %100;
			epicsSnprintf(CBuf, sizeof(CBuf), "%1d.%02d.%02d", Major, Minor1, Minor2);
			setStringParam (TableIndex, FIRMWAREVERSION, CBuf);

		}
		Status = CLeyboldBase::readOctet(pasynUser, value, maxChars, nActual, eomReason);
	}
	catch(CException const& E) {
		// make sure we return an error state if there are comms problems
		Status = ErrorHandler(TableIndex, E);
	}
	callParamCallbacks(TableIndex);
	return Status;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
//	template<size_t NoOfPZD> bool CLeyboldTurboPortDriver::writeRead(int TableIndex,			//
//		asynUser *pasynUser, USSPacket<NoOfPZD> USSWritePacket, USSPacket<NoOfPZD>& USSReadPacket)
//		size_t maxChars, size_t *nActual, int *eomReason)										//
//																								//
//	Description:																				//
//		This method is invoked by the readInt32 method to send a request (or command) packet,	//
//		and then read the controller's response													//
//		It's a wrapper round Asyn's writeRead method that provides byte-swapping and validation.//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
template<size_t NoOfPZD> bool CLeyboldTurboPortDriver::writeRead(int TableIndex, asynUser *pasynUser, USSPacket<NoOfPZD> USSWritePacket, USSPacket<NoOfPZD>& USSReadPacket)
{
	int eomReason;
	size_t nBytesOut, nBytesIn;
	asynUser* IOUser = m_IOUsers[TableIndex];
#ifdef _DEBUG
	// Infinite timeout, convenient for debugging.
	const double TimeOut = -1;
#else
	const double TimeOut = 5;
#endif
	if (IOUser == NULL)
		// The connection is failed.
		return false;

	USSWritePacket.m_USSPacketStruct.HToN();
	// NB, *don't* pass pasynUser to this function - it has the wrong type and will cause an access violation.
	asynStatus Status = pasynOctetSyncIO->writeRead(IOUser,
		reinterpret_cast<const char*>(USSWritePacket.m_Bytes), USSPacketStruct<NoOfPZD>::USSPacketSize, 
		reinterpret_cast<char*>(USSReadPacket.m_Bytes), USSPacketStruct<NoOfPZD>::USSPacketSize,
		TimeOut, &nBytesOut, &nBytesIn, &eomReason);

	if (m_Instance == NULL)
		// The IOC is exiting
		return false;

	if ((getParamStatus(TableIndex, FAULT) != asynSuccess) || (getIntegerParam (TableIndex, FAULT) == 65))
	{
		// If the connection is broken, it will generate an error every time, which is unhelpful at diagnosing.
		// Only log the first fail and when the connection is restored.
		if (Status == asynSuccess)
			asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, "Connection to %d resumed\n", TableIndex);
		else
			return false;
	}
	if (Status != asynSuccess)
	{
		throw CException(IOUser, Status, __FUNCTION__, "Can't write/read:");
	}

	STATIC_ASSERT ( sizeof(USSPacketStruct<NoOfPZD>) == USSPacketStruct<NoOfPZD>::USSPacketSize );
	STATIC_ASSERT ( sizeof(USSPacket<NoOfPZD>) == USSPacketStruct<NoOfPZD>::USSPacketSize );
//	if ((sizeof(USSPacket<NoOfPZD>) != USSPacketStruct<NoOfPZD>::USSPacketSize) ||
//		(sizeof(USSPacketStruct<NoOfPZD>) != USSPacketStruct<NoOfPZD>::USSPacketSize))
//		asynPrint(pasynUser, ASYN_TRACE_ERROR, "Packet size descrepant %ul %ul %ul\n", sizeof(USSPacket<NoOfPZD>), sizeof(USSPacketStruct<NoOfPZD>), USSPacketStruct<NoOfPZD>::USSPacketSize);

	if (nBytesOut != USSPacketStruct<NoOfPZD>::USSPacketSize)
		throw CException(pasynUser, asynError, __FUNCTION__, "Incorrect packet size transmitted");
	if (nBytesIn != USSPacketStruct<NoOfPZD>::USSPacketSize)
		throw CException(pasynUser, asynError, __FUNCTION__, "Unexpected packet size recieved");

	USSReadPacket.m_USSPacketStruct.NToH();

	if (!USSReadPacket.ValidateChecksum())
		throw CException(pasynUser, asynError, __FUNCTION__, "Packet validation failed");
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
//	template<size_t NoOfPZD> void CLeyboldTurboPortDriver::processRead(int TableIndex,			//
//		asynUser *pasynUser, USSPacket<NoOfPZD> const& USSReadPacket)							//
//																								//
//	Description:																				//
//		This method(s) is processes a data packet that has just been read.						//
//		It includes the data management that is generic to both 16 (PZD==2) and 24 (PZD==6)		//
//		packet types. So it can respond to the contents of m_PZD[0] and [1], but not 2..5.		//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
template<size_t NoOfPZD> void CLeyboldTurboPortDriver::processRead(int TableIndex, asynUser *pasynUser, USSPacket<NoOfPZD> const& USSReadPacket)
{
	bool Running = (getIntegerParam(TableIndex, RUNNING) != Off);

	// Normal operation 1 = the pump is running in the normal operation mode
	RunStates RunState=Off;
	if (USSReadPacket.m_USSPacketStruct.m_PZD[0] & (1 << 10))
		RunState=On;
	else if (USSReadPacket.m_USSPacketStruct.m_PZD[0] & (1 << 4))
		RunState=Accel;
	else if (USSReadPacket.m_USSPacketStruct.m_PZD[0] & (1 << 5))
		RunState=Decel;
	else if (USSReadPacket.m_USSPacketStruct.m_PZD[0] & (1 << 11))
		RunState=Moving;
	setIntegerParam (TableIndex, RUNNING, RunState);

	// NB Asyn 4-27 requires the parameter status to be clear before the value can be set.
	setParamStatus(TableIndex, FAULT, asynSuccess);
	setParamStatus(TableIndex, FAULTSTR, asynSuccess);
	setParamStatus(TableIndex, WARNINGTEMPERATURE, asynSuccess);
	setParamStatus(TableIndex, WARNINGTEMPERATURESTR, asynSuccess);
	setParamStatus(TableIndex, WARNINGHIGHLOAD, asynSuccess);
	setParamStatus(TableIndex, WARNINGHIGHLOADSTR, asynSuccess);
	setParamStatus(TableIndex, WARNINGPURGE, asynSuccess);
	setParamStatus(TableIndex, WARNINGPURGESTR, asynSuccess);
	if (USSReadPacket.m_USSPacketStruct.m_PZD[0] & (1 << 3))
	{
		// We have an error status. Request the error code
		USSPacket<NoOfPZD> USSWritePacket(Running, 171), USSReadPacket;

		if (!writeRead(TableIndex, pasynUser, USSWritePacket, USSReadPacket))
			// Get outta here but don't log error.
			return;

		setIntegerParam (TableIndex, FAULT, USSReadPacket.m_USSPacketStruct.m_PWE);

		// Plus null termination.
		const char ErrorStrings[77][MaxEPICSStrLen+1] =
		{
			"",											// 0, No failure
			"Overload (load limit exceeded)",			// 1
			"Motor temperature too high",				// 2
			"There has been a mains failure",			// 3
			"Converter temperature too high",			// 4
			"An overspeed has occurred",				// 5
			"During overload frequency dropped",		// 6
			"Max. run-up time was exceeded",			// 7
			"Pump identification comms fail",			// 8
			"Bearing temperature too high",				// 9
			"Cooling water temperature too high",		// 10
			"Warning TMS failure",						// 11
			"Warning Unbalance PVW13",					// 12
			"Warning Unbalance PVW24",					// 13
			"Warning Unbalance PZ12",					// 14
			"Warning magnetic bearings",				// 15
			"Max. overload time has been exceeded",		// 16
			"No motor current",							// 17
			"Pump connection converter failure",		// 18
			"Run-up time has been exceeded",			// 19
			"TMS failure",								// 20
			"TMS failure",								// 21
			"TMS failure",								// 22
			"TMS failure",								// 23
			"TMS failure",								// 24
			"Unspecified",								// 25
			"Bearing temp sensor short-circuit",		// 26
			"Cooling water temp short-circuit",			// 27
			"Motor temperature sensor short-circuit",	// 28
			"Bearing temp sensor interrupt fail",		// 29
			"Cooling water temp interrupt fail",		// 30
			"Internal connection failure",				// 31
			"Internal connection failure",				// 32
			"Magnetic bearing overload PZ12",			// 33
			"Magnetic bearing overload PV13",			// 34
			"Magnetic bearing overload PW24",			// 35
			"Unspecified",								// 36
			"Flow warning",								// 37
			"Warning operation without purge gas",		// 38
			"Magnetic bearing failure",					// 39
			"Magnetic bearing, purge gas OFF",			// 40
			"Magnetic bearing, purge gas ON",			// 41
			"Magnetic bearing code wrong",				// 42
			"Internal failure",							// 43
			"Internal failure",							// 44
			"Internal failure",							// 45
			"Internal failure",							// 46
			"Internal failure",							// 47
			"Internal failure",							// 48
			"Internal failure",							// 49
			"Internal failure",							// 50
			"Internal failure",							// 51
			"Internal failure",							// 52
			"Internal failure",							// 53
			"Internal failure",							// 54
			"Internal failure",							// 55
			"External shutdown for protection",			// 56
			"Internal failure",							// 57
			"Internal failure",							// 58
			"Internal failure",							// 59
			"Internal failure",							// 60
			"Internal failure",							// 61
			"Internal failure",							// 62
			"Internal communication failure (SPI)",		// 63
			"Mag bearing electronics not initialised",	// 64
			"Internal communication timeout",			// 65
			"Magnetic bearing overloaded",				// 66
			"Internal overload",						// 67
			"Rotor not lifted",							// 68
			"ABS inactive warning",						// 69 
			"ABS active warning",						// 70
			"Failure during parameter download",		// 71
			"Failure during firmware download",			// 72
			"Operating cycles limit has been reached",	// 73
			"Operating hours limit has been reached",	// 74
			"Faulty configuration",						// 75
			"Firmware update is required"				// 76
		};

		// NB, Asyn 4-27 requires that the parameter status is success before the value can be set through callback.
		setStringParam (TableIndex, FAULTSTR, ErrorStrings[USSReadPacket.m_USSPacketStruct.m_PWE]);
		callParamCallbacks(TableIndex);

		setParamStatus(TableIndex, FAULT, asynError);
		setParamStatus(TableIndex, FAULTSTR, asynError);
	}
	else
	{
		// Clear error status
		setIntegerParam (TableIndex, FAULT, 0);
		setStringParam (TableIndex, FAULTSTR, "");
	}

	if (USSReadPacket.m_USSPacketStruct.m_PZD[0] & (1 << 7))
	{
		// We have a temperature warning status. Request the warning bits.
		USSPacket<NoOfPZD> USSWritePacket(Running, 227), USSReadPacket;

		if (!writeRead(TableIndex, pasynUser, USSWritePacket, USSReadPacket))
			// Get outta here but don't log error.
			return;

		// NB, 16 possible warning values here.
		// This is not (now) using MBBI but keep it short.
		const char WarningStrings[BitsPerUInt16][MaxEPICSMBBIStrLen] =
		{
			"Motor",
			"Converter",
			"Bearing",
			"Cooling water",
			"TCU temperature",
			"Pump identifier",
			"Overspeed",
			"TCU collective",
			"No flow",
			"Mag bearing",
			"Imbalance PVW13",
			"Imbalance PVW24",
			"Imbalance PZ12",
			"No purge gas",
			"Too much purge",
			"without purge"
		};

		std::string WarningTemperatureStr;
		for(size_t Bit = 0; Bit < BitsPerUInt16; Bit++)
		{
			if (!(USSReadPacket.m_USSPacketStruct.m_PWE & (1 << Bit)))
				continue;
			if (WarningTemperatureStr.size() > 0)
				WarningTemperatureStr += "\n";
			WarningTemperatureStr += WarningStrings[Bit];
		}
		if (WarningTemperatureStr == "")
			// Unexpected that the string should be empty.
			WarningTemperatureStr = "Unknown";
		setIntegerParam (TableIndex, WARNINGTEMPERATURE, USSReadPacket.m_USSPacketStruct.m_PWE);
		setStringParam (TableIndex, WARNINGTEMPERATURESTR, WarningTemperatureStr);

		// NB, Asyn 4-27 requires that the parameter status is success before the value can be set through callback.
		setParamStatus(TableIndex, WARNINGTEMPERATURE, asynError);
		setParamStatus(TableIndex, WARNINGTEMPERATURESTR, asynError);
	}
	else
	{
		setIntegerParam (TableIndex, WARNINGTEMPERATURE, 0);
		setStringParam (TableIndex, WARNINGTEMPERATURESTR, "");
	}

	if (USSReadPacket.m_USSPacketStruct.m_PZD[0] & (1 << 13))
	{
		// We have a high load warning status. Request the warning bits.
		USSPacket<NoOfPZD> USSWritePacket(Running, 228), USSReadPacket;

		if (!writeRead(TableIndex, pasynUser, USSWritePacket, USSReadPacket))
			// Get outta here but don't log error.
			return;

		const char WarningStrings[BitsPerUInt16][MaxEPICSMBBIStrLen] =
		{
			"TMS heater fail",
			"TMS time error",
			"TMS Pt 100 def",
			"TMS current hi",
			"TMS fuse def",
			"Ident read",
			"Ident write",
			"Without comms",
			"ABS inactive",
			"ABS active",
			"Bearing inact",
			"Bearing overl",
			"Internal overl",
			"SPI (AMB2SR)",
			"SPI (SR2AMB)"
		};
		std::string WarningHighLoadStr;
		for(size_t Bit = 0; Bit < BitsPerUInt16; Bit++)
		{
			if (!(USSReadPacket.m_USSPacketStruct.m_PWE & (1 << Bit)))
				continue;
			if (WarningHighLoadStr.size() > 0)
				WarningHighLoadStr += "\n";
			WarningHighLoadStr += WarningStrings[Bit];
		}
		if (WarningHighLoadStr == "")
			// Unexpected that the warning string will be empty. But this can cause an update problem.
			WarningHighLoadStr = "Unknown";
		// NB, Asyn 4-27 requires that the parameter status is success before the value can be set through callback.
		setIntegerParam (TableIndex, WARNINGHIGHLOAD, USSReadPacket.m_USSPacketStruct.m_PWE);
		setStringParam (TableIndex, WARNINGHIGHLOADSTR, WarningHighLoadStr);

		setParamStatus(TableIndex, WARNINGHIGHLOAD, asynError);
		setParamStatus(TableIndex, WARNINGHIGHLOADSTR, asynError);
	}
	else
	{
		setIntegerParam (TableIndex, WARNINGHIGHLOAD, 0);
		setStringParam (TableIndex, WARNINGHIGHLOADSTR, "");
	}

	if (USSReadPacket.m_USSPacketStruct.m_PZD[0] & (1 << 14))
	{
		// We have a purge not active warning status. Request the warning bits.
		USSPacket<NoOfPZD> USSWritePacket(Running, 230), USSReadPacket;

		if (!writeRead(TableIndex, pasynUser, USSWritePacket, USSReadPacket))
			// Get outta here but don't log error.
			return;

		const char WarningStrings[8][MaxEPICSMBBIStrLen] =
		{
			"Ident CRC data",
			"Ident CRC proto",
			"Ident timeout",
			"TMS temp",
			"Unassigned",
			"Nr of cycles",
			"Nr of hours",
			"EEPROM contents"
		};
		std::string WarningPurgeStr;
		for(size_t Bit = 0; Bit < 8; Bit++)
		{
			if (!(USSReadPacket.m_USSPacketStruct.m_PWE & (1 << Bit)))
				continue;
			if (WarningPurgeStr.size() > 0)
				WarningPurgeStr += "\n";
			WarningPurgeStr += WarningStrings[Bit];
		}
		if (WarningPurgeStr == "")
			// Unexpected that the string should be empty.
			WarningPurgeStr = "Unknown";

		// NB, Asyn 4-27 requires that the parameter status is success before the value can be set through callback.
		setIntegerParam (TableIndex, WARNINGPURGE, USSReadPacket.m_USSPacketStruct.m_PWE);
		setStringParam (TableIndex, WARNINGPURGESTR, WarningPurgeStr);
		callParamCallbacks(TableIndex);

		setParamStatus(TableIndex, WARNINGPURGE, asynError);
		setParamStatus(TableIndex, WARNINGPURGESTR, asynError);
	}
	else
	{
		setIntegerParam (TableIndex, WARNINGPURGE, 0);
		setStringParam (TableIndex, WARNINGPURGESTR, "");
	}

	setIntegerParam (TableIndex, STATORFREQUENCY, USSReadPacket.m_USSPacketStruct.m_PZD[1]);
}

template void CLeyboldTurboPortDriver::processRead<CLeyboldTurboPortDriver::NoOfPZD2>(int TableIndex, asynUser *pasynUser, USSPacket<NoOfPZD2> const& USSReadPacket);
template void CLeyboldTurboPortDriver::processRead<CLeyboldTurboPortDriver::NoOfPZD6>(int TableIndex, asynUser *pasynUser, USSPacket<NoOfPZD6> const& USSReadPacket);

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
//	template<size_t NoOfPZD> void CLeyboldTurboPortDriver::processWrite(int TableIndex,			//
//		asynUser *pasynUser, epicsInt32 value)													//
//																								//
//	Description:																				//
//		This method implements the logic of writeInt32.											//
//		It's templated for the two packet types.												//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
template<size_t NoOfPZD> void CLeyboldTurboPortDriver::processWrite(int TableIndex, asynUser *pasynUser, epicsInt32 value)
{
	// Normal operation 1 = the pump is running in the normal operation mode
	USSPacket<NoOfPZD> USSWritePacket(false), // Assumed not currently running
		USSReadPacket;

	int function = pasynUser->reason;

	if (function == Parameters(RUNNING))
	{
		// 1 = Start; 0 = Stop
		// Is only run provided if
		//		no error is present and
		//		control bit 10 = 1
		USSWritePacket.m_USSPacketStruct.m_PZD[0] |= 1 << 10;
		bool Running = (value == On) || (value == Accel);	// Just in case someone tries to set the Accel state.
		USSWritePacket.m_USSPacketStruct.m_PZD[0] |= (Running ? 1 : 0) << 0;	// Set Running bit.
	}

	if (function == Parameters(RESET))
	{
		// 0 to 1 transition = Error reset
		//
		// Is only run provided if 
		//		the cause for the error has been removed and
		//		control bit 0 = 0 and
		//		control bit 10 = 1
		bool Running = (getIntegerParam(TableIndex, RUNNING) != Off);
		if ((Running) && (value))
			throw CException(pasynUser, asynError, __FUNCTION__, "The pump must be halted before a reset can be applied");
		USSWritePacket.m_USSPacketStruct.m_PZD[0] |= 1 << 10;
		USSWritePacket.m_USSPacketStruct.m_PZD[0] |= (value ? 1 : 0) << 7;	// High
	}
	USSWritePacket.GenerateChecksum();
	if (!writeRead(TableIndex, pasynUser, USSWritePacket, USSReadPacket))
		// Get outta here but don't log error.
		return;
	processRead(TableIndex, pasynUser, USSReadPacket);
}

asynStatus CLeyboldTurboPortDriver::ErrorHandler(int TableIndex, CException const& E)
{
	if (m_Instance == NULL)
		// The IOC is exiting
		return asynTimeout;

	// Internal communication failure
	// NB, Asyn 4-27 requires that the parameter status is success before the value can be set through callback.
	setParamStatus(TableIndex, FAULT, asynSuccess);
	setParamStatus(TableIndex, FAULTSTR, asynSuccess);
	setIntegerParam(TableIndex, FAULT, 65);
	setStringParam (TableIndex, FAULTSTR, "Internal communication timeout");
	callParamCallbacks(TableIndex);

	setParamStatus(TableIndex, FAULT, E.Status());
	setParamStatus(TableIndex, FAULTSTR, E.Status());
	return E.Status();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
//	asynStatus CLeyboldTurboPortDriver::writeInt32(asynUser *pasynUser, epicsInt32 value)		//
//																								//
//	Description:																				//
//		This method is invoked by the Asyn framework to write values to the hardware.			//
//		There are only 2 parameters that have write behaviour, RUNNING and RESET.				//
//		NB, RUNNING *also* has read-back behaviour.												//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
asynStatus CLeyboldTurboPortDriver::writeInt32(asynUser *pasynUser, epicsInt32 value)
{
	if (m_Instance == NULL)
		// The IOC is exiting
		return asynTimeout;

	// Invoke the base class method to store the value in the database.
	asynStatus Status = CLeyboldBase::writeInt32(pasynUser, value);
	int TableIndex = 0;
	try {
		Status = getAddress(pasynUser, &TableIndex);
		if (Status != asynSuccess)
			throw CException(pasynUser, Status, __FUNCTION__, "Could not get address");
		if ((TableIndex < 0) || (TableIndex >= int(m_IOUsers.size())))
			throw CException(pasynUser, asynError, __FUNCTION__, "User / pump not configured");
		epicsGuard < epicsMutex > guard ( *(m_Mutexes[TableIndex]) );
		if (m_Instance == NULL)
			// The IOC is exiting
			return asynTimeout;
		if (m_NoOfPZD == NoOfPZD2)
			processWrite<NoOfPZD2>(TableIndex, pasynUser, value);
		else
			processWrite<NoOfPZD6>(TableIndex, pasynUser, value);
	}
	catch(CException const& E) {
		// make sure we return an error state if there are comms problems
		Status = ErrorHandler(TableIndex, E);
	}
	callParamCallbacks(TableIndex);
	return Status;
}

static const iocshArg initArg0 = { "asynPortName", iocshArgString};
static const iocshArg initArg1 = { "numPumps", iocshArgString};
static const iocshArg initArg2 = { "NoOfPZD", iocshArgString};
static const iocshArg * const initArgs[] = {&initArg0, &initArg1, &initArg2};
static const iocshFuncDef initFuncDef = {"LeyboldTurboPortDriverConfigure",3,initArgs};

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
//	void LeyboldTurboExitFunc(void * param)														//
//																								//
//	Description:																				//
//		This function will be invoked when the IOC exits.										//
//		In order to not leak resources, it destroys the object that's been created.				//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
void LeyboldTurboExitFunc(void * param)
{
	CLeyboldTurboPortDriver* Instance = static_cast<CLeyboldTurboPortDriver*>(param);
//	Instance->disconnect();
	delete Instance;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
//	static void LeyboldTurboPortDriverConfigure(const iocshArgBuf *args)						//
//																								//
//	Description:																				//
//		This function will be invoked when from the st.cmd starup script.						//
//		It creates the object and also schedules the exit event.								//
//																								//
//	Parameters:																					//
//		asynPortName. This is the Asyn port name (e.g. LEYBOLDTURBO).							//
//					This will have been mapped by drvAsynIPPortConfigure to a physical port,	//
//					e.g. COM1: (or /dev/ttyS0).													//
//		numPumps - how many pumps will be attached?												//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
static void LeyboldTurboPortDriverConfigure(const iocshArgBuf *args)
{
	try {
		const char* asynPortName = args[0].sval;
		int numPumps = atoi(args[1].sval);
		int NoOfPZD = atoi(args[2].sval);
		CLeyboldTurboPortDriver* Instance = new CLeyboldTurboPortDriver(asynPortName, numPumps, NoOfPZD);
		epicsAtExit(LeyboldTurboExitFunc, Instance);
	}
	catch(CLeyboldTurboPortDriver::CException const&) {
	}
}

static const iocshArg addArg0 = { "IOPortName", iocshArgString};
static const iocshArg * const addArgs[] = {&addArg0};
static const iocshFuncDef addFuncDef = {"LeyboldTurboAddIOPort",1,addArgs};

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
//	static void LeyboldTurboAddIOPort(const iocshArgBuf *args)									//
//	Description:																				//
//		EPICS iocsh callable function to add a (simulated or real) pump to the IOC.				//
//																								//
//	Parameters:																					//
//		IOPortName - the IOC port name (e.g. PUMP:1) to be used.								//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
static void LeyboldTurboAddIOPort(const iocshArgBuf *args)
{
	try {
		const char* IOPortName = args[0].sval;
		// Test the driver has been configured
		if (CLeyboldTurboPortDriver::Instance())
			CLeyboldTurboPortDriver::Instance()->addIOPort(IOPortName);
	}
	catch(CLeyboldTurboPortDriver::CException const& E) {
		// make sure we return an error state if there are comms problems
		if (CLeyboldTurboPortDriver::Instance())
			CLeyboldTurboPortDriver::Instance()->ErrorHandler(CLeyboldTurboPortDriver::Instance()->NrInstalled(), E);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
//	static void LeyboldTurboRegistrar(void)														//
//																								//
//	Description:																				//
//		Registers the functions to be called within the IOC.									//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
static void LeyboldTurboRegistrar(void)
{
    iocshRegister(&initFuncDef, LeyboldTurboPortDriverConfigure);
    iocshRegister(&addFuncDef, LeyboldTurboAddIOPort);
}

extern "C" {

epicsExportRegistrar(LeyboldTurboRegistrar);

}
