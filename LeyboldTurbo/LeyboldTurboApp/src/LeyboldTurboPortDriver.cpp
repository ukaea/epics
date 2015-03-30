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
#include <epicsThread.h>
#include <asynOctetSyncIO.h>

#include "USSPacket.h"
#include "ParameterDefns.h"

#define epicsExportSharedSymbols
#include <epicsExport.h>

#include <stdlib.h>
#include <stdexcept>

static CLeyboldTurboPortDriver* g_LeyboldTurboPortDriver;

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
//	class CLeyboldTurboPortDriver::CException : public std::runtime_error						//
//	Description:																				//
//		If an error ocurrs, an object of this type is thrown.									//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
class CLeyboldTurboPortDriver::CException : public std::runtime_error
{
public:
	CException(asynUser* AsynUser, const char* functionName, std::string const& what) : std::runtime_error(what) {
		std::string message = "%s:%s ERROR: " + what + "\n";
		asynPrint(AsynUser, ASYN_TRACE_ERROR, message.c_str(), __FILE__, functionName, AsynUser->errorMessage);
	}
};

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
   : asynPortDriver(asynPortName, 
                    numPumps, // maxAddr
                    NUM_PARAMS,
                    asynDrvUserMask | asynInt32Mask | asynFloat64Mask | asynOctetMask, // Interface mask
                    asynDrvUserMask | asynInt32Mask | asynFloat64Mask | asynOctetMask, // Interrupt mask
					ASYN_MULTIDEVICE,
                    1, // Autoconnect
                    0, // Default priority
                    0) // Default stack size
{
	m_NoOfPZD = NoOfPZD;
}

