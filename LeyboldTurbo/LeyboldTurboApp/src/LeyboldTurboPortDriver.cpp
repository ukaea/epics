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

#include <exception>

static CLeyboldTurboPortDriver* g_LeyboldTurboPortDriver;

class CLeyboldTurboPortDriver::CException : public std::runtime_error
{
public:
	CException(asynUser* AsynUser, const char* functionName, std::string const& what) : std::runtime_error(what) {
		std::string message = "%s:%s ERROR: " + what + "\n";
		asynPrint(AsynUser, ASYN_TRACE_ERROR, message.c_str(), __FILE__, functionName, AsynUser->errorMessage);
	}
};

/** Constructor for the testAsynPortDriver class.
  * Calls constructor for the asynPortDriver base class.
  * \param[in] portName The name of the asyn port driver to be created.
  * \param[in] maxPoints The maximum  number of points in the volt and time arrays */
CLeyboldTurboPortDriver::CLeyboldTurboPortDriver(const char *asynPortName, int numPumps)
   : asynPortDriver(asynPortName, 
                    numPumps, /* maxAddr */ 
                    NUM_PARAMS,
                    asynDrvUserMask | asynInt32Mask | asynFloat64Mask | asynOctetMask, /* Interface mask */
                    asynDrvUserMask | asynInt32Mask | asynFloat64Mask | asynOctetMask, /* Interrupt mask */
					ASYN_MULTIDEVICE,
                    1, /* Autoconnect */
                    0, /* Default priority */
                    0) /* Default stack size*/    
{
}

void CLeyboldTurboPortDriver::addIOPort(const char* IOPortName)
{
	for (size_t ParamIndex = 0; ParamIndex < NUM_PARAMS; ParamIndex++)
	{
		int Index;
		std::string ParamName =  ParameterDefns[ParamIndex].ParamName;
		if (createParam(m_AsynUsers.size(), ParamName.c_str(), ParameterDefns[ParamIndex].ParamType, &Index) != asynSuccess)
			throw CException(pasynUserSelf, __FUNCTION__, "createParam" + std::string(ParameterDefns[ParamIndex].ParamName));
		m_Parameters[ParamName] = Index;
		switch(ParameterDefns[ParamIndex].ParamType)
		{
			case asynParamInt32: 
				if (setIntegerParam(m_AsynUsers.size(), Index, ParameterDefns[ParamIndex].DefaultValue) != asynSuccess)
					throw CException(pasynUserSelf, __FUNCTION__, "setIntegerParam" + std::string(ParameterDefns[ParamIndex].ParamName));
				break;
			case asynParamFloat64: 
				if (setDoubleParam (m_AsynUsers.size(), Index, ParameterDefns[ParamIndex].DefaultValue) != asynSuccess)
					throw CException(pasynUserSelf, __FUNCTION__, "setDoubleParam" + std::string(ParameterDefns[ParamIndex].ParamName));
				break;
			default: assert(false);
		}
	}
	asynUser* IOUser;
	if (pasynOctetSyncIO->connect(IOPortName, m_AsynUsers.size(), &IOUser, NULL) != asynSuccess)
		throw CException(pasynUserSelf, __FUNCTION__, "connecting to IO port=" + std::string(IOPortName));
//	connect(AsynUser);
	m_AsynUsers.push_back(IOUser);
}

CLeyboldTurboPortDriver::~CLeyboldTurboPortDriver()
{
	for(size_t Index = 0; Index < m_AsynUsers.size(); Index++)
		asynStatus status = pasynOctetSyncIO->disconnect(m_AsynUsers[Index]);
}

