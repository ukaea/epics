#include "LeyboldSimPortDriver.h"

#include <iocsh.h>
#include <epicsExit.h>
#include <epicsAssert.h>
#include <epicsThread.h>
#include <asynCommonSyncIO.h>
#include <asynOctetSyncIO.h>
#include <asynUInt32Digital.h>
#include <asynStandardInterfaces.h>

#define epicsExportSharedSymbols
#include <epicsExport.h>

#include <exception>

static const char *driverName = "LeyboldSimPortDriver";

static CLeyboldSimPortDriver* g_LeyboldSimPortDriver;

class CLeyboldSimPortDriver::CException : public std::runtime_error
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
CLeyboldSimPortDriver::CLeyboldSimPortDriver(const char *asynPortName, int numPumps)
   : asynPortDriver(asynPortName, 
                    numPumps, /* maxAddr */ 
                    NUM_PARAMS,
					asynCommonMask | asynDrvUserMask |asynOptionMask | asynInt32Mask |asynUInt32DigitalMask | asynFloat64Mask | asynOctetMask | asynGenericPointerMask | asynEnumMask,
					asynCommonMask | asynDrvUserMask |asynOptionMask | asynInt32Mask |asynUInt32DigitalMask | asynFloat64Mask | asynOctetMask | asynGenericPointerMask | asynEnumMask,
					ASYN_MULTIDEVICE | ASYN_CANBLOCK,
                    1, /* Autoconnect */
                    0, /* Default priority */
                    0) /* Default stack size*/    
{
}

void CallbackOctet(void *userPvt, asynUser *pasynUser,
                      char *data,size_t numchars, int eomReason)
{
	CLeyboldSimPortDriver* This = static_cast<CLeyboldSimPortDriver*>(userPvt);
	This->readUInt32Digital(pasynUser, NULL, 1);
}

void CLeyboldSimPortDriver::addIOPort(const char* IOPortName)
{
    static const char *functionName = "addIOPort";
	for (size_t ParamIndex = 0; ParamIndex < NUM_PARAMS; ParamIndex++)
	{
		int Index;
//		char Buf[10];
//		std::string ParamName =  std::string(":") + _itoa(m_AsynUsers.size() + 1, Buf, 10) + ParameterDefns[ParamIndex].ParamName;
		std::string ParamName =  ParameterDefns[ParamIndex].ParamName;
		if (createParam(m_AsynUsers.size(), ParamName.c_str(), ParameterDefns[ParamIndex].ParamType, &Index) != asynSuccess)
			throw CException(pasynUserSelf, functionName, "createParam" + std::string(ParameterDefns[ParamIndex].ParamName));
		m_Parameters[ParamName] = Index;
		switch(ParameterDefns[ParamIndex].ParamType)
		{
			case asynParamFloat64: 
				if (setDoubleParam (Index, 0) != asynSuccess)
					throw CException(pasynUserSelf, functionName, "setDoubleParam" + std::string(ParameterDefns[ParamIndex].ParamName));
				break;
			case asynParamUInt32Digital: 
				if (setUIntDigitalParam(Index, 0, 1) != asynSuccess)
					throw CException(pasynUserSelf, functionName, "setUIntDigitalParam" + std::string(ParameterDefns[ParamIndex].ParamName));
				break;
			default: assert(false);
		}
	}

	asynUser* AsynUser;
	if (pasynOctetSyncIO->connect(IOPortName, m_AsynUsers.size(), &AsynUser, NULL) != asynSuccess)
		throw CException(pasynUserSelf, functionName, "connecting to IO port=" + std::string(IOPortName));
	connect(AsynUser);

    asynInterface* pasynOctetInterface = pasynManager->findInterface(AsynUser, asynOctetType, 1);

	asynOctet* Octet = (asynOctet*)pasynOctetInterface->pinterface;
	void      *pinterruptNode;

	Octet->registerInterruptUser(pasynOctetInterface->drvPvt, AsynUser, CallbackOctet, this, &pinterruptNode);
	 
	m_AsynUsers.push_back(AsynUser);
}

CLeyboldSimPortDriver::~CLeyboldSimPortDriver()
{
	for(size_t Index = 0; Index < m_AsynUsers.size(); Index++)
		asynStatus status = pasynOctetSyncIO->disconnect(m_AsynUsers[Index]);
}