CLeyboldTurboPortDriver::~CLeyboldTurboPortDriver()
{
	asynStatus overallstatus = asynSuccess;
	for(size_t Index = 0; Index < m_AsynUsers.size(); Index++)
	{
		asynStatus status = pasynOctetSyncIO->disconnect(m_AsynUsers[Index]);
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
	for (size_t ParamIndex = 0; ParamIndex < NUM_PARAMS; ParamIndex++)
	{
		// Create parameters from the definitions.
		// These variables end up being addressed as e.g. TURBO:1:RUNNING.
		int Index;
		std::string const& ParamName =  ParameterDefns[ParamIndex].ParamName;
		if (createParam(int(m_AsynUsers.size()), ParamName.c_str(), ParameterDefns[ParamIndex].ParamType, &Index) != asynSuccess)
			throw CException(pasynUserSelf, __FUNCTION__, "createParam" + std::string(ParameterDefns[ParamIndex].ParamName));
		m_Parameters[ParamName] = Index;
		switch(ParameterDefns[ParamIndex].ParamType)
		{
			// Set default values.
			case asynParamInt32: 
				if (setIntegerParam(int(m_AsynUsers.size()), Index, 0) != asynSuccess)
					throw CException(pasynUserSelf, __FUNCTION__, "setIntegerParam" + std::string(ParameterDefns[ParamIndex].ParamName));
				break;
			case asynParamFloat64: 
				if (setDoubleParam (int(m_AsynUsers.size()), Index, 0) != asynSuccess)
					throw CException(pasynUserSelf, __FUNCTION__, "setDoubleParam" + std::string(ParameterDefns[ParamIndex].ParamName));
				break;
			default: assert(false);
		}
	}

	asynUser* IOUser;
	// Connect to the I/O port.
	if (pasynOctetSyncIO->connect(IOPortName, int(m_AsynUsers.size()), &IOUser, NULL) != asynSuccess)
		throw CException(pasynUserSelf, __FUNCTION__, "connecting to IO port=" + std::string(IOPortName));

	if (callParamCallbacks() != asynSuccess)
		// Make sure the (just set) default values are available to read.
		throw CException(pasynUserSelf, __FUNCTION__, "callParamCallbacks");
	m_AsynUsers.push_back(IOUser);
}
 
//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
//	asynStatus CLeyboldTurboPortDriver::readInt32(asynUser *pasynUser, epicsInt32 *value)		//
//																								//
//	Description:																				//
//		This method is invoked by the Asyn framework to read values from the hardware.			//
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
		if ((TableIndex < 0) || (TableIndex >= int(m_AsynUsers.size())))
			throw CException(pasynUser, __FUNCTION__, "User / pump not configured");


		if (function != m_Parameters[FIRMWAREVERSION])
		{
			if (m_NoOfPZD == NoOfPZD2)
			{
				USSPacket<NoOfPZD2> USSWritePacket(2), // Software version
					USSReadPacket;
				writeRead(TableIndex, pasynUser, USSWritePacket, USSReadPacket);
				if (setIntegerParam (TableIndex, m_Parameters[FIRMWAREVERSION], USSReadPacket.m_USSPacketStruct.m_PWE) != asynSuccess)
					throw CException(pasynUser, __FUNCTION__, "Can't set parameter");
			}
			else
			{
				USSPacket<NoOfPZD6> USSWritePacket(2), // Software version
					USSReadPacket;
				writeRead(TableIndex, pasynUser, USSWritePacket, USSReadPacket);
				if (setIntegerParam (TableIndex, m_Parameters[FIRMWAREVERSION], USSReadPacket.m_USSPacketStruct.m_PWE) != asynSuccess)
					throw CException(pasynUser, __FUNCTION__, "Can't set parameter");
			}
		}
		else if (function == m_Parameters[FAULT])
		{
			if (m_NoOfPZD == NoOfPZD2)
			{
				USSPacket<NoOfPZD2> USSWritePacket, USSReadPacket;
				process(TableIndex, pasynUser, USSWritePacket, USSReadPacket);

				{
					USSPacket<NoOfPZD2> USSWritePacket(3), // Frequency - actual value
						USSReadPacket;
					writeRead(TableIndex, pasynUser, USSWritePacket, USSReadPacket);
					if (setIntegerParam (TableIndex, m_Parameters[STATORFREQUENCY], USSReadPacket.m_USSPacketStruct.m_PWE) != asynSuccess)
						throw CException(pasynUser, __FUNCTION__, "Can't set parameter");
				}
				{
					USSPacket<NoOfPZD2> USSWritePacket(11), // Converter temperature - actual value
						USSReadPacket;
					writeRead(TableIndex, pasynUser, USSWritePacket, USSReadPacket);
					if (setIntegerParam (TableIndex, m_Parameters[CONVERTERTEMPERATURE], USSReadPacket.m_USSPacketStruct.m_PWE) != asynSuccess)
						throw CException(pasynUser, __FUNCTION__, "Can't set parameter");
				}
				{
					USSPacket<NoOfPZD2> USSWritePacket(7), // Converter temperature - actual value
						USSReadPacket;
					writeRead(TableIndex, pasynUser, USSWritePacket, USSReadPacket);
					if (setIntegerParam (TableIndex, m_Parameters[PUMPTEMPERATURE], USSReadPacket.m_USSPacketStruct.m_PWE) != asynSuccess)
						throw CException(pasynUser, __FUNCTION__, "Can't set parameter");
				}
				{
					USSPacket<NoOfPZD2> USSWritePacket(5), // Motor current - actual value
						USSReadPacket;
					writeRead(TableIndex, pasynUser, USSWritePacket, USSReadPacket);
					if (setIntegerParam (TableIndex, m_Parameters[MOTORCURRENT], USSReadPacket.m_USSPacketStruct.m_PWE) != asynSuccess)
						throw CException(pasynUser, __FUNCTION__, "Can't set parameter");
				}
				{
					USSPacket<NoOfPZD2> USSWritePacket(4), // Intermediate circuit voltage Uzk
						USSReadPacket;
					writeRead(TableIndex, pasynUser, USSWritePacket, USSReadPacket);
					if (setIntegerParam (TableIndex, m_Parameters[CIRCUITVOLTAGE], USSReadPacket.m_USSPacketStruct.m_PWE) != asynSuccess)
						throw CException(pasynUser, __FUNCTION__, "Can't set parameter");
				}
			}
			else
			{
				USSPacket<NoOfPZD6> USSWritePacket, USSReadPacket;
				process(TableIndex, pasynUser, USSWritePacket, USSReadPacket);
			}
		}

	}
	catch(CException const&) {
		// Internal communication failure
		setIntegerParam(TableIndex, m_Parameters[FAULT], 65);
	}
	callParamCallbacks(TableIndex);
	return asynPortDriver::readInt32(pasynUser, value);
}

