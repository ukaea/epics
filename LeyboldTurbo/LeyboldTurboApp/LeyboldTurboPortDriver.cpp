#include "LeyboldTurboPortDriver.h"

#include <iocsh.h>
#include <epicsExit.h>
#include <epicsAssert.h>
#include <asynOctetSyncIO.h>

#define epicsExportSharedSymbols
#include <epicsExport.h>

#pragma pack(push, 1)
struct USSPacketStruct
{
	void SetDefault() {
		m_STX = 2;			// Start byte 2
		m_LGE = 22;			// LGE Length of the payload data block in bytes (bytes 3 to 22) + 2: 22 22
		m_ADR = m_Reserved = m_IND = m_BCC = 0;
		m_PKE = m_PZD1 = m_PZD2 = m_PZD3 = m_PZD4 = m_PZD5 = m_PZD6 = 0;
		m_PWE = 0;
	}
	epicsUInt8  m_STX;		// Start byte 2
	epicsUInt8  m_LGE;		// LGE Length of the payload data block in bytes (bytes 3 to 22) + 2: 22 22
	epicsUInt8  m_ADR;		// Frequency converter address RS232: 0. RS485: 0...15
	epicsUInt16 m_PKE;		// Parameter number and type of access Value (s. 2.1)
	epicsUInt8  m_Reserved;	// 0
	epicsUInt8	m_IND;		// Parameter index Value (s. 2.1)
	epicsUInt32 m_PWE;		// Parameter value Value
	union {
		//Status and control bits Value (see 2.2)
		epicsUInt16	m_PZD1;
		epicsUInt16	m_STW;
		epicsUInt16	m_ZSW;
	};
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
};

const char* StatorFrequency = "StatorFrequency";
ParameterDefn ParameterDefns[] =
{
	{StatorFrequency, asynParamFloat64}
};


/** Constructor for the testAsynPortDriver class.
  * Calls constructor for the asynPortDriver base class.
  * \param[in] portName The name of the asyn port driver to be created.
  * \param[in] maxPoints The maximum  number of points in the volt and time arrays */
CLeyboldTurboPortDriver::CLeyboldTurboPortDriver(const char *AsynPortName, const char *IOPortName) 
   : asynPortDriver(AsynPortName, 
                    1, /* maxAddr */ 
                    sizeof(ParameterDefns) / sizeof(ParameterDefn),
                    asynInt8ArrayMask | asynInt32Mask | asynFloat64Mask | asynFloat64ArrayMask | asynEnumMask | asynDrvUserMask, /* Interface mask */
                    asynInt8ArrayMask | asynInt32Mask | asynFloat64Mask | asynFloat64ArrayMask | asynEnumMask,  /* Interrupt mask */
                    0, /* asynFlags.  This driver does not block and it is not multi-device, so flag is 0 */
                    1, /* Autoconnect */
                    0, /* Default priority */
                    0) /* Default stack size*/    
{
	for (size_t ParamIndex = 0; ParamIndex < sizeof(ParameterDefns) / sizeof(ParameterDefn); ParamIndex++)
	{
		int Index;
		asynStatus status = createParam(ParameterDefns[ParamIndex].ParamName, ParameterDefns[ParamIndex].ParamType, &Index);
		m_Parameters[ParameterDefns[ParamIndex].ParamName] = Index;
		switch(ParameterDefns[ParamIndex].ParamType)
		{
			case asynParamFloat64: setDoubleParam (Index, 0); break;
			default: assert(false);
		}
	}
	m_asynUser = NULL;
	asynStatus status = pasynOctetSyncIO->connect(IOPortName, 0, &m_asynUser, NULL);
}

CLeyboldTurboPortDriver::~CLeyboldTurboPortDriver()
{
    asynStatus status = pasynManager->freeAsynUser(m_asynUser);
}

asynStatus CLeyboldTurboPortDriver::readFloat64(asynUser *pasynUser, epicsFloat64 *value)
{
    int function = pasynUser->reason;
    const char *paramName;

    /* Fetch the parameter string name for possible use in debugging */
    getParamName(function, &paramName);

	USSPacket USSWritePacket, USSReadPacket;
	assert(sizeof(USSPacket)==24);
	size_t nbytesOut, nbytesIn;
	int eomReason;

	if (strcmp(paramName, StatorFrequency)==0)
		pasynOctetSyncIO->writeRead(pasynUser,
			reinterpret_cast<const char*>(&USSWritePacket), sizeof(USSPacket), 
			reinterpret_cast<char*>(&USSReadPacket), sizeof(USSPacket),
			10, &nbytesOut, &nbytesIn, &eomReason);
	return asynSuccess;
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
	CLeyboldTurboPortDriver* LeyboldTurboPortDriver = new CLeyboldTurboPortDriver(asynPortName, IOPortName);
	epicsAtExit(LeyboldTurboExitFunc, LeyboldTurboPortDriver);
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
    iocshRegister(&initFuncDef,initCallFunc);
}

extern "C" {

epicsExportRegistrar(LeyboldTurboRegistrar);

}
