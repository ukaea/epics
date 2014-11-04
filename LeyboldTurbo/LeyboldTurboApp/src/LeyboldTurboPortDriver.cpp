#include "LeyboldTurboPortDriver.h"

#include <iocsh.h>
#include <epicsExit.h>
#include <epicsAssert.h>
#include <epicsThread.h>
#include <asynOctetSyncIO.h>

#define epicsExportSharedSymbols
#include <epicsExport.h>

#include <exception>

static const char *driverName = "LeyboldTurboPortDriver";

class CLeyboldTurboPortDriver::CException : public std::runtime_error
{
public:
	CException(asynUser* AsynUser, const char* functionName, std::string const& what) : std::runtime_error(what) {
		std::string message = "%s:%s ERROR: " + what + "\n";
		asynPrint(AsynUser, ASYN_TRACE_ERROR, message.c_str(), driverName, functionName, AsynUser->errorMessage);
	}
};

#pragma pack(push, 1)
struct USSPacketStruct
{
	void SetDefault() {
		m_STX = 2;			// Start byte 2
		m_LGE = 22;			// LGE Length of the payload data block in bytes (bytes 3 to 22) + 2: 22
		m_ADR = m_Reserved = m_IND = m_BCC = 0;
		m_PKE = m_PZD1 = m_PZD2 = m_PZD3 = m_PZD4 = m_PZD5 = m_PZD6 = 0;
		m_PWE = 0;
	}
	epicsUInt8  m_STX;		// Start byte 2
	epicsUInt8  m_LGE;		// LGE Length of the payload data block in bytes (bytes 3 to 22) + 2: 22
	epicsUInt8  m_ADR;		// Frequency converter address RS232: 0. RS485: 0...15
	epicsUInt16 m_PKE;		// Parameter number and type of access Value (s. 2.1)
	epicsUInt8  m_Reserved;	// 0
	epicsUInt8	m_IND;		// Parameter index Value (s. 2.1)
	epicsUInt32 m_PWE;		// Parameter value Value
	epicsUInt16	m_PZD1;		// Status and control bits Value (see 2.2)
	epicsUInt16	m_PZD2;		// Current stator frequency (= P3) Value (Hz)
	epicsUInt16 m_PZD3;		// Current frequency converter temperature (= P11) Value (°C)
	epicsUInt16 m_PZD4;		// Current motor current (= P5) Value (0.1 A)
	epicsUInt16 m_PZD5;		// Current pump temperature (= P127) Value (°C)
	epicsUInt16 m_PZD6;		// Current intermediate circuit voltage (=P4) Value (0.1 V)
	epicsUInt8	m_BCC;		// Recursive calculation:
};
#pragma pack(pop)

static const size_t USSPacketSize = 24;

union USSPacket
{
	USSPacket() {
		m_USSPacketStruct.SetDefault();
		Checksum();
	}
	USSPacketStruct m_USSPacketStruct;
	epicsUInt8 m_Bytes[USSPacketSize];
	void Checksum() {
		epicsUInt8 Checksum = 0;
		for (size_t i = 1; i < 22; i++)
			Checksum = Checksum ^ m_Bytes[i];
		m_USSPacketStruct.m_BCC = Checksum;
	}
};

struct ParameterDefn
{
	const char* ParamName;
	asynParamType ParamType;
	bool InParamerter;
};

const char* STARTSTOP = "STARTSTOP"; 
const char* RESET = "RESET";
const char* FAULT = "FAULT";
const char* WARNINGTEMPERATURE = "WARNINGTEMPERATURE";
const char* WARNINGHIGHLOAD = "WARNINGHIGHLOAD";
const char* STATORFREQUENCY = "STATORFREQUENCY";
const char* CONVERTERTEMPERATURE = "CONVERTERTEMPERATURE";
const char* MOTORCURRENT = "MOTORCURRENT";
const char* PUMPTEMPERATURE = "PUMPTEMPERATURE";
const char* CIRCUITVOLTAGE = "CIRCUITVOLTAGE";

ParameterDefn ParameterDefns[] =
{
	{STARTSTOP, asynParamUInt32Digital, false},
	{RESET, asynParamUInt32Digital, false},
	{FAULT, asynParamUInt32Digital, false},
	{WARNINGTEMPERATURE, asynParamUInt32Digital, true},
	{WARNINGHIGHLOAD, asynParamUInt32Digital, true},
	{STATORFREQUENCY, asynParamFloat64, true},
	{CONVERTERTEMPERATURE, asynParamFloat64, true},
	{MOTORCURRENT, asynParamFloat64, true},
	{PUMPTEMPERATURE, asynParamFloat64, true},
	{CIRCUITVOLTAGE, asynParamFloat64, true},
};

static const int NUM_PARAMS = sizeof(ParameterDefns) / sizeof(ParameterDefn);

/** Constructor for the testAsynPortDriver class.
  * Calls constructor for the asynPortDriver base class.
  * \param[in] portName The name of the asyn port driver to be created.
  * \param[in] maxPoints The maximum  number of points in the volt and time arrays */
