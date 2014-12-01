#include "LeyboldSimPortDriver.h"

#include <iocsh.h>
#include <epicsExit.h>
#include <epicsAssert.h>
#include <epicsThread.h>
#include <asynOctetSyncIO.h>
#include <asynStandardInterfaces.h>

#include <USSPacket.h>
#include <ParameterDefns.h>

#define epicsExportSharedSymbols
#include <epicsExport.h>

static CLeyboldSimPortDriver* g_LeyboldSimPortDriver;

class CLeyboldSimPortDriver::CException : public std::runtime_error
{
public:
	CException(asynUser* AsynUser, const char* functionName, std::string const& what) : std::runtime_error(what) {
		std::string message = "%s:%s ERROR: " + what + "\n";
		asynPrint(AsynUser, ASYN_TRACE_ERROR, message.c_str(), __FILE__, functionName, AsynUser->errorMessage);
	}
};

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
//	CLeyboldSimPortDriver::CLeyboldSimPortDriver(const char *asynPortName, int numPumps)		//
//																								//
//	Description:
//		Class constructor.
//		
/** Constructor for the testAsynPortDriver class.
  * Calls constructor for the asynPortDriver base class.
  * \param[in] portName The name of the asyn port driver to be created.
  * \param[in] maxPoints The maximum  number of points in the volt and time arrays */
CLeyboldSimPortDriver::CLeyboldSimPortDriver(const char *asynPortName, int numPumps)
   : asynPortDriver(asynPortName, 
                    numPumps, /* maxAddr */ 
                    NUM_PARAMS,
                    asynDrvUserMask | asynInt32Mask | asynFloat64Mask | asynOctetMask, /* Interface mask */
                    asynDrvUserMask | asynInt32Mask | asynFloat64Mask | asynOctetMask, /* Interrupt mask */
					ASYN_MULTIDEVICE | ASYN_CANBLOCK,
                    1, /* Autoconnect */
                    0, /* Default priority */
                    0) /* Default stack size*/    
{
	m_mutexId = epicsMutexCreate();
	m_NumConnected = 0;
	m_Exiting = false;
}

CLeyboldSimPortDriver::~CLeyboldSimPortDriver()
{
	m_Exiting = true;
	epicsMutexDestroy(m_mutexId);
}

void CLeyboldSimPortDriver::ListenerThread(void* parm)
{
	CLeyboldSimPortDriver* This = static_cast<CLeyboldSimPortDriver*>(parm);
	asynUser* AsynUser;
	const char* IOPortName = epicsThreadGetNameSelf();
	try {
		if (pasynOctetSyncIO->connect(IOPortName, This->m_NumConnected, &AsynUser, NULL) != asynSuccess)
			throw CException(This->pasynUserSelf, __FUNCTION__, "connecting to IO port=" + std::string(IOPortName));
		This->m_NumConnected++;

		while ((!This->m_Exiting) && 
			   (This->process(AsynUser)));
	} catch(CException const&) {
	}
	This->m_NumConnected--;
	asynStatus status = pasynOctetSyncIO->disconnect(AsynUser);
    if (status != asynSuccess) {
        asynPrint(AsynUser, ASYN_TRACE_ERROR,
                              "ListenerThread: Can't disconnect port %s asynUser\n",
                                                               IOPortName);
    }
    status = pasynManager->freeAsynUser(AsynUser);
    if (status != asynSuccess)
        asynPrint(AsynUser, ASYN_TRACE_ERROR,
                              "echoListener: Can't free port %s asynUser\n",
                                                               IOPortName);
}

void CLeyboldSimPortDriver::octetConnectionCallback(void *drvPvt, asynUser *pasynUser, char *portName, 
                               size_t len, int eomReason)
{
    asynPrint(pasynUser, ASYN_TRACE_FLOW, 
              "octetConnectionCallback, portName=%s\n", portName);
    /* Create a new thread to communicate with this port */
    epicsThreadCreate(portName,
                      epicsThreadPriorityLow,
                      epicsThreadGetStackSize(epicsThreadStackSmall),
                      ListenerThread, drvPvt);
	asynStatus status = pasynManager->freeAsynUser(pasynUser);
    if (status != asynSuccess)
        asynPrint(pasynUser, ASYN_TRACE_ERROR,
                              "octetConnectionCallback: Can't free port %s asynUser\n", portName);
}