asynStatus CLeyboldTurboPortDriver::readInt32(asynUser *pasynUser, epicsInt32 *value)
{
	int function = pasynUser->reason;
	size_t TableIndex = function / NUM_PARAMS;
	if (TableIndex >= m_AsynUsers.size())
		throw CException(pasynUser, __FUNCTION__, "User / pump not configured");

	try {
		USSPacket USSWritePacket, USSReadPacket;
		STATIC_ASSERT(sizeof(USSPacket)==USSPacketSize);
		size_t nbytesOut, nbytesIn;
		int eomReason;
		const char *paramName;

		if (getParamName(function, &paramName) != asynSuccess)
			throw CException(pasynUser, __FUNCTION__, "Can't get parameter name:");

		if (std::string(paramName) != FAULT)
			return asynPortDriver::readInt32(pasynUser, value);

		asynUser* IOUser = m_AsynUsers[TableIndex];

		USSWritePacket.GenerateChecksum();
		USSWritePacket.m_USSPacketStruct.HToN();
		// NB, *don't* pass pasynUser to this function - it has the wrong type and will cause an access violation.
		if (pasynOctetSyncIO->writeRead(IOUser,
			reinterpret_cast<const char*>(&USSWritePacket), sizeof(USSPacket), 
			reinterpret_cast<char*>(&USSReadPacket), sizeof(USSPacket),
			1, &nbytesOut, &nbytesIn, &eomReason) != asynSuccess)
			throw CException(pasynUser, __FUNCTION__, "Can't write/read:");
		USSReadPacket.m_USSPacketStruct.NToH();

		if (!USSReadPacket.ValidateChecksum())
		{
			asynPrint(pasynUser, ASYN_TRACE_WARNING, "Packet validation failed", __FILE__, __FUNCTION__);
			return asynError;
		}
/*		// Normal operation 1 = the pump is running in the normal operation mode
		if (setIntegerParam (TableIndex, m_Parameters[STARTSTOP], USSReadPacket.m_USSPacketStruct.m_PZD1 & (1 << 10) ? 1 : 0) != asynSuccess)
			throw CException(pasynUser, __FUNCTION__, "Can't set parameter");

		// Remote has been activated 1 = start/stop (control bit 0) and reset(control bit 7) through serial interface is possible.
		if (setIntegerParam (TableIndex, m_Parameters[RESET], USSReadPacket.m_USSPacketStruct.m_PZD1 & (1 << 15) ? 1 : 0) != asynSuccess)
			throw CException(pasynUser, __FUNCTION__, "Can't set parameter");*/

		if (setIntegerParam (TableIndex, m_Parameters[FAULT], USSReadPacket.m_USSPacketStruct.m_PZD1 & (1 << 3) ? 1 : 0) != asynSuccess)
			throw CException(pasynUser, __FUNCTION__, "Can't set parameter");
		if (setIntegerParam (TableIndex, m_Parameters[WARNINGTEMPERATURE], USSReadPacket.m_USSPacketStruct.m_PZD1 & (1 << 2) ? 1 : 0) != asynSuccess)
			throw CException(pasynUser, __FUNCTION__, "Can't set parameter");
		if (setIntegerParam (TableIndex, m_Parameters[WARNINGHIGHLOAD], USSReadPacket.m_USSPacketStruct.m_PZD1 & (1 << 13) ? 1 : 0) != asynSuccess)
			throw CException(pasynUser, __FUNCTION__, "Can't set parameter");
		if (setIntegerParam (TableIndex, m_Parameters[STATORFREQUENCY], USSReadPacket.m_USSPacketStruct.m_PZD2) != asynSuccess)
			throw CException(pasynUser, __FUNCTION__, "Can't set parameter");
		if (setIntegerParam (TableIndex, m_Parameters[CONVERTERTEMPERATURE], USSReadPacket.m_USSPacketStruct.m_PZD3) != asynSuccess)
			throw CException(pasynUser, __FUNCTION__, "Can't set parameter");
		if (setDoubleParam  (TableIndex, m_Parameters[MOTORCURRENT], 0.1 * USSReadPacket.m_USSPacketStruct.m_PZD4) != asynSuccess)
			throw CException(pasynUser, __FUNCTION__, "Can't set parameter");
		if (setIntegerParam (TableIndex, m_Parameters[PUMPTEMPERATURE], USSReadPacket.m_USSPacketStruct.m_PZD5) != asynSuccess)
			throw CException(pasynUser, __FUNCTION__, "Can't set parameter");
		if (setDoubleParam  (TableIndex, m_Parameters[CIRCUITVOLTAGE], 0.1 * USSReadPacket.m_USSPacketStruct.m_PZD6) != asynSuccess)
			throw CException(pasynUser, __FUNCTION__, "Can't set parameter");
		if (callParamCallbacks() != asynSuccess)
			throw CException(pasynUser, __FUNCTION__, "callParamCallbacks");
		asynPrint(pasynUser, ASYN_TRACE_FLOW, "Packet success", __FILE__, __FUNCTION__);
	}
	catch(CException const&) {
		setIntegerParam(TableIndex, m_Parameters[FAULT], 1);
	}
	return asynPortDriver::readInt32(pasynUser, value);
}