CLeyboldTurboPortDriver::CLeyboldTurboPortDriver(const char *AsynPortName, const char* IOPortName)
   : asynPortDriver(AsynPortName, 
                    1, /* maxAddr */ 
                    NUM_PARAMS,
                    asynUInt32DigitalMask | asynInt8ArrayMask | asynInt32Mask | asynFloat64Mask | asynFloat64ArrayMask | asynEnumMask | asynDrvUserMask, /* Interface mask */
                    asynInt8ArrayMask | asynInt32Mask | asynFloat64Mask | asynFloat64ArrayMask | asynEnumMask,  /* Interrupt mask */
                    0, /* asynFlags.  This driver does not block and it is not multi-device, so flag is 0 */
                    1, /* Autoconnect */
                    0, /* Default priority */
                    0) /* Default stack size*/    
{
    static const char *functionName = "CLeyboldTurboPortDriver";
	m_AsynUser = NULL;
	if (pasynOctetSyncIO->connect(IOPortName, 0, &m_AsynUser, NULL) != asynSuccess)
		throw CException(m_AsynUser, functionName, "connecting to IO port=" + std::string(IOPortName));
	for (size_t ParamIndex = 0; ParamIndex < NUM_PARAMS; ParamIndex++)
	{
		int Index;
		if (createParam(ParameterDefns[ParamIndex].ParamName, ParameterDefns[ParamIndex].ParamType, &Index) != asynSuccess)
			throw CException(m_AsynUser, functionName, "createParam" + std::string(ParameterDefns[ParamIndex].ParamName));
		m_Parameters[ParameterDefns[ParamIndex].ParamName] = Index;
		switch(ParameterDefns[ParamIndex].ParamType)
		{
			case asynParamFloat64: 
				if (setDoubleParam (Index, 0) != asynSuccess)
					throw CException(m_AsynUser, functionName, "setDoubleParam" + std::string(ParameterDefns[ParamIndex].ParamName));
				break;
			case asynParamUInt32Digital: 
				if (setUIntDigitalParam(Index, 0, 1) != asynSuccess)
					throw CException(m_AsynUser, functionName, "setUIntDigitalParam" + std::string(ParameterDefns[ParamIndex].ParamName));
				break;
			default: assert(false);
		}
	}
}

CLeyboldTurboPortDriver::~CLeyboldTurboPortDriver()
{
	if (m_AsynUser)
		asynStatus status = pasynOctetSyncIO->disconnect(m_AsynUser);
}

asynStatus CLeyboldTurboPortDriver::readFloat64(asynUser* pasynUser, epicsFloat64* value)
{
	asynStatus status = asynSuccess;
	int function = pasynUser->reason;
	static const char *functionName = "readFloat64";
	try {
		USSPacket USSWritePacket, USSReadPacket;
		STATIC_ASSERT(sizeof(USSPacket)==USSPacketSize);
		size_t nbytesOut, nbytesIn;
		int eomReason;

		// NB, *don't* pass pasynUser to this function - it has the wrong type and will cause an access violation.
		status = pasynOctetSyncIO->writeRead(m_AsynUser,
			reinterpret_cast<const char*>(&USSWritePacket), sizeof(USSPacket), 
			reinterpret_cast<char*>(&USSReadPacket), sizeof(USSPacket),
			1, &nbytesOut, &nbytesIn, &eomReason);
		if (status != asynSuccess)
			throw CException(m_AsynUser, functionName, "Can't write/read:");

		setUIntDigitalParam(m_Parameters[FAULT], USSReadPacket.m_USSPacketStruct.m_PZD1 & (1 << 3) ? 1 : 0, 1);
		setUIntDigitalParam(m_Parameters[WARNINGTEMPERATURE], USSReadPacket.m_USSPacketStruct.m_PZD1 & (1 << 2) ? 1 : 0, 1);
		setUIntDigitalParam(m_Parameters[WARNINGHIGHLOAD], USSReadPacket.m_USSPacketStruct.m_PZD1 & (1 << 13) ? 1 : 0, 1);
		setDoubleParam (m_Parameters[STATORFREQUENCY], USSReadPacket.m_USSPacketStruct.m_PZD2);
		setDoubleParam (m_Parameters[CONVERTERTEMPERATURE], USSReadPacket.m_USSPacketStruct.m_PZD3);
		setDoubleParam (m_Parameters[MOTORCURRENT], 0.1 * USSReadPacket.m_USSPacketStruct.m_PZD4);
		setDoubleParam (m_Parameters[PUMPTEMPERATURE], USSReadPacket.m_USSPacketStruct.m_PZD5);
		setDoubleParam (m_Parameters[CIRCUITVOLTAGE], 0.1 * USSReadPacket.m_USSPacketStruct.m_PZD6);
	}
	catch(CException const&) {
		setUIntDigitalParam(m_Parameters[FAULT], 1, 1);
	}

	return status;
}

