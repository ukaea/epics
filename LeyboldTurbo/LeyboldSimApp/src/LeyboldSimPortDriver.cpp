//////////////////////////////////////////////////////////////////////////////////////////////////////
//																									//
//	Module:																							//
//		LeyboldSimDriver.cpp																		//
//																									//
//	Description:																					//
//		Implements the CLeyboldSimDriver class.														//
//		This uses AsynPortDriver and asynOctetSyncIO to provide a simulated connection with			//
//		the CLeyboldTurboPortDriver class instance.													//
//																									//
//		The class communicates by means of the Universal Serial Interface (USS)						//
//		protocol (http://www.automation.siemens.com/WW/forum/guests/PostShow.aspx?PostID=104133).	//
//																									//
//	Author:  Peter Heesterman (Tessella plc). Date: 05 Jan 2015.									//
//																									//
//	LeyboldTurbo is distributed subject to a Software License Agreement								//
//	found in file LICENSE that is included with this distribution.									//
//																									//
//////////////////////////////////////////////////////////////////////////////////////////////////////

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

#include <stdlib.h>
#include <stdexcept>

static CLeyboldSimPortDriver* g_LeyboldSimPortDriver;

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
//	class CLeyboldSimPortDriver::CException : public std::runtime_error							//
//	Description:																				//
//		If an error ocurrs, an object of this type is thrown.									//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
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
//	CLeyboldSimPortDriver::~CLeyboldSimPortDriver()												//
//																								//
//	Description:																				//
//		Class constructor & destructor.															//
//	Parameters:																					//
//		asynPortName - the IOC port name to be used.											//
//		numPumps - how many pumps will be attached?												//
//				 - The expectation is that addIOPort will be called this many times				//
//				 - from the st.cmd script.														//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
CLeyboldSimPortDriver::CLeyboldSimPortDriver(const char *asynPortName, int numPumps)
   : asynPortDriver(asynPortName, 
                    numPumps, // maxAddr
                    NUM_PARAMS-1, // -1 because Reset is not used.
                    asynDrvUserMask | asynInt32Mask | asynFloat64Mask | asynOctetMask, // Interface mask
                    asynDrvUserMask | asynInt32Mask | asynFloat64Mask | asynOctetMask, // Interrupt mask
					ASYN_MULTIDEVICE | ASYN_CANBLOCK,
                    1, // Autoconnect
                    0, // Default priority
                    0) // Default stack size
{
	m_NumConnected = 0;
	m_Exiting = false;
}

