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
//	Written for CCFE (Culham Centre for Fusion Energy).												//
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
#include <epicsGuard.h>
#include <asynOctetSyncIO.h>
#include <asynStandardInterfaces.h>

#include <ParameterDefns.h>

#define epicsExportSharedSymbols
#include <epicsExport.h>

static CLeyboldSimPortDriver* g_LeyboldSimPortDriver;

#ifndef ASYN_TRACE_WARNING
// Added with asyn4-22
static const int ASYN_TRACE_WARNING = ASYN_TRACE_ERROR;
#endif


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
CLeyboldSimPortDriver::CLeyboldSimPortDriver(const char *asynPortName, int numPumps, int NoOfPZD)
   : CLeyboldBase(asynPortName, 
                    numPumps,		// maxAddr
                    NUM_PARAMS-5,	// Because Reset, FaultStr, WarningTemperatureStr, WarningHighLoadStr and WarningPurgeStr are not used.
					NoOfPZD,		// Either 2 or 6, depending on the serial port and model.
                    asynDrvUserMask | asynInt32Mask | asynFloat64Mask | asynOctetMask // Interface and interrupt mask
				)
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
		while (!This->m_Exiting)
		{
			bool Running = (This->getIntegerParam(TableIndex, RUNNING) != 0);
			if (This->m_NoOfPZD == NoOfPZD2)
			{
				USSPacket<NoOfPZD2> USSReadPacket, USSWritePacket(Running);
				if (!This->read<NoOfPZD2>(AsynUser, USSReadPacket))
				   break;
				if (!This->process<NoOfPZD2>(AsynUser, USSReadPacket, USSWritePacket, TableIndex))
				   break;
			}
			else
			{
				USSPacket<NoOfPZD6> USSReadPacket, USSWritePacket(Running);
				if (!This->read<NoOfPZD6>(AsynUser, USSReadPacket))
				   break;
				This->process(AsynUser, USSWritePacket, TableIndex);
				if (!This->process<NoOfPZD6>(AsynUser, USSReadPacket, USSWritePacket, TableIndex))
				   break;
			}
		}
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
                      epicsThreadPriorityMedium,
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
	epicsGuard < epicsMutex > guard ( m_Mutex );
	for (size_t ParamIndex = 0; ParamIndex < NUM_PARAMS; ParamIndex++)
	{
		std::string const& ParamName =  ParameterDefns[ParamIndex].ParamName;
		if ((ParamName == "RESET") || 
			(ParamName == "FAULTSTR") || 
			(ParamName == "WARNINGTEMPERATURESTR") || 
			(ParamName == "WARNINGHIGHLOADSTR") || 
			(ParamName == "WARNINGPURGESTR"))
			// Not implemented, because not meaningful for the simulater.
			continue;

		createParam(m_WasRunning.size(), ParamIndex);
	}
	setDefaultValues(m_WasRunning.size());
	setIntegerParam(m_WasRunning.size(), FAULT, 0);

    asynUser *AsynUser = pasynManager->createAsynUser(0,0);

    if (pasynManager->connectDevice(AsynUser, IOPortName, m_NumConnected) != asynSuccess)
		throw CException(AsynUser, __FUNCTION__, "connectDevice" + std::string(IOPortName));

    asynInterface* pasynOctetInterface = pasynManager->findInterface(AsynUser, asynOctetType, 1);

	asynOctet* Octet = (asynOctet*)pasynOctetInterface->pinterface;
	void      *pinterruptNode;

	Octet->registerInterruptUser(pasynOctetInterface->drvPvt, AsynUser, octetConnectionCallback, this, &pinterruptNode);
	m_WasRunning.push_back(true);
}