asynStatus CLeyboldTurboPortDriver::writeUInt32Digital(asynUser *pasynUser, epicsUInt32 value, epicsUInt32 mask)
{
	asynStatus status = asynSuccess;
	int function = pasynUser->reason;
	static const char *functionName = "writeUInt32Digital";
	try {
		const char *paramName;
		USSPacket USSWritePacket, USSReadPacket;
		STATIC_ASSERT(sizeof(USSPacket)==USSPacketSize);
		size_t nbytesOut, nbytesIn;
		int eomReason;

		/* Fetch the parameter string name for possible use in debugging */
		status = getParamName(function, &paramName);
		if (status != asynSuccess)
			throw CException(m_AsynUser, functionName, "Can't get parameter name:");

		if (strcmp(paramName, STARTSTOP)==0)
		{
			// 1 = Start; 0 = Stop
			// Is only run provided if
			//		no error is present and
			//		control bit 10 = 1
			epicsUInt32 value;
			status = getUIntDigitalParam(m_Parameters[STARTSTOP], &value, 1);
			if (status != asynSuccess)
				throw CException(m_AsynUser, functionName, "getUIntDigitalParam");
			USSWritePacket.m_USSPacketStruct.m_PZD1 |= 1 << 10;
			USSWritePacket.m_USSPacketStruct.m_PZD1 |= value ? 1 : 0; // Set StartStop bit.
			USSWritePacket.Checksum();
			status = pasynOctetSyncIO->writeRead(m_AsynUser,
				reinterpret_cast<const char*>(&USSWritePacket), sizeof(USSPacket), 
				reinterpret_cast<char*>(&USSReadPacket), sizeof(USSPacket),
				1, &nbytesOut, &nbytesIn, &eomReason);
			if (status != asynSuccess)
				throw CException(m_AsynUser, functionName, "Can't write/read:");
		}

		if (strcmp(paramName, RESET)==0)
		{
			// 0 to 1 transition = Error reset
			//
			// Is only run provided if 
			//		the cause for the error has been removed and
			//		control bit 0 = 0 and
			//		control bit 10 = 1
			USSWritePacket.m_USSPacketStruct.m_PZD1 |= 1 << 10;
			USSWritePacket.m_USSPacketStruct.m_PZD1 |= 1 << 7; // High
			USSWritePacket.Checksum();
			status = pasynOctetSyncIO->writeRead(m_AsynUser,
				reinterpret_cast<const char*>(&USSWritePacket), sizeof(USSPacket), 
				reinterpret_cast<char*>(&USSReadPacket), sizeof(USSPacket),
				1, &nbytesOut, &nbytesIn, &eomReason);
			if (status != asynSuccess)
				throw CException(m_AsynUser, functionName, "Can't write/read:");
			epicsThreadSleep(.1);
			USSWritePacket.m_USSPacketStruct.m_PZD1 &= ~(1 << 7);	// Low
			USSWritePacket.Checksum();
			status = pasynOctetSyncIO->writeRead(m_AsynUser,
				reinterpret_cast<const char*>(&USSWritePacket), sizeof(USSPacket), 
				reinterpret_cast<char*>(&USSReadPacket), sizeof(USSPacket),
				1, &nbytesOut, &nbytesIn, &eomReason);
			if (status != asynSuccess)
				throw CException(m_AsynUser, functionName, "Can't write/read:");
			// Reset the variable to 0.
			setUIntDigitalParam(m_Parameters[RESET], 0, 1);
			if (status != asynSuccess)
				throw CException(m_AsynUser, functionName, "setUIntDigitalParam");
		}
	}
	catch(CException const&) {
		setUIntDigitalParam(m_Parameters[FAULT], 1, 1);
	}
	return status;
}

static const iocshArg initArg0 = { "asynPortName", iocshArgString};
static const iocshArg initArg1 = { "IOPortName", iocshArgString};
static const iocshArg * const initArgs[] = {&initArg0, &initArg1};
static const iocshFuncDef initFuncDef = {"LeyboldTurboPortDriverConfigure",2,initArgs};

void LeyboldTurboExitFunc(void * param)
{
	CLeyboldTurboPortDriver* LeyboldTurboPortDriver = static_cast<CLeyboldTurboPortDriver*>(param);
	delete LeyboldTurboPortDriver;
}

/** EPICS iocsh callable function to call constructor for the testAsynPortDriver class.
  * \param[in] portName The name of the asyn port driver to be created.*/
int LeyboldTurboPortDriverConfigure(const char *asynPortName, const char* IOPortName)
{
	try {
		CLeyboldTurboPortDriver* LeyboldTurboPortDriver = new CLeyboldTurboPortDriver(asynPortName, IOPortName);
		epicsAtExit(LeyboldTurboExitFunc, LeyboldTurboPortDriver);
	}
	catch(CLeyboldTurboPortDriver::CException const&) {
	}
    return(asynSuccess);
}

static void initCallFunc(const iocshArgBuf *args)
{
	const char* asynPortName = args[0].sval;
	const char* IOPortName = args[1].sval;
	LeyboldTurboPortDriverConfigure(asynPortName, IOPortName);
}

static void LeyboldTurboRegistrar(void)
{
    iocshRegister(&initFuncDef, initCallFunc);
}

extern "C" {

epicsExportRegistrar(LeyboldTurboRegistrar);

}