CLeyboldSimPortDriver::~CLeyboldSimPortDriver()
{
	m_Exiting = true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
//	void CLeyboldSimPortDriver::ListenerThread(void* parm)										//
//																								//
//	Description:																				//
//		static method, implements a thread that waits for connecting sockets and responds		//
///		to packet requests.																		//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
void CLeyboldSimPortDriver::ListenerThread(void* parm)
{
	CLeyboldSimPortDriver* This = static_cast<CLeyboldSimPortDriver*>(parm);
	asynUser* AsynUser;
	const char* IOPortName = epicsThreadGetNameSelf();
	try {
		int TableIndex = This->m_NumConnected;
		if (pasynOctetSyncIO->connect(IOPortName, TableIndex, &AsynUser, NULL) != asynSuccess)
			throw CException(This->pasynUserSelf, __FUNCTION__, "connecting to IO port=" + std::string(IOPortName));
		This->m_NumConnected++;
		while ((!This->m_Exiting) && 
			   (This->process(AsynUser, TableIndex)));
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
	if (This->m_NumConnected == 0)
		epicsExit(0);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
//	void CLeyboldSimPortDriver::octetConnectionCallback(										//
//		void *drvPvt, asynUser *pasynUser, char *portName, size_t len, int eomReason)			//
//																								//
//	Description:																				//
//		static method, callback is invoked when a client connects.								//
//		NB, one thread for each simulated pump connection.										//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
void CLeyboldSimPortDriver::octetConnectionCallback(void *drvPvt, asynUser *pasynUser, char *portName, 
                               size_t len, int eomReason)
{
    asynPrint(pasynUser, ASYN_TRACE_FLOW, 
              "octetConnectionCallback, portName=%s\n", portName);
    // Create a new thread to communicate with this port
    epicsThreadCreate(portName,
                      epicsThreadPriorityLow,
                      epicsThreadGetStackSize(epicsThreadStackSmall),
                      ListenerThread, drvPvt);
	// This user isn't needed any more 
	asynStatus status = pasynManager->freeAsynUser(pasynUser);
    if (status != asynSuccess)
        asynPrint(pasynUser, ASYN_TRACE_ERROR,
                              "octetConnectionCallback: Can't free port %s asynUser\n", portName);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
//	void CLeyboldSimPortDriver::addIOPort(const char* IOPortName)								//
//																								//
//	Description:																				//
//		Called once (from LeyboldSimAddIOPort) for each pump,									//
//		in response to the st.cmd startup script.												//
//		Adds a pump, and the parameters to support it, to the configuration.					//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
void CLeyboldSimPortDriver::addIOPort(const char* IOPortName)
{
	for (size_t ParamIndex = 0; ParamIndex < NUM_PARAMS; ParamIndex++)
	{
		int Index;
		std::string const& ParamName =  ParameterDefns[ParamIndex].ParamName;
		if (ParamName == "RESET")
			// Not implemented, because not meaningful for the simulater.
			continue;

		if (createParam(m_WasRunning.size(), ParamName.c_str(), ParameterDefns[ParamIndex].ParamType, &Index) != asynSuccess)
			throw CException(pasynUserSelf, __FUNCTION__, "createParam" + std::string(ParameterDefns[ParamIndex].ParamName));
		m_Parameters[ParamName] = Index;
	}
	setDefaultValues(m_WasRunning.size());
	setIntegerParam(m_WasRunning.size(), m_Parameters[FAULT], 0);

    asynUser *AsynUser = pasynManager->createAsynUser(0,0);

    if (pasynManager->connectDevice(AsynUser, IOPortName, m_NumConnected) != asynSuccess)
		throw CException(AsynUser, __FUNCTION__, "connectDevice" + std::string(IOPortName));

    asynInterface* pasynOctetInterface = pasynManager->findInterface(AsynUser, asynOctetType, 1);

	asynOctet* Octet = (asynOctet*)pasynOctetInterface->pinterface;
	void      *pinterruptNode;

	Octet->registerInterruptUser(pasynOctetInterface->drvPvt, AsynUser, octetConnectionCallback, this, &pinterruptNode);
	m_WasRunning.push_back(true);
}

void CLeyboldSimPortDriver::setDefaultValues(int TableIndex)
{
	// The running state has just been enabled.
	setIntegerParam(TableIndex, m_Parameters[RUNNING], 1);
	setIntegerParam(TableIndex, m_Parameters[STATORFREQUENCY], 500);
	setIntegerParam(TableIndex, m_Parameters[CONVERTERTEMPERATURE], 50);
	setDoubleParam(TableIndex, m_Parameters[MOTORCURRENT], 10);
	setIntegerParam(TableIndex, m_Parameters[PUMPTEMPERATURE], 40);
	setDoubleParam(TableIndex, m_Parameters[CIRCUITVOLTAGE], 30);
	setIntegerParam(TableIndex, m_Parameters[WARNINGTEMPERATURE], 0);
	setIntegerParam(TableIndex, m_Parameters[WARNINGHIGHLOAD], 0);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
//	bool CLeyboldSimPortDriver::process(asynUser *pasynUser, int TableIndex)					//
//																								//
//	Description:																				//
//		Called from the listening thread to process a packet request.							//
//																								//
//	Parameters:																					//
//		pasynUser - the user associated with the TCP link (*not* the device connection user).	//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
bool CLeyboldSimPortDriver::process(asynUser *pasynUser, int TableIndex)
{
	asynStatus status = asynSuccess;
	USSPacket USSWritePacket, USSReadPacket;
	STATIC_ASSERT(sizeof(USSPacket)==USSPacketSize);
	size_t nbytesOut, nbytesIn;
	int eomReason;

	if ((TableIndex < 0) || (TableIndex >= m_NumConnected))
		throw CException(pasynUser, __FUNCTION__, "User / pump not configured");

	// NB, This pasynUser is OK because it emitted by pasynOctetSyncIO->connect().
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
		asynPrint(pasynUser, ASYN_TRACE_WARNING, "Packet validation failed %s %s\n", __FILE__, __FUNCTION__);
		return true;
	}

	epicsInt32 IBuf;
	epicsFloat64 DBuf;
	// Normal operation 1 = the pump is running in the normal operation mode
	getIntegerParam(TableIndex, m_Parameters[RUNNING], &IBuf);
	bool Running = (IBuf != 0);

	//	control bit 10 = 1
	bool RemoteActivated = ((USSReadPacket.m_USSPacketStruct.m_PZD1 & (1 << 10)) != 0);

	if (RemoteActivated)
	{
		Running = ((USSReadPacket.m_USSPacketStruct.m_PZD1 & (1 << 0)) != 0);

		// 0 to 1 transition = Error reset Is only run provided if:
		//		the cause for the error has been removed
		//		and control bit 0 = 0 and control bit 10 = 1
		if ((USSReadPacket.m_USSPacketStruct.m_PZD1 & (1 << 7)) && (!Running))
		{
			// Clear the fault condition.
			setIntegerParam(TableIndex, m_Parameters[FAULT], 0);
		}
	}

	if (Running && !m_WasRunning[TableIndex])
	{
		// The running state has just been enabled.
		setDefaultValues(TableIndex);
	}
	if (!Running && m_WasRunning[TableIndex])
	{
		// The running state has just been disabled.
		setIntegerParam(TableIndex, m_Parameters[RUNNING], Running ? 1 : 0);
		setIntegerParam(TableIndex, m_Parameters[STATORFREQUENCY], 0);
	}

	m_WasRunning[TableIndex] = Running;

	USSWritePacket.m_USSPacketStruct.m_PZD1 = 0;

	// Normal operation 1 = the pump is running in the normal operation mode
	getIntegerParam(TableIndex, m_Parameters[RUNNING], &IBuf); USSWritePacket.m_USSPacketStruct.m_PZD1 |= (IBuf << 10);

	// Remote has been activated 1 = start/stop (control bit 0) and reset(control bit 7) through serial interface is possible.
	USSWritePacket.m_USSPacketStruct.m_PZD1 |= ((RemoteActivated ? 1 : 0) << 15);

	getIntegerParam(TableIndex, m_Parameters[FAULT], &IBuf); USSWritePacket.m_USSPacketStruct.m_PZD1 |= (IBuf << 3);
	bool Fault = (IBuf != 0);
	if (Fault)
	{
		setIntegerParam(TableIndex, m_Parameters[STATORFREQUENCY], 0);
		setIntegerParam(TableIndex, m_Parameters[RUNNING], 0);
	}
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
		-1, &nbytesOut);
	if (status == asynDisconnected)
		return false;
	if (status != asynSuccess)
		throw CException(pasynUser, __FUNCTION__, "Can't write/read:");

	callParamCallbacks(TableIndex);
	asynPrint(pasynUser, ASYN_TRACE_FLOW, "Packet success %s %s\n", __FILE__, __FUNCTION__);

	return true;

}

static const iocshArg initArg0 = { "asynPortName", iocshArgString};
static const iocshArg initArg1 = { "numPumps", iocshArgString};
static const iocshArg * const initArgs[] = {&initArg0, &initArg1};
static const iocshFuncDef initFuncDef = {"LeyboldSimPortDriverConfigure",2,initArgs};

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
//	void LeyboldSimExitFunc(void * param)														//
//																								//
//	Description:																				//
//		This function will be invoked when the IOC exits.										//
//		In order to not leak resources, it destroys the object that's been created.				//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
void LeyboldSimExitFunc(void * param)
{
	delete g_LeyboldSimPortDriver;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
//	int LeyboldSimPortDriverConfigure(const char *asynPortName, int numPumps)					//
//																								//
//	Description:																				//
//		EPICS iocsh callable function to call constructor for the CLeyboldSimPortDriver class.	//
//																								//
//	Parameters:																					//
//		asynPortName - the Asyn port name (e.g. TURBOSIM) to be used.							//
//		numPumps - how many pumps will be attached?												//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
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
	LeyboldSimPortDriverConfigure(asynPortName, numPumps);
}

static const iocshArg addArg0 = { "IOPortName", iocshArgString};
static const iocshArg * const addArgs[] = {&addArg0};
static const iocshFuncDef addFuncDef = {"LeyboldSimAddIOPort",1,addArgs};

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
//	int LeyboldSimAddIOPort(const char *IOPortName)												//
//																								//
//	Description:																				//
//		EPICS iocsh callable function to add a (simulated) pump to the IOC.						//
//																								//
//	Parameters:																					//
//		IOPortName - the IOC port name (e.g. PUMP:1) to be used.								//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
int LeyboldSimAddIOPort(const char *IOPortName)
{
	try {
		g_LeyboldSimPortDriver->addIOPort(IOPortName);
	}
	catch(CLeyboldSimPortDriver::CException const&) {
	}
    return(asynSuccess);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
//	static void addPumpFunc(const iocshArgBuf *args)											//
//																								//
//	Description:																				//
//		Registered function that adds a (simulated) pump to the IOC.							//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
static void addPumpFunc(const iocshArgBuf *args)
{
	const char* IOPortName = args[0].sval;
	LeyboldSimAddIOPort(IOPortName);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
//	static void LeyboldSimRegistrar(void)														//
//																								//
//	Description:																				//
//		Registers the functions to be called within the IOC.									//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
static void LeyboldSimRegistrar(void)
{
    iocshRegister(&initFuncDef, initCallFunc);
    iocshRegister(&addFuncDef, addPumpFunc);
}

extern "C" {

epicsExportRegistrar(LeyboldSimRegistrar);

}