void CLeyboldSimPortDriver::setDefaultValues(size_t TableIndex)
{
	// The running state has just been enabled.
	setIntegerParam(TableIndex, RUNNING, 1);
	// Not set here : FAULT
	// Reset, FaultStr, WarningTemperatureStr, WarningHighLoadStr and WarningPurgeStr are not used.

	setStringParam(TableIndex, FIRMWAREVERSION, "3.03.05");
	setIntegerParam(TableIndex, WARNINGTEMPERATURE, 0);
	setIntegerParam(TableIndex, WARNINGHIGHLOAD, 0);
	setIntegerParam(TableIndex, WARNINGPURGE, 0);
	setIntegerParam(TableIndex, STATORFREQUENCY, 500);
	setIntegerParam(TableIndex, CONVERTERTEMPERATURE, 50);
	setDoubleParam(TableIndex, MOTORCURRENT, 10.0);
	setIntegerParam(TableIndex, PUMPTEMPERATURE, 40);
	setDoubleParam(TableIndex, CIRCUITVOLTAGE, 30.0);
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
template<size_t NoOfPZD> bool CLeyboldSimPortDriver::read(asynUser *pasynUser, USSPacket<NoOfPZD>& USSReadPacket)
{
	// NB, This pasynUser is OK because it emitted by pasynOctetSyncIO->connect().
	size_t nbytesIn;
	int eomReason;
	asynStatus status = pasynOctetSyncIO->read(pasynUser, reinterpret_cast<char*>(USSReadPacket.m_Bytes), USSPacketStruct<NoOfPZD>::USSPacketSize, -1, &nbytesIn, &eomReason);
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
	return true;
}

void CLeyboldSimPortDriver::process(asynUser *pasynUser, USSPacket<NoOfPZD6>& USSWritePacket, int TableIndex)
{
	epicsInt32 IBuf;
	epicsFloat64 DBuf;
	// Frequency - actual value. This is equivalent to parameter 3.
	IBuf = getIntegerParam(TableIndex, STATORFREQUENCY);
	USSWritePacket.m_USSPacketStruct.m_PZD[1] = IBuf;

	// Converter temperature - actual value. This is equivalent to parameter 11.
	IBuf = getIntegerParam(TableIndex, CONVERTERTEMPERATURE); 
	USSWritePacket.m_USSPacketStruct.m_PZD[2] = IBuf;

	// Motor current - actual value. This is equivalent to parameter 5.
	DBuf = getDoubleParam(TableIndex, MOTORCURRENT); 
	USSWritePacket.m_USSPacketStruct.m_PZD[3] = epicsUInt32(10.0 * DBuf + 0.5);

	// Motor temperature - actual value. This is equivalent to parameter 7.
	IBuf = getIntegerParam(TableIndex, PUMPTEMPERATURE); 
	USSWritePacket.m_USSPacketStruct.m_PZD[4] = IBuf;

	// Intermediate circuit voltage Uzk. This is equivalent to parameter 4.
	DBuf = getDoubleParam(TableIndex, CIRCUITVOLTAGE);
	USSWritePacket.m_USSPacketStruct.m_PZD[5] = epicsUInt32(10.0 * DBuf + 0.5);
}

template<size_t NoOfPZD> bool CLeyboldSimPortDriver::process(asynUser *pasynUser, USSPacket<NoOfPZD> const& USSReadPacket, USSPacket<NoOfPZD>& USSWritePacket, int TableIndex)
{
	if ((TableIndex < 0) || (TableIndex >= m_NumConnected))
		throw CException(pasynUser, __FUNCTION__, "User / pump not configured");

	// Normal operation 1 = the pump is running in the normal operation mode
	bool Running = (getIntegerParam(TableIndex, RUNNING) != 0);

	//	control bit 10 = 1
	bool RemoteActivated = ((USSReadPacket.m_USSPacketStruct.m_PZD[0] & (1 << 10)) != 0);

	if (RemoteActivated)
	{
		Running = ((USSReadPacket.m_USSPacketStruct.m_PZD[0] & (1 << 0)) != 0);

		// 0 to 1 transition = Error reset Is only run provided if:
		//		the cause for the error has been removed
		//		and control bit 0 = 0 and control bit 10 = 1
		if ((USSReadPacket.m_USSPacketStruct.m_PZD[0] & (1 << 7)) && (!Running))
		{
			// Clear the fault condition.
			setIntegerParam(TableIndex, FAULT, 0);
		}
	}

	{
		epicsGuard < epicsMutex > guard ( m_Mutex );

		if (Running && !m_WasRunning[TableIndex])
		{
			// The running state has just been enabled.
			setDefaultValues(TableIndex);
		}
		if (!Running && m_WasRunning[TableIndex])
		{
			// The running state has just been disabled.
			setIntegerParam(TableIndex, RUNNING, Running ? 1 : 0);
			setIntegerParam(TableIndex, STATORFREQUENCY, 0);
		}

		m_WasRunning[TableIndex] = Running;
	}

	USSWritePacket.m_USSPacketStruct.m_PZD[0] = 0;

	// Remote has been activated 1 = start/stop (control bit 0) and reset(control bit 7) through serial interface is possible.
	USSWritePacket.m_USSPacketStruct.m_PZD[0] |= ((RemoteActivated ? 1 : 0) << 15);

	bool Fault = (getIntegerParam(TableIndex, FAULT) != 0);
	if (Fault)
	{
		USSWritePacket.m_USSPacketStruct.m_PZD[0] |= (1 << 3);
		setIntegerParam(TableIndex, STATORFREQUENCY, 0);
		setIntegerParam(TableIndex, RUNNING, 0);
	}
	if (getIntegerParam(TableIndex, WARNINGTEMPERATURE) != 0)
		USSWritePacket.m_USSPacketStruct.m_PZD[0] |= (1 << 2);
	if (getIntegerParam(TableIndex, WARNINGHIGHLOAD) != 0)
		USSWritePacket.m_USSPacketStruct.m_PZD[0] |= (1 << 13);
	if (getIntegerParam(TableIndex, WARNINGPURGE) != 0)
		USSWritePacket.m_USSPacketStruct.m_PZD[0] |= (1 << 14);

	epicsUInt16 PKE = 0;
	if (USSReadPacket.m_USSPacketStruct.m_PKE & (1 << 12))
		// The requested parameter is in the least 12 bits.
		PKE = USSReadPacket.m_USSPacketStruct.m_PKE & 0X0FFF;

	switch (PKE)
	{
	case 3 : 
		// Frequency - actual value. This is equivalent to PZD[1].
		USSWritePacket.m_USSPacketStruct.m_PWE = getIntegerParam(TableIndex, STATORFREQUENCY);
		break;
	case 11:
		// Converter temperature - actual value. This is equivalent to PZD[2].
		USSWritePacket.m_USSPacketStruct.m_PWE = getIntegerParam(TableIndex, CONVERTERTEMPERATURE);
		break;
	case 5 :
		// Motor current - actual value. This is equivalent to PZD[3].
		USSWritePacket.m_USSPacketStruct.m_PWE = epicsUInt32(10.0 * getDoubleParam(TableIndex, MOTORCURRENT) + 0.5);
		break;
	case 7 :
		// Motor temperature - actual value. This is equivalent to PZD[4].
		USSWritePacket.m_USSPacketStruct.m_PWE = getIntegerParam(TableIndex, PUMPTEMPERATURE);
		break;
	case 4 :
		// Intermediate circuit voltage Uzk. This is equivalent to PZD[5].
		USSWritePacket.m_USSPacketStruct.m_PWE = epicsUInt32(10.0 * getDoubleParam(TableIndex, CIRCUITVOLTAGE) + 0.5);
		break;
	case 2 : 
		// Software version (I assume this means firmware). e.g. 3.03.05
		char CBuf[8]; // 7 chars plus null termination.
		int Major, Minor1, Minor2;
		getStringParam(TableIndex, FIRMWAREVERSION, sizeof(CBuf), CBuf);
		sscanf(CBuf, "%1d.%02d.%02d", &Major, &Minor1, &Minor2);
		USSWritePacket.m_USSPacketStruct.m_PWE = Major * 10000 + Minor1 * 100 + Minor2;
		break;
	case 171:
		// Error code
		USSWritePacket.m_USSPacketStruct.m_PWE = getIntegerParam(TableIndex, FAULT);
		break;
	case 227:
		// Temperature Warning code
		USSWritePacket.m_USSPacketStruct.m_PWE = getIntegerParam(TableIndex, WARNINGTEMPERATURE);
		break;
	case 230:
		// Load warning code.
		USSWritePacket.m_USSPacketStruct.m_PWE = getIntegerParam(TableIndex, WARNINGHIGHLOAD);
		break;

	default:
		break;
		// No action.
	}

	USSWritePacket.GenerateChecksum();
	USSWritePacket.m_USSPacketStruct.HToN();

	// NB, *don't* pass pasynUser to this function - it has the wrong type and will cause an access violation.
	size_t nbytesOut;
	asynStatus status = pasynOctetSyncIO->write(pasynUser,
		reinterpret_cast<char*>(&USSWritePacket.m_Bytes), USSPacketStruct<NoOfPZD>::USSPacketSize,
		-1, &nbytesOut);
	if (status == asynDisconnected)
		return false;
	if (status != asynSuccess)
		throw CException(pasynUser, __FUNCTION__, "Can't write/read:");

	callParamCallbacks(TableIndex);
	asynPrint(pasynUser, ASYN_TRACE_FLOW, "Packet success %s %s\n", __FILE__, __FUNCTION__);

	return true;

}

bool process6ByteFields(asynUser *pasynUser, int TableIndex);

static const iocshArg initArg0 = { "asynPortName", iocshArgString};
static const iocshArg initArg1 = { "numPumps", iocshArgString};
static const iocshArg initArg2 = { "NoOfPZD", iocshArgString};
static const iocshArg * const initArgs[] = {&initArg0, &initArg1, &initArg2};
static const iocshFuncDef initFuncDef = {"LeyboldSimPortDriverConfigure",3,initArgs};

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
int LeyboldSimPortDriverConfigure(const char *asynPortName, int numPumps, int NoOfPZD)
{
	try {
		g_LeyboldSimPortDriver = new CLeyboldSimPortDriver(asynPortName, numPumps, NoOfPZD);
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
	int NoOfPZD = atoi(args[2].sval);
	LeyboldSimPortDriverConfigure(asynPortName, numPumps, NoOfPZD);
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
		// Test the driver has been configured
		if (g_LeyboldSimPortDriver)
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