void CLeyboldSimPortDriver::addIOPort(const char* IOPortName)
{
	for (size_t ParamIndex = 0; ParamIndex < NUM_PARAMS; ParamIndex++)
	{
		int Index;
		std::string ParamName =  ParameterDefns[ParamIndex].ParamName;
		if (createParam(m_NumConnected, ParamName.c_str(), ParameterDefns[ParamIndex].ParamType, &Index) != asynSuccess)
			throw CException(pasynUserSelf, __FUNCTION__, "createParam" + std::string(ParameterDefns[ParamIndex].ParamName));
		m_Parameters[ParamName] = Index;
		switch(ParameterDefns[ParamIndex].ParamType)
		{
			case asynParamInt32: 
				if (setIntegerParam(m_NumConnected, Index, ParameterDefns[ParamIndex].DefaultValue) != asynSuccess)
					throw CException(pasynUserSelf, __FUNCTION__, "setUIntDigitalParam" + std::string(ParameterDefns[ParamIndex].ParamName));
				break;
			case asynParamFloat64: 
				if (setDoubleParam (m_NumConnected, Index, ParameterDefns[ParamIndex].DefaultValue) != asynSuccess)
					throw CException(pasynUserSelf, __FUNCTION__, "setDoubleParam" + std::string(ParameterDefns[ParamIndex].ParamName));
				break;
			default: assert(false);
		}
	}

	if (callParamCallbacks() != asynSuccess)
		throw CException(pasynUserSelf, __FUNCTION__, "callParamCallbacks");

    asynUser *AsynUser = pasynManager->createAsynUser(0,0);

    if (pasynManager->connectDevice(AsynUser, IOPortName, m_NumConnected) != asynSuccess)
		throw CException(AsynUser, __FUNCTION__, "connectDevice" + std::string(IOPortName));

    asynInterface* pasynOctetInterface = pasynManager->findInterface(AsynUser, asynOctetType, 1);

	asynOctet* Octet = (asynOctet*)pasynOctetInterface->pinterface;
	void      *pinterruptNode;

	Octet->registerInterruptUser(pasynOctetInterface->drvPvt, AsynUser, octetConnectionCallback, this, &pinterruptNode);
	m_NumConnected++;
}