asynStatus CLeyboldTurboPortDriver::writeInt32(asynUser *pasynUser, epicsInt32 value)
{
	asynStatus status = asynPortDriver::writeInt32(pasynUser, value);
	int function = pasynUser->reason;
	size_t TableIndex = function / NUM_PARAMS;
	if (TableIndex >= m_AsynUsers.size())
		throw CException(pasynUser, __FUNCTION__, "User / pump not configured");
	try {
		const char *paramName;
		USSPacket USSWritePacket, USSReadPacket;
		STATIC_ASSERT(sizeof(USSPacket)==USSPacketSize);
		size_t nbytesOut, nbytesIn;
		int eomReason;

		/* Fetch the parameter string name for possible use in debugging */
		status = getParamName(function, &paramName);
		if (status != asynSuccess)
			throw CException(pasynUser, __FUNCTION__, "Can't get parameter name:");

		asynUser* IOUser = m_AsynUsers[TableIndex];

		if (strcmp(paramName, STARTSTOP)==0)
		{
			// 1 = Start; 0 = Stop
			// Is only run provided if
			//		no error is present and
			//		control bit 10 = 1
			USSWritePacket.m_USSPacketStruct.m_PZD1 |= 1 << 10;
			USSWritePacket.m_USSPacketStruct.m_PZD1 |= (value ? 1 : 0) << 0; // Set StartStop bit.
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
			USSWritePacket.m_USSPacketStruct.m_PZD1 |= (value ? 1 : 0) << 7; // High
		}
		USSWritePacket.GenerateChecksum();
		USSWritePacket.m_USSPacketStruct.HToN();
		if (pasynOctetSyncIO->writeRead(IOUser,
				reinterpret_cast<const char*>(&USSWritePacket), sizeof(USSPacket), 
				reinterpret_cast<char*>(&USSReadPacket), sizeof(USSPacket),
				1, &nbytesOut, &nbytesIn, &eomReason) != asynSuccess)
				throw CException(pasynUser, __FUNCTION__, "Can't write/read:");
		USSReadPacket.m_USSPacketStruct.NToH();
		if (!USSReadPacket.ValidateChecksum())
		{
			asynPrint(pasynUser, ASYN_TRACE_WARNING, "Packet validation failed", __FILE__, __FUNCTION__);
			return asynError;
		}
	}
	catch(CException const&) {
		setIntegerParam(TableIndex, m_Parameters[FAULT], 1);
	}
	return status;
}

static const iocshArg initArg0 = { "asynPortName", iocshArgString};
static const iocshArg initArg1 = { "numPumps", iocshArgString};
static const iocshArg * const initArgs[] = {&initArg0, &initArg1};
static const iocshFuncDef initFuncDef = {"LeyboldTurboPortDriverConfigure",2,initArgs};

void LeyboldTurboExitFunc(void * param)
{
	delete g_LeyboldTurboPortDriver;
}

/** EPICS iocsh callable function to call constructor for the testAsynPortDriver class.
  * \param[in] portName The name of the asyn port driver to be created.*/
int LeyboldTurboPortDriverConfigure(const char *asynPortName, int numPumps)
{
	try {
		g_LeyboldTurboPortDriver = new CLeyboldTurboPortDriver(asynPortName, numPumps);
		epicsAtExit(LeyboldTurboExitFunc, NULL);
	}
	catch(CLeyboldTurboPortDriver::CException const&) {
	}
    return(asynSuccess);
}

static void initCallFunc(const iocshArgBuf *args)
{
	const char* asynPortName = args[0].sval;
	int numPumps = atoi(args[1].sval);
	LeyboldTurboPortDriverConfigure(asynPortName, numPumps);
}

static const iocshArg addArg0 = { "IOPortName", iocshArgString};
static const iocshArg * const addArgs[] = {&addArg0};
static const iocshFuncDef addFuncDef = {"LeyboldTurboAddIOPort",1,addArgs};

int LeyboldTurboAddIOPort(const char *IOPortName)
{
	try {
		g_LeyboldTurboPortDriver->addIOPort(IOPortName);
	}
	catch(CLeyboldTurboPortDriver::CException const&) {
	}
    return(asynSuccess);
}

static void addPumpFunc(const iocshArgBuf *args)
{
	const char* IOPortName = args[0].sval;
	LeyboldTurboAddIOPort(IOPortName);
}

static void LeyboldTurboRegistrar(void)
{
    iocshRegister(&initFuncDef, initCallFunc);
    iocshRegister(&addFuncDef, addPumpFunc);
}

extern "C" {

epicsExportRegistrar(LeyboldTurboRegistrar);

}
