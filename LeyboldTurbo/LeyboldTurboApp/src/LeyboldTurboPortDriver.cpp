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
#include <asynOctetSyncIO.h>

#include "USSPacket.h"
#include "ParameterDefns.h"

#define epicsExportSharedSymbols
#include <epicsExport.h>

#include <stdlib.h>
#include <stdexcept>

static CLeyboldTurboPortDriver* g_LeyboldTurboPortDriver;

const char* SoftwareVersion = "1.0";

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
                    NUM_PARAMS,
					NoOfPZD,	// Either 2 or 6, depending on the serial port and model
                    asynDrvUserMask | asynInt32Mask | asynFloat64Mask | asynOctetMask // Interface and interrupt mask// Interface mask
					)
{
}

CLeyboldTurboPortDriver::~CLeyboldTurboPortDriver()
{
	asynStatus overallstatus = asynSuccess;
	for(size_t Index = 0; Index < m_IOUsers.size(); Index++)
	{
		asynStatus status = pasynOctetSyncIO->disconnect(m_IOUsers[Index]);
		if (overallstatus == asynSuccess)
			overallstatus = status;
	}
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
		throw CException(pasynUserSelf, __FUNCTION__, "Too many pumps connected=" + std::string(IOPortName));
		
	for (size_t ParamIndex = 0; ParamIndex < size_t(NUM_PARAMS); ParamIndex++)
	{
		// Create parameters from the definitions.
		// These variables end up being addressed as e.g. TURBO:1:RUNNING.
		createParam(m_IOUsers.size(), ParamIndex);
		switch(ParameterDefns[ParamIndex].m_ParamType)
		{
			// Set default values.
			case asynParamInt32: 
				setIntegerParam(int(m_IOUsers.size()), ParameterDefns[ParamIndex].m_ParamName, 0);
				break;
			case asynParamFloat64: 
				setDoubleParam (int(m_IOUsers.size()), ParameterDefns[ParamIndex].m_ParamName, 0.0);
				break;
			case asynParamOctet: 
				setStringParam (int(m_IOUsers.size()), ParameterDefns[ParamIndex].m_ParamName, "");
				break;
			default: assert(false);
		}
	}

	// Normal operation 1 = the pump is running in the normal operation mode
	setIntegerParam (m_IOUsers.size(), RUNNING, 1);

	asynUser* IOUser;
	// Connect to the I/O port.
	if (pasynOctetSyncIO->connect(IOPortName, int(m_IOUsers.size()), &IOUser, NULL) != asynSuccess)
		throw CException(pasynUserSelf, __FUNCTION__, "connecting to IO port=" + std::string(IOPortName));

	if (callParamCallbacks() != asynSuccess)
		// Make sure the (just set) default values are available to read.
		throw CException(pasynUserSelf, __FUNCTION__, "callParamCallbacks");
	m_IOUsers.push_back(IOUser);
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

	try {
		if (getAddress(pasynUser, &TableIndex) != asynSuccess)
			throw CException(pasynUser, __FUNCTION__, "Could not get address");
		if ((TableIndex < 0) || (TableIndex >= int(m_IOUsers.size())))
			throw CException(pasynUser, __FUNCTION__, "User / pump not configured");

		bool Running = (getIntegerParam(TableIndex, RUNNING) != 0);

		if (function == Parameters(FAULT))
		{
			// The following values are present in the 24-byte packet (NoOfPZD==6),
			// but need each to be explictly queried when the 16-byte packet (NoOfPZD==2) is being used.
			if (m_NoOfPZD == NoOfPZD2)
			{
				{
					USSPacket<NoOfPZD2> USSWritePacket(Running, 4), // Intermediate circuit voltage Uzk
						USSReadPacket;
					writeRead(TableIndex, pasynUser, USSWritePacket, USSReadPacket);
					processRead(TableIndex, pasynUser, USSReadPacket);
					setDoubleParam  (TableIndex, CIRCUITVOLTAGE, 0.1 * USSReadPacket.m_USSPacketStruct.m_PWE);
				}
				{
					USSPacket<NoOfPZD2> USSWritePacket(Running, 5), // Motor current - actual value
						USSReadPacket;
					writeRead(TableIndex, pasynUser, USSWritePacket, USSReadPacket);
					processRead(TableIndex, pasynUser, USSReadPacket);
					setDoubleParam  (TableIndex, MOTORCURRENT, 0.1 * USSReadPacket.m_USSPacketStruct.m_PWE);
				}
				{
					USSPacket<NoOfPZD2> USSWritePacket(Running, 7), // Converter temperature - actual value
						USSReadPacket;
					writeRead(TableIndex, pasynUser, USSWritePacket, USSReadPacket);
					processRead(TableIndex, pasynUser, USSReadPacket);
					setIntegerParam (TableIndex, PUMPTEMPERATURE, USSReadPacket.m_USSPacketStruct.m_PWE);
				}
				{
					USSPacket<NoOfPZD2> USSWritePacket(Running, 11), // Converter temperature - actual value
						USSReadPacket;
					writeRead(TableIndex, pasynUser, USSWritePacket, USSReadPacket);
					processRead(TableIndex, pasynUser, USSReadPacket);
					setIntegerParam (TableIndex, CONVERTERTEMPERATURE, USSReadPacket.m_USSPacketStruct.m_PWE);
				}
			}
			else
			{
				USSPacket<NoOfPZD6> USSWritePacket(Running), USSReadPacket;
				writeRead(TableIndex, pasynUser, USSWritePacket, USSReadPacket);
				processRead(TableIndex, pasynUser, USSReadPacket);
				// Process the data fields 2..5. These don't exist for the smaller packet.
				setIntegerParam (TableIndex, CONVERTERTEMPERATURE, USSReadPacket.m_USSPacketStruct.m_PZD[2]);
				setDoubleParam  (TableIndex, MOTORCURRENT, 0.1 * USSReadPacket.m_USSPacketStruct.m_PZD[3]);
				setIntegerParam (TableIndex, PUMPTEMPERATURE, USSReadPacket.m_USSPacketStruct.m_PZD[4]);
				setDoubleParam  (TableIndex, CIRCUITVOLTAGE, 0.1 * USSReadPacket.m_USSPacketStruct.m_PZD[5]);
			}
		}
		asynPrint(pasynUser, ASYN_TRACE_FLOW, "Packet success %s %s\n", __FILE__, __FUNCTION__);

	}
	catch(CException const&) {
		// Internal communication failure
		setIntegerParam(TableIndex, FAULT, 65);
	}
	callParamCallbacks(TableIndex);
	return asynPortDriver::readInt32(pasynUser, value);
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
	try {
		if (getAddress(pasynUser, &TableIndex) != asynSuccess)
			throw CException(pasynUser, __FUNCTION__, "Could not get address");
		if ((TableIndex < 0) || (TableIndex >= int(m_IOUsers.size())))
			throw CException(pasynUser, __FUNCTION__, "User / pump not configured");
		bool Running = (getIntegerParam(TableIndex, RUNNING) != 0);
		if (function == Parameters(FIRMWAREVERSION))
		{
			epicsUInt32 PWE;
			// Software version (I assume this means firmware). e.g. 3.03.05
			if (m_NoOfPZD == NoOfPZD2)
			{
				USSPacket<NoOfPZD2> USSWritePacket(Running, 2),
					USSReadPacket;
				writeRead(TableIndex, pasynUser, USSWritePacket, USSReadPacket);
				PWE = USSReadPacket.m_USSPacketStruct.m_PWE;
			}
			else
			{
				USSPacket<NoOfPZD6> USSWritePacket(Running, 2),
					USSReadPacket;
				writeRead(TableIndex, pasynUser, USSWritePacket, USSReadPacket);
				PWE = USSReadPacket.m_USSPacketStruct.m_PWE;
			}
			char CBuf[8]; // 7 chars plus null termination
			int Major=PWE / 10000,
				Minor1 = (PWE % 10000) / 100,
				Minor2 = PWE %100;
			epicsSnprintf(CBuf, sizeof(CBuf), "%1d.%02d.%02d", Major, Minor1, Minor2);
			setStringParam (TableIndex, FIRMWAREVERSION, CBuf);

			FILE* Version = fopen("version.txt", "rt");
			if (Version)
			{
				char CBuf[MaxEPICSStrLen];
				fgets(CBuf, MaxEPICSStrLen, Version);
				setStringParam (TableIndex, SOFTWAREVERSION, CBuf);
				fclose(Version);
			}
			else
				setStringParam (TableIndex, SOFTWAREVERSION, SoftwareVersion);

		}
	}
	catch(CException const&) {
		// Internal communication failure
		setIntegerParam(TableIndex, FAULT, 65);
	}
	callParamCallbacks(TableIndex);
	return asynPortDriver::readOctet(pasynUser, value, maxChars, nActual, eomReason);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
//	template<size_t NoOfPZD> void CLeyboldTurboPortDriver::writeRead(int TableIndex,			//
//		asynUser *pasynUser, USSPacket<NoOfPZD> USSWritePacket, USSPacket<NoOfPZD>& USSReadPacket)
//		size_t maxChars, size_t *nActual, int *eomReason)										//
//																								//
//	Description:																				//
//		This method is invoked by the readInt32 method to send a request (or command) packet,	//
//		and then read the controller's response													//
//		It's a wrapper round Asyn's writeRead method that provides byte-swapping and validation.//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
template<size_t NoOfPZD> void CLeyboldTurboPortDriver::writeRead(int TableIndex, asynUser *pasynUser, USSPacket<NoOfPZD> USSWritePacket, USSPacket<NoOfPZD>& USSReadPacket)
{
	int eomReason;
	size_t nBytesOut, nBytesIn;
	asynUser* IOUser = m_IOUsers[TableIndex];
#ifdef _DEBUG
	// Infinite timeout, convenient for debugging.
	const double TimeOut = -1;
#else
	const double TimeOut = 1;
#endif
	USSWritePacket.m_USSPacketStruct.HToN();
	// NB, *don't* pass pasynUser to this function - it has the wrong type and will cause an access violation.
	if (pasynOctetSyncIO->writeRead(IOUser,
		reinterpret_cast<const char*>(USSWritePacket.m_Bytes), USSPacketStruct<NoOfPZD>::USSPacketSize, 
		reinterpret_cast<char*>(USSReadPacket.m_Bytes), USSPacketStruct<NoOfPZD>::USSPacketSize,
		TimeOut, &nBytesOut, &nBytesIn, &eomReason) != asynSuccess)
		throw CException(IOUser, __FUNCTION__, "Can't write/read:");

	STATIC_ASSERT ( sizeof(USSPacketStruct<NoOfPZD>) == USSPacketStruct<NoOfPZD>::USSPacketSize );
	STATIC_ASSERT ( sizeof(USSPacket<NoOfPZD>) == USSPacketStruct<NoOfPZD>::USSPacketSize );
//	if ((sizeof(USSPacket<NoOfPZD>) != USSPacketStruct<NoOfPZD>::USSPacketSize) ||
//		(sizeof(USSPacketStruct<NoOfPZD>) != USSPacketStruct<NoOfPZD>::USSPacketSize))
//		asynPrint(pasynUser, ASYN_TRACE_ERROR, "Packet size descrepant %ul %ul %ul\n", sizeof(USSPacket<NoOfPZD>), sizeof(USSPacketStruct<NoOfPZD>), USSPacketStruct<NoOfPZD>::USSPacketSize);

	if (nBytesOut != USSPacketStruct<NoOfPZD>::USSPacketSize)
		throw CException(pasynUser, __FUNCTION__, "Incorrect packet size transmitted");
	if (nBytesIn != USSPacketStruct<NoOfPZD>::USSPacketSize)
		throw CException(pasynUser, __FUNCTION__, "Unexpected packet size recieved");

	USSReadPacket.m_USSPacketStruct.NToH();

	if (!USSReadPacket.ValidateChecksum())
		throw CException(pasynUser, __FUNCTION__, "Packet validation failed");
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
	bool Running = (getIntegerParam(TableIndex, RUNNING) != 0);

	// Normal operation 1 = the pump is running in the normal operation mode
	setIntegerParam (TableIndex, RUNNING, USSReadPacket.m_USSPacketStruct.m_PZD[0] & (1 << 10) ? 1 : 0);

	if (USSReadPacket.m_USSPacketStruct.m_PZD[0] & (1 << 3))
	{
		// We have an error status. Request the error code
		USSPacket<NoOfPZD> USSWritePacket(Running, 171), USSReadPacket;

		writeRead(TableIndex, pasynUser, USSWritePacket, USSReadPacket);

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

		setStringParam (TableIndex, FAULTSTR, ErrorStrings[USSReadPacket.m_USSPacketStruct.m_PWE]);
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

		writeRead(TableIndex, pasynUser, USSWritePacket, USSReadPacket);

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
		setIntegerParam (TableIndex, WARNINGTEMPERATURE, USSReadPacket.m_USSPacketStruct.m_PWE);
		setStringParam (TableIndex, WARNINGTEMPERATURESTR, WarningTemperatureStr);
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

		writeRead(TableIndex, pasynUser, USSWritePacket, USSReadPacket);

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
		setIntegerParam (TableIndex, WARNINGHIGHLOAD, USSReadPacket.m_USSPacketStruct.m_PWE);
		setStringParam (TableIndex, WARNINGHIGHLOADSTR, WarningHighLoadStr);
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

		writeRead(TableIndex, pasynUser, USSWritePacket, USSReadPacket);
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
		setIntegerParam (TableIndex, WARNINGPURGE, USSReadPacket.m_USSPacketStruct.m_PWE);
		setStringParam (TableIndex, WARNINGPURGESTR, WarningPurgeStr);
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
		USSWritePacket.m_USSPacketStruct.m_PZD[0] |= (value ? 1 : 0) << 0;	// Set Running bit.
	}

	if (function == Parameters(RESET))
	{
		// 0 to 1 transition = Error reset
		//
		// Is only run provided if 
		//		the cause for the error has been removed and
		//		control bit 0 = 0 and
		//		control bit 10 = 1
		bool Running = (getIntegerParam(TableIndex, RUNNING) != 0);
		if ((Running) && (value))
			throw CException(pasynUser, __FUNCTION__, "The pump must be halted before a reset can be applied");
		USSWritePacket.m_USSPacketStruct.m_PZD[0] |= 1 << 10;
		USSWritePacket.m_USSPacketStruct.m_PZD[0] |= (value ? 1 : 0) << 7;	// High
	}
	USSWritePacket.GenerateChecksum();
	writeRead(TableIndex, pasynUser, USSWritePacket, USSReadPacket);
	processRead(TableIndex, pasynUser, USSReadPacket);
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
	// Invoke the base class method to store the value in the database.
	asynStatus status = asynPortDriver::writeInt32(pasynUser, value);
	int TableIndex = 0;
	try {
		if (getAddress(pasynUser, &TableIndex) != asynSuccess)
			throw CException(pasynUser, __FUNCTION__, "Could not get address");
		if ((TableIndex < 0) || (TableIndex >= int(m_IOUsers.size())))
			throw CException(pasynUser, __FUNCTION__, "User / pump not configured");
		if (m_NoOfPZD == NoOfPZD2)
			processWrite<NoOfPZD2>(TableIndex, pasynUser, value);
		else
			processWrite<NoOfPZD6>(TableIndex, pasynUser, value);
	}
	catch(CException const&) {
		setIntegerParam(TableIndex, FAULT, 65);
	}
	callParamCallbacks(TableIndex);
	return status;
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
	delete g_LeyboldTurboPortDriver;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
//	int LeyboldTurboPortDriverConfigure(const char *asynPortName, int numPumps, int NoOfPZD)	//
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
		g_LeyboldTurboPortDriver = new CLeyboldTurboPortDriver(asynPortName, numPumps, NoOfPZD);
		epicsAtExit(LeyboldTurboExitFunc, NULL);
	}
	catch(CLeyboldTurboPortDriver::CException const&) {
	}
}

static const iocshArg addArg0 = { "IOPortName", iocshArgString};
static const iocshArg * const addArgs[] = {&addArg0};
static const iocshFuncDef addFuncDef = {"LeyboldTurboAddIOPort",1,addArgs};

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
//	int LeyboldTurboAddIOPort(const char *IOPortName)											//
//	Description:																				//
//		EPICS iocsh callable function to add a (simulated or real) pump to the IOC.				//
//																								//
//	Parameters:																					//
//		IOPortName - the IOC port name (e.g. PUMP:1) to be used.								//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
void LeyboldTurboAddIOPort(const iocshArgBuf *args)
{
	try {
		const char* IOPortName = args[0].sval;
		// Test the driver has been configured
		if (g_LeyboldTurboPortDriver)
			g_LeyboldTurboPortDriver->addIOPort(IOPortName);
	}
	catch(CLeyboldTurboPortDriver::CException const&) {
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