asynStatus CLeyboldSimPortDriver::readUInt32Digital(asynUser *pasynUser, epicsUInt32 *value, epicsUInt32 mask)
{
	asynStatus status = asynSuccess;
	int function = pasynUser->reason;
	static const char *functionName = "readUInt32Digital";
	try {
		USSPacket USSWritePacket, USSReadPacket;
		STATIC_ASSERT(sizeof(USSPacket)==USSPacketSize);
		size_t nbytesOut, nbytesIn;
		int eomReason;

		size_t TableIndex = function / NUM_PARAMS;
		if (TableIndex >= m_AsynUsers.size())
			throw CException(pasynUser, functionName, "User / pump not configured");

		pasynUser = m_AsynUsers[TableIndex];

		asynInterface* pasynOctetInterface = pasynManager->findInterface(pasynUser, asynOctetType, 1);

		asynOctet* Octet = (asynOctet*)pasynOctetInterface->pinterface;

		// NB, *don't* pass pasynUser to this function - it has the wrong type and will cause an access violation.
		status = pasynOctetSyncIO->read(pasynUser, reinterpret_cast<char*>(&USSReadPacket), sizeof(USSPacket), 10, &nbytesIn, &eomReason);
		if (status == asynTimeout)
			return status;
		if (status != asynSuccess)
			throw CException(pasynUser, functionName, "Can't read:");

		bool StartStop = true;
		bool Reset = false;
		if (USSWritePacket.m_USSPacketStruct.m_PZD1 & (1 << 10))
		{
			//		control bit 10 = 1
			StartStop = (USSWritePacket.m_USSPacketStruct.m_PZD1 && (1 << 0) != 0);

			// 0 to 1 transition = Error reset Is only run provided if:
			//		the cause for the error has been removed
			//		and control bit 0 = 0 and control bit 10 = 1
			if ((USSWritePacket.m_USSPacketStruct.m_PZD1 && (1 << 7)) && (!StartStop))
				Reset = true;
		}

		epicsUInt32 Buf;
		epicsFloat64 DBuf;

		setUIntDigitalParam(m_Parameters[STARTSTOP], StartStop ? 1 : 0, 1);

		USSWritePacket.m_USSPacketStruct.m_PZD1 = 0;

		// Normal operation 1 = the pump is running in the normal operation mode
		getUIntDigitalParam(m_Parameters[STARTSTOP], &Buf, 1); USSReadPacket.m_USSPacketStruct.m_PZD1 |= (Buf << 10);

		// Remote has been activated 1 = start/stop (control bit 0) and reset(control bit 7) through serial interface is possible.
		getUIntDigitalParam(m_Parameters[RESET], &Buf, 1); USSReadPacket.m_USSPacketStruct.m_PZD1 |= (Buf << 15);


		getUIntDigitalParam(m_Parameters[FAULT], &Buf, 1); USSWritePacket.m_USSPacketStruct.m_PZD1 |= (Buf << 3);
		getUIntDigitalParam(m_Parameters[WARNINGTEMPERATURE], &Buf, 1); USSWritePacket.m_USSPacketStruct.m_PZD1 |= (Buf << 2);
		getUIntDigitalParam(m_Parameters[WARNINGHIGHLOAD], &Buf, 1); USSWritePacket.m_USSPacketStruct.m_PZD1 |= (Buf << 13);

		getUIntDigitalParam(m_Parameters[STATORFREQUENCY], &Buf, -1); USSWritePacket.m_USSPacketStruct.m_PZD2 = Buf;
		getUIntDigitalParam(m_Parameters[CONVERTERTEMPERATURE], &Buf, -1); USSWritePacket.m_USSPacketStruct.m_PZD3 = Buf;
		getDoubleParam(m_Parameters[MOTORCURRENT], &DBuf); USSWritePacket.m_USSPacketStruct.m_PZD4 = epicsUInt32(10.0 * DBuf + 0.5);
		getUIntDigitalParam(m_Parameters[PUMPTEMPERATURE], &Buf, -1); USSWritePacket.m_USSPacketStruct.m_PZD5 = Buf;
		getDoubleParam(m_Parameters[CIRCUITVOLTAGE], &DBuf); USSWritePacket.m_USSPacketStruct.m_PZD6 = epicsUInt32(10.0 * DBuf + 0.5);
		USSWritePacket.Checksum();

		// NB, *don't* pass pasynUser to this function - it has the wrong type and will cause an access violation.
		status = pasynOctetSyncIO->write(pasynUser,
			reinterpret_cast<char*>(&USSWritePacket), sizeof(USSPacket),
			1, &nbytesOut);
		if (status != asynSuccess)
			throw CException(pasynUser, functionName, "Can't write/read:");

	}
	catch(CException const&) {
		setUIntDigitalParam(m_Parameters[FAULT], 1, 1);
	}

	return status;
}

static const iocshArg initArg0 = { "asynPortName", iocshArgString};
static const iocshArg initArg1 = { "numPumps", iocshArgString};
static const iocshArg * const initArgs[] = {&initArg0, &initArg1};
static const iocshFuncDef initFuncDef = {"LeyboldSimPortDriverConfigure",2,initArgs};

void LeyboldSimExitFunc(void * param)
{
	delete g_LeyboldSimPortDriver;
}

/** EPICS iocsh callable function to call constructor for the testAsynPortDriver class.
  * \param[in] portName The name of the asyn port driver to be created.*/
int LeyboldSimPortDriverConfigure(const char *asynPortName, int numPumps)
{
	try {
		g_LeyboldSimPortDriver = new CLeyboldSimPortDriver(asynPortName, numPumps);
		epicsAtExit(LeyboldSimExitFunc, NULL);
	}
	catch(CLeyboldSimPortDriver::CException const&) {
	}
    return(asynSuccess);
}

static void initCallFunc(const iocshArgBuf *args)
{
	const char* asynPortName = args[0].sval;
	int numPumps = atoi(args[1].sval);
	LeyboldSimPortDriverConfigure(asynPortName, numPumps);
}

static const iocshArg addArg0 = { "IOPortName", iocshArgString};
static const iocshArg * const addArgs[] = {&addArg0};
static const iocshFuncDef addFuncDef = {"LeyboldSimAddIOPort",1,addArgs};

int LeyboldSimAddIOPort(const char *IOPortName)
{
	try {
		g_LeyboldSimPortDriver->addIOPort(IOPortName);
	}
	catch(CLeyboldSimPortDriver::CException const&) {
	}
    return(asynSuccess);
}

static void addPumpFunc(const iocshArgBuf *args)
{
	const char* IOPortName = args[0].sval;
	LeyboldSimAddIOPort(IOPortName);
}

static void LeyboldSimRegistrar(void)
{
    iocshRegister(&initFuncDef, initCallFunc);
    iocshRegister(&addFuncDef, addPumpFunc);
}

extern "C" {

epicsExportRegistrar(LeyboldSimRegistrar);

}