bool CLeyboldSimPortDriver::process(asynUser *pasynUser)
{
	asynStatus status = asynSuccess;
	int function = pasynUser->reason;
	USSPacket USSWritePacket, USSReadPacket;
	STATIC_ASSERT(sizeof(USSPacket)==USSPacketSize);
	size_t nbytesOut, nbytesIn;
	int eomReason;

	int TableIndex = function / NUM_PARAMS;
	if (TableIndex >= m_NumConnected)
		throw CException(pasynUser, __FUNCTION__, "User / pump not configured");

	asynInterface* pasynOctetInterface = pasynManager->findInterface(pasynUser, asynOctetType, 1);

	asynOctet* Octet = (asynOctet*)pasynOctetInterface->pinterface;

	// NB, *don't* pass pasynUser to this function - it has the wrong type and will cause an access violation.
	status = pasynOctetSyncIO->read(pasynUser, reinterpret_cast<char*>(&USSReadPacket), sizeof(USSPacket), 10, &nbytesIn, &eomReason);
	if (status == asynTimeout)
		return true;
	if (status == asynDisconnected)
		return false;
	if (status != asynSuccess)
		throw CException(pasynUser, __FUNCTION__, "Can't read:");

	USSReadPacket.m_USSPacketStruct.NToH();
	if (!USSReadPacket.ValidateChecksum())
	{
		asynPrint(pasynUser, ASYN_TRACE_WARNING, "Packet validation failed", __FILE__, __FUNCTION__);
		return true;
	}

	epicsInt32 IBuf;
	epicsFloat64 DBuf;
	// Normal operation 1 = the pump is running in the normal operation mode
	getIntegerParam(TableIndex, m_Parameters[STARTSTOP], &IBuf);
	bool WasStartStop = (IBuf != 0);

	bool StartStop = WasStartStop;
	bool Reset = false;
	if (USSReadPacket.m_USSPacketStruct.m_PZD1 & (1 << 10))
	{
		//		control bit 10 = 1
		StartStop = ((USSReadPacket.m_USSPacketStruct.m_PZD1 & (1 << 0)) != 0);

		// 0 to 1 transition = Error reset Is only run provided if:
		//		the cause for the error has been removed
		//		and control bit 0 = 0 and control bit 10 = 1
		if ((USSReadPacket.m_USSPacketStruct.m_PZD1 & (1 << 7)) && (!StartStop))
		{
			// Clear the fault condition.
			Reset = true;
			setIntegerParam(TableIndex, m_Parameters[FAULT], 0);
		}
	}

	if (StartStop && !WasStartStop)
	{
		setIntegerParam(TableIndex, m_Parameters[STARTSTOP], StartStop ? 1 : 0);
		setIntegerParam(TableIndex, m_Parameters[STATORFREQUENCY], ParameterDefns[m_Parameters[STATORFREQUENCY]].DefaultValue);
		setIntegerParam(TableIndex, m_Parameters[CONVERTERTEMPERATURE], ParameterDefns[m_Parameters[CONVERTERTEMPERATURE]].DefaultValue);
		setDoubleParam(TableIndex, m_Parameters[MOTORCURRENT], ParameterDefns[m_Parameters[MOTORCURRENT]].DefaultValue);
		setIntegerParam(TableIndex, m_Parameters[PUMPTEMPERATURE], ParameterDefns[m_Parameters[PUMPTEMPERATURE]].DefaultValue);
		setDoubleParam(TableIndex, m_Parameters[CIRCUITVOLTAGE], ParameterDefns[m_Parameters[CIRCUITVOLTAGE]].DefaultValue);
		setIntegerParam(TableIndex, m_Parameters[WARNINGTEMPERATURE], 0);
		setIntegerParam(TableIndex, m_Parameters[WARNINGHIGHLOAD], 0);
		if (callParamCallbacks() != asynSuccess)
			throw CException(pasynUser, __FUNCTION__, "callParamCallbacks");
	}
	if (!StartStop && WasStartStop)
	{
		setIntegerParam(TableIndex, m_Parameters[STARTSTOP], StartStop ? 1 : 0);
		setIntegerParam(TableIndex, m_Parameters[STATORFREQUENCY], 0);
		if (callParamCallbacks() != asynSuccess)
			throw CException(pasynUser, __FUNCTION__, "callParamCallbacks");
	}

	USSWritePacket.m_USSPacketStruct.m_PZD1 = 0;

	// Normal operation 1 = the pump is running in the normal operation mode
	getIntegerParam(TableIndex, m_Parameters[STARTSTOP], &IBuf); USSWritePacket.m_USSPacketStruct.m_PZD1 |= (IBuf << 10);

	// Remote has been activated 1 = start/stop (control bit 0) and reset(control bit 7) through serial interface is possible.
	getIntegerParam(TableIndex, m_Parameters[RESET], &IBuf); USSWritePacket.m_USSPacketStruct.m_PZD1 |= (IBuf << 15);
	bool Fault = (IBuf != 0);
	if (Fault)
	{
		setIntegerParam(TableIndex, m_Parameters[STATORFREQUENCY], 0);
	}

	getIntegerParam(TableIndex, m_Parameters[FAULT], &IBuf); USSWritePacket.m_USSPacketStruct.m_PZD1 |= (IBuf << 3);
	getIntegerParam(TableIndex, m_Parameters[WARNINGTEMPERATURE], &IBuf); USSWritePacket.m_USSPacketStruct.m_PZD1 |= (IBuf << 2);
	getIntegerParam(TableIndex, m_Parameters[WARNINGHIGHLOAD], &IBuf); USSWritePacket.m_USSPacketStruct.m_PZD1 |= (IBuf << 13);

	getIntegerParam(TableIndex, m_Parameters[STATORFREQUENCY], &IBuf); USSWritePacket.m_USSPacketStruct.m_PZD2 = IBuf;
	getIntegerParam(TableIndex, m_Parameters[CONVERTERTEMPERATURE], &IBuf); USSWritePacket.m_USSPacketStruct.m_PZD3 = IBuf;
	getDoubleParam(TableIndex, m_Parameters[MOTORCURRENT], &DBuf); USSWritePacket.m_USSPacketStruct.m_PZD4 = epicsUInt32(10.0 * DBuf + 0.5);
	getIntegerParam(TableIndex, m_Parameters[PUMPTEMPERATURE], &IBuf); USSWritePacket.m_USSPacketStruct.m_PZD5 = IBuf;
	getDoubleParam(TableIndex, m_Parameters[CIRCUITVOLTAGE], &DBuf); USSWritePacket.m_USSPacketStruct.m_PZD6 = epicsUInt32(10.0 * DBuf + 0.5);
	USSWritePacket.GenerateChecksum();
	USSWritePacket.m_USSPacketStruct.HToN();

	// NB, *don't* pass pasynUser to this function - it has the wrong type and will cause an access violation.
	status = pasynOctetSyncIO->write(pasynUser,
		reinterpret_cast<char*>(&USSWritePacket), sizeof(USSPacket),
		1, &nbytesOut);
	if (status == asynDisconnected)
		return false;
	if (status != asynSuccess)
		throw CException(pasynUser, __FUNCTION__, "Can't write/read:");

	if (callParamCallbacks() != asynSuccess)
		throw CException(pasynUser, __FUNCTION__, "callParamCallbacks");
	asynPrint(pasynUser, ASYN_TRACE_FLOW, "Packet success", __FILE__, __FUNCTION__);

	return true;

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