template<size_t NoOfPZD> void CLeyboldTurboPortDriver::writeRead(int TableIndex, asynUser *pasynUser, USSPacket<NoOfPZD> USSWritePacket, USSPacket<NoOfPZD>& USSReadPacket)
{
	int eomReason;
	size_t nbytesOut, nbytesIn;
	asynUser* IOUser = m_AsynUsers[TableIndex];
#ifdef _DEBUG
	// Infinite timeout, convenient for debugging.
	const double TimeOut = -1;
#else
	const double TimeOut = 1;
#endif
	USSWritePacket.m_USSPacketStruct.HToN();
	// NB, *don't* pass pasynUser to this function - it has the wrong type and will cause an access violation.
	if (pasynOctetSyncIO->writeRead(IOUser,
		reinterpret_cast<const char*>(USSWritePacket.m_Bytes), USSPacket<NoOfPZD>::USSPacketSize, 
		reinterpret_cast<char*>(USSReadPacket.m_Bytes), USSPacket<NoOfPZD>::USSPacketSize,
		TimeOut, &nbytesOut, &nbytesIn, &eomReason) != asynSuccess)
		throw CException(IOUser, __FUNCTION__, "Can't write/read:");
	USSReadPacket.m_USSPacketStruct.NToH();

	if (!USSReadPacket.ValidateChecksum())
		throw CException(pasynUser, __FUNCTION__, "Packet validation failed");
}

template<size_t NoOfPZD> void CLeyboldTurboPortDriver::processRead(int TableIndex, asynUser *pasynUser, USSPacket<NoOfPZD> const& USSReadPacket)
{
	// Normal operation 1 = the pump is running in the normal operation mode
	if (setIntegerParam (TableIndex, m_Parameters[RUNNING], USSReadPacket.m_USSPacketStruct.m_PZD[0] & (1 << 10) ? 1 : 0) != asynSuccess)
		throw CException(pasynUser, __FUNCTION__, "Can't set parameter");

	// Remote has been activated 1 = start/stop (control bit 0) and reset(control bit 7) through serial interface is possible.
	if (setIntegerParam (TableIndex, m_Parameters[RESET], USSReadPacket.m_USSPacketStruct.m_PZD[0] & (1 << 15) ? 1 : 0) != asynSuccess)
		throw CException(pasynUser, __FUNCTION__, "Can't set parameter");

	if (USSReadPacket.m_USSPacketStruct.m_PZD[0] & (1 << 3))
	{
		// We have an error status. Request the error code
		USSPacket<NoOfPZD> USSWritePacket(171), USSReadPacket;

		writeRead(TableIndex, pasynUser, USSWritePacket, USSReadPacket);

		if (setIntegerParam (TableIndex, m_Parameters[FAULT], USSReadPacket.m_USSPacketStruct.m_PWE) != asynSuccess)
			throw CException(pasynUser, __FUNCTION__, "Can't set parameter");
	}

	if (USSReadPacket.m_USSPacketStruct.m_PZD[0] & (1 << 7))
	{
		// We have a temperature warning status. Request the warning bits.
		USSPacket<NoOfPZD> USSWritePacket(227), USSReadPacket;

		writeRead(TableIndex, pasynUser, USSWritePacket, USSReadPacket);

		if (setIntegerParam (TableIndex, m_Parameters[WARNINGTEMPERATURE], USSReadPacket.m_USSPacketStruct.m_PWE) != asynSuccess)
			throw CException(pasynUser, __FUNCTION__, "Can't set parameter");
	}

	if (USSReadPacket.m_USSPacketStruct.m_PZD[0] & (1 << 13))
	{
		// We have a high load warning status. Request the warning bits.
		USSPacket<NoOfPZD> USSWritePacket(230), USSReadPacket;

		writeRead(TableIndex, pasynUser, USSWritePacket, USSReadPacket);

		if (setIntegerParam (TableIndex, m_Parameters[WARNINGHIGHLOAD], USSReadPacket.m_USSPacketStruct.m_PWE) != asynSuccess)
			throw CException(pasynUser, __FUNCTION__, "Can't set parameter");
	}

	if (setIntegerParam (TableIndex, m_Parameters[STATORFREQUENCY], USSReadPacket.m_USSPacketStruct.m_PZD[1]) != asynSuccess)
		throw CException(pasynUser, __FUNCTION__, "Can't set parameter");

}

template void CLeyboldTurboPortDriver::processRead<CLeyboldTurboPortDriver::NoOfPZD2>(int TableIndex, asynUser *pasynUser, USSPacket<NoOfPZD2> const& USSReadPacket);
template void CLeyboldTurboPortDriver::processRead<CLeyboldTurboPortDriver::NoOfPZD6>(int TableIndex, asynUser *pasynUser, USSPacket<NoOfPZD6> const& USSReadPacket);

void CLeyboldTurboPortDriver::process(int TableIndex, asynUser *pasynUser, USSPacket<NoOfPZD2> const& USSWritePacket, USSPacket<NoOfPZD2>& USSReadPacket)
{
	writeRead(TableIndex, pasynUser, USSWritePacket, USSReadPacket);
	processRead(TableIndex, pasynUser, USSReadPacket);
}

void CLeyboldTurboPortDriver::process(int TableIndex, asynUser *pasynUser, USSPacket<NoOfPZD6> const& USSWritePacket, USSPacket<NoOfPZD6>& USSReadPacket)
{
	writeRead(TableIndex, pasynUser, USSWritePacket, USSReadPacket);

	processRead(TableIndex, pasynUser, USSReadPacket);

	if (setIntegerParam (TableIndex, m_Parameters[CONVERTERTEMPERATURE], USSReadPacket.m_USSPacketStruct.m_PZD[2]) != asynSuccess)
		throw CException(pasynUser, __FUNCTION__, "Can't set parameter");
	if (setDoubleParam  (TableIndex, m_Parameters[MOTORCURRENT], 0.1 * USSReadPacket.m_USSPacketStruct.m_PZD[3]) != asynSuccess)
		throw CException(pasynUser, __FUNCTION__, "Can't set parameter");
	if (setIntegerParam (TableIndex, m_Parameters[PUMPTEMPERATURE], USSReadPacket.m_USSPacketStruct.m_PZD[4]) != asynSuccess)
		throw CException(pasynUser, __FUNCTION__, "Can't set parameter");
	if (setDoubleParam  (TableIndex, m_Parameters[CIRCUITVOLTAGE], 0.1 * USSReadPacket.m_USSPacketStruct.m_PZD[5]) != asynSuccess)
		throw CException(pasynUser, __FUNCTION__, "Can't set parameter");

	asynPrint(pasynUser, ASYN_TRACE_FLOW, "Packet success %s %s\n", __FILE__, __FUNCTION__);
}

template<size_t NoOfPZD> void CLeyboldTurboPortDriver::processWrite(int TableIndex, asynUser *pasynUser, epicsInt32 value)
{
	USSPacket<NoOfPZD> USSWritePacket, USSReadPacket;

	asynUser* IOUser = m_AsynUsers[TableIndex];
	int function = pasynUser->reason;

	if (function == m_Parameters[RUNNING])
	{
		// 1 = Start; 0 = Stop
		// Is only run provided if
		//		no error is present and
		//		control bit 10 = 1
		USSWritePacket.m_USSPacketStruct.m_PZD[0] |= 1 << 10;
		USSWritePacket.m_USSPacketStruct.m_PZD[0] |= (value ? 1 : 0) << 0; // Set Running bit.
	}

	if (function == m_Parameters[RESET])
	{
		int IBuf;
		// Normal operation 1 = the pump is running in the normal operation mode
		if (getIntegerParam(TableIndex, m_Parameters[RUNNING], &IBuf) != asynSuccess)
			throw CException(pasynUser, __FUNCTION__, "Can't get parameter");
		bool Running = (IBuf != 0);
		// 0 to 1 transition = Error reset
		//
		// Is only run provided if 
		//		the cause for the error has been removed and
		//		control bit 0 = 0 and
		//		control bit 10 = 1
		USSWritePacket.m_USSPacketStruct.m_PZD[0] |= 1 << 10;
		USSWritePacket.m_USSPacketStruct.m_PZD[0] |= (Running ? 1 : 0) << 0; // Set Running bit.
		USSWritePacket.m_USSPacketStruct.m_PZD[0] |= (value ? 1 : 0) << 7; // High
	}
	USSWritePacket.GenerateChecksum();
	process(TableIndex, pasynUser, USSWritePacket, USSReadPacket);
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
	asynStatus status = asynPortDriver::writeInt32(pasynUser, value);
	int TableIndex = 0;
	try {
		if (getAddress(pasynUser, &TableIndex) != asynSuccess)
			throw CException(pasynUser, __FUNCTION__, "Could not get address");
		if ((TableIndex < 0) || (TableIndex >= int(m_AsynUsers.size())))
			throw CException(pasynUser, __FUNCTION__, "User / pump not configured");
		if (m_NoOfPZD == NoOfPZD2)
			processWrite<NoOfPZD2>(TableIndex, pasynUser, value);
		else
			processWrite<NoOfPZD6>(TableIndex, pasynUser, value);
	}
	catch(CException const&) {
		setIntegerParam(TableIndex, m_Parameters[FAULT], 1);
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
//		asynPortName. This is the Asyn port name (e.g. TURBO).									//
//					This will have been mapped by drvAsynIPPortConfigure to a physical port,	//
//					e.g. COM1: (or /dev/ttyS0).													//
//		numPumps - how many pumps will be attached?												//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
int LeyboldTurboPortDriverConfigure(const char *asynPortName, int numPumps, int NoOfPZD)
{
	try {
		g_LeyboldTurboPortDriver = new CLeyboldTurboPortDriver(asynPortName, numPumps, NoOfPZD);
		epicsAtExit(LeyboldTurboExitFunc, NULL);
	}
	catch(CLeyboldTurboPortDriver::CException const&) {
	}
    return(asynSuccess);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
//	static void initCallFunc(const iocshArgBuf *args)											//
//																								//
//	Description:																				//
//		Registered function that configures the IOC.											//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
static void initCallFunc(const iocshArgBuf *args)
{
	const char* asynPortName = args[0].sval;
	int numPumps = atoi(args[1].sval);
	int NoOfPZD = atoi(args[2].sval);
	LeyboldTurboPortDriverConfigure(asynPortName, numPumps, NoOfPZD);
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
int LeyboldTurboAddIOPort(const char *IOPortName)
{
	try {
		g_LeyboldTurboPortDriver->addIOPort(IOPortName);
	}
	catch(CLeyboldTurboPortDriver::CException const&) {
	}
    return(asynSuccess);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
//	static void addPumpFunc(const iocshArgBuf *args)											//
//																								//
//	Description:																				//
//		Registered function that adds a (simulated or real) pump to the IOC.					//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
static void addPumpFunc(const iocshArgBuf *args)
{
	const char* IOPortName = args[0].sval;
	LeyboldTurboAddIOPort(IOPortName);
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
    iocshRegister(&initFuncDef, initCallFunc);
    iocshRegister(&addFuncDef, addPumpFunc);
}

extern "C" {

epicsExportRegistrar(LeyboldTurboRegistrar);

}
