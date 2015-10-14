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
#include <epicsTime.h>
#include <asynOctetSyncIO.h>
#include <asynStandardInterfaces.h>

#include <ParameterDefns.h>

#define epicsExportSharedSymbols
#include <epicsExport.h>

#include <stdlib.h>

epicsMutex CLeyboldSimPortDriver::m_Mutex;
CLeyboldSimPortDriver* CLeyboldSimPortDriver::m_Instance;

static const int NormalStatorFrequency = 490;

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
                    UsedParams(),
					NoOfPZD,		// Either 2 or 6, depending on the serial port and model.
                    asynDrvUserMask | asynInt32Mask | asynFloat64Mask | asynOctetMask // Interface and interrupt mask
				)
{
	m_Exiting = false;
	m_Instance = this;
}

CLeyboldSimPortDriver::~CLeyboldSimPortDriver()
{
	m_Exiting = true;
	while (m_WasRunning.size() > 0)
		m_ExitEvent.wait();
}

unsigned getTickCount()
{
	epicsTimeStamp TimeStamp = epicsTime::getCurrent();
	return TimeStamp.secPastEpoch * 1000 + TimeStamp.nsec / 1000000;
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
	asynUser* IOUser;
	const char* IOPortName = epicsThreadGetNameSelf();
	size_t TableIndex;
	{
		epicsGuard < epicsMutex > guard ( CLeyboldSimPortDriver::m_Mutex );
		TableIndex = int(This->m_WasRunning.size());
	}
	asynUser* asynUser = This->m_asynUsers[TableIndex];
	try {
		asynStatus Status = pasynOctetSyncIO->connect(IOPortName, int(TableIndex), &IOUser, NULL);
		if (Status != asynSuccess)
			throw CException(This->pasynUserSelf, Status, __FUNCTION__, "connecting to IO port=" + std::string(IOPortName));
		{
			epicsGuard < epicsMutex > guard ( CLeyboldSimPortDriver::m_Mutex );
			This->m_WasRunning.push_back(std::pair<RunStates, unsigned>(On, getTickCount()));
		}
		while (!This->m_Exiting)
		{
			if (This->m_NoOfPZD == NoOfPZD2)
			{
				USSPacket<NoOfPZD2> USSReadPacket, USSWritePacket(false);
				if (!This->read<NoOfPZD2>(asynUser, IOUser, USSReadPacket))
				   break;
				{
					if (!This->process<NoOfPZD2>(asynUser, IOUser, USSReadPacket, USSWritePacket, TableIndex))
						break;
				}
			}
			else
			{
				USSPacket<NoOfPZD6> USSReadPacket, USSWritePacket(false);
				if (!This->read<NoOfPZD6>(asynUser, IOUser, USSReadPacket))
				   break;
				{
					This->process(USSWritePacket, int(TableIndex));
					if (!This->process<NoOfPZD6>(asynUser, IOUser, USSReadPacket, USSWritePacket, TableIndex))
						break;
				}
			}
		}
	} catch(CException const&) {
	}
	asynStatus status = pasynOctetSyncIO->disconnect(IOUser);
    if (status != asynSuccess) {
        asynPrint(asynUser, ASYN_TRACE_ERROR,
                              "ListenerThread: Can't disconnect port %s IOUser\n",
                                                               IOPortName);
    }
	
	{
		epicsGuard < epicsMutex > guard ( CLeyboldSimPortDriver::m_Mutex );

		status = pasynManager->freeAsynUser(IOUser);
		if (status != asynSuccess)
			asynPrint(asynUser, ASYN_TRACE_ERROR,
								"echoListener: Can't free port %s IOUser\n",
                                                               IOPortName);
		status = pasynManager->freeAsynUser(asynUser);
		if (status != asynSuccess)
			asynPrint(This->pasynUserSelf, ASYN_TRACE_ERROR,
                              "echoListener: Can't free port %s asynUser\n",
                                                               IOPortName);
		This->m_asynUsers.erase(This->m_asynUsers.begin()+TableIndex);
		This->m_WasRunning.erase(This->m_WasRunning.begin()+TableIndex);
	}
	This->m_ExitEvent.signal();
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
	epicsGuard < epicsMutex > guard ( CLeyboldSimPortDriver::m_Mutex );
	for (size_t ParamIndex = 0; ParamIndex < size_t(NUM_PARAMS); ParamIndex++)
	{
		if (ParameterDefns[ParamIndex].m_UseCase == NotForSim)
			// Not implemented, because not meaningful for the simulater.
			continue;
		if (ParameterDefns[ParamIndex].m_UseCase == Single)
			// Single instance parameter
			continue;

		std::string const& ParamName =  ParameterDefns[ParamIndex].m_Name;

		createParam(m_asynUsers.size(), ParamIndex);
	}
	setDefaultValues(m_asynUsers.size());
	setIntegerParam(m_asynUsers.size(), FAULT, 0);

    asynUser *asynUser = pasynManager->createAsynUser(0,0);
	m_asynUsers.push_back(asynUser);

	asynStatus Status = pasynManager->connectDevice(asynUser, IOPortName, int(m_asynUsers.size()));
    if (Status != asynSuccess)
		throw CException(asynUser, Status, __FUNCTION__, "connectDevice" + std::string(IOPortName));

    asynInterface* pasynOctetInterface = pasynManager->findInterface(asynUser, asynOctetType, 1);

	asynOctet* Octet = (asynOctet*)pasynOctetInterface->pinterface;
	void      *pinterruptNode;

	Octet->registerInterruptUser(pasynOctetInterface->drvPvt, asynUser, octetConnectionCallback, this, &pinterruptNode);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
//	int CLeyboldSimPortDriver::UsedParams()														//
//																								//
//	Description:																				//
//		Gives a count of how many parameters are required for this IOC.							//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
int CLeyboldSimPortDriver::UsedParams()
{
	int UsedParams = 0;
	for (size_t ParamIndex = 0; ParamIndex < size_t(NUM_PARAMS); ParamIndex++)
	{
		if (ParameterDefns[ParamIndex].m_UseCase == NotForSim)
			// Not implemented, because not meaningful for the simulater.
			continue;
		UsedParams++;
	}
	return UsedParams;
}


//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
//	void CLeyboldSimPortDriver::setDefaultValues(size_t TableIndex)								//
//																								//
//	Descreiption:																				//
//		This method sets a set of normal 'typical' values that would be expected from a			//
//		pump running in steady state.															//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
void CLeyboldSimPortDriver::setDefaultValues(size_t TableIndex)
{
	// The running state has just been enabled.
	setIntegerParam(TableIndex, RUNNING, On);
	// Not set here : FAULT
	// Reset, FaultStr, WarningTemperatureStr, WarningHighLoadStr and WarningPurgeStr are not used.

	setIntegerParam(TableIndex, WARNINGTEMPERATURE, 0);
	setIntegerParam(TableIndex, WARNINGHIGHLOAD, 0);
	setIntegerParam(TableIndex, WARNINGPURGE, 0);
	setIntegerParam(TableIndex, STATORFREQUENCY, NormalStatorFrequency);
	setIntegerParam(TableIndex, CONVERTERTEMPERATURE, 50);
	setDoubleParam(TableIndex, MOTORCURRENT, 1.1);
	setIntegerParam(TableIndex, PUMPTEMPERATURE, 65);
	setDoubleParam(TableIndex, CIRCUITVOLTAGE, 60.2);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
//	template<size_t NoOfPZD> bool CLeyboldSimPortDriver::read(asynUser *pasynUser,				//
//		USSPacket<NoOfPZD>& USSReadPacket)														//
//																								//
//	Description:																				//
//		Called from the listening thread to wait for an incoming packet.						//
//		The packet is then byte-swapped and validated.											//
//																								//
//	Parameters:																					//
//		pasynUser - the user associated with the TCP link (*not* the device connection user).	//
//		USSReadPacket - output of the data packet that has been read.							//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
template<size_t NoOfPZD> bool CLeyboldSimPortDriver::read(asynUser *pasynUser, asynUser *IOUser, USSPacket<NoOfPZD>& USSReadPacket)
{
	// NB, This pasynUser is OK because it emitted by pasynOctetSyncIO->connect().
	size_t nBytesIn;
	int eomReason;
	asynStatus status = pasynOctetSyncIO->read(IOUser, reinterpret_cast<char*>(USSReadPacket.m_Bytes), USSPacketStruct<NoOfPZD>::USSPacketSize, -1, &nBytesIn, &eomReason);
	if (status == asynTimeout)
		return true;
	if (status == asynDisconnected)
		return false;
	if (status != asynSuccess)
		throw CException(pasynUser, status, __FUNCTION__, "Can't read:");
	STATIC_ASSERT ( sizeof(USSPacketStruct<NoOfPZD>) == USSPacketStruct<NoOfPZD>::USSPacketSize );
	STATIC_ASSERT ( sizeof(USSPacket<NoOfPZD>) == USSPacketStruct<NoOfPZD>::USSPacketSize );
//	if ((sizeof(USSPacket<NoOfPZD>) != USSPacketStruct<NoOfPZD>::USSPacketSize) ||
//		(sizeof(USSPacketStruct<NoOfPZD>) != USSPacketStruct<NoOfPZD>::USSPacketSize))
//		asynPrint(pasynUser, ASYN_TRACE_ERROR, "Packet size descrepant %ul %ul %ul\n", sizeof(USSPacket<NoOfPZD>), sizeof(USSPacketStruct<NoOfPZD>), USSPacketStruct<NoOfPZD>::USSPacketSize);
	if (nBytesIn != USSPacketStruct<NoOfPZD>::USSPacketSize)
		asynPrint(pasynUser, ASYN_TRACE_ERROR, "Unexpected packet size recieved %s %s\n", __FILE__, __FUNCTION__);

	USSReadPacket.m_USSPacketStruct.NToH();
	if (!USSReadPacket.ValidateChecksum()) 
	{
		asynPrint(pasynUser, ASYN_TRACE_WARNING, "Packet validation failed %s %s\n", __FILE__, __FUNCTION__);
		return true;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
//	bool CLeyboldSimPortDriver::process(USSPacket<NoOfPZD6>& USSWritePacket, int TableIndex)	//
//																								//
//	Description:																				//
//		Called from the listening thread to process a packet request.							//
//		The response should resemble the behaviour of the 'real' pump controller.				//
//																								//
//	Parameters:																					//
//		USSWritePacket - this is the packet that will be returned as output.					//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
void CLeyboldSimPortDriver::process(USSPacket<NoOfPZD6>& USSWritePacket, int TableIndex)
{
	epicsInt32 IBuf;
	epicsFloat64 DBuf;

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

template<size_t NoOfPZD> bool CLeyboldSimPortDriver::process(asynUser* pasynUser, asynUser* IOUser, USSPacket<NoOfPZD> const& USSReadPacket, USSPacket<NoOfPZD>& USSWritePacket, size_t TableIndex)
{
	if ((TableIndex < 0) || (TableIndex >= m_WasRunning.size()))
		throw CException(pasynUser, asynError, __FUNCTION__, "User / pump not configured");

	RunStates RunState = static_cast<RunStates>(getIntegerParam(TableIndex, RUNNING));
	// Means the running state is in effect or has been requested.
	bool Running = ((RunState == On) || (RunState == Accel));

	//	control bit 10 = 1
	bool RemoteActivated = ((USSReadPacket.m_USSPacketStruct.m_PZD[0] & (1 << 10)) != 0);

	if (RemoteActivated)
	{
		// Running state change can be requested both ways!
		Running = (USSReadPacket.m_USSPacketStruct.m_PZD[0] & (1 << 0));

		// 0 to 1 transition = Error reset Is only run provided if:
		//		the cause for the error has been removed
		//		and control bit 0 = 0 and control bit 10 = 1
		if ((USSReadPacket.m_USSPacketStruct.m_PZD[0] & (1 << 7)) && (RunState==Off))
		{
			// Clear the fault condition.
			setIntegerParam(TableIndex, FAULT, 0);
		}
	}

	{
		epicsGuard < epicsMutex > guard ( CLeyboldSimPortDriver::m_Mutex );

		if ((Running) && (m_WasRunning[TableIndex].first != On))
		{
			// The running state has just been enabled.
			if (m_WasRunning[TableIndex].first == Off)
			{
				setIntegerParam(TableIndex, RUNNING, Accel);
				setDoubleParam(TableIndex, MOTORCURRENT, 15.2);
				m_WasRunning[TableIndex].first = Accel;
				m_WasRunning[TableIndex].second = getTickCount();
			}
			else
			{
				// Accel.
				// It is intended that the (simulated) pump speed ramps up to full speed in Duration
				static const unsigned Duration = 2000;
				unsigned ElapsedTime = getTickCount() - m_WasRunning[TableIndex].second;
				setIntegerParam(TableIndex, STATORFREQUENCY, (ElapsedTime * NormalStatorFrequency) / Duration);
				if (ElapsedTime >= Duration)
				{
					setDefaultValues(TableIndex);
					m_WasRunning[TableIndex].first = On;
					m_WasRunning[TableIndex].second = getTickCount();
				}
			}
		}
		else if ((!Running) && (m_WasRunning[TableIndex].first != Off))
		{
			// The running state has just been disabled.
			if (m_WasRunning[TableIndex].first == On)
			{
				setIntegerParam(TableIndex, RUNNING, Decel);
				m_WasRunning[TableIndex].first = Decel;
				m_WasRunning[TableIndex].second = getTickCount();
			}
			else if (m_WasRunning[TableIndex].first == Decel)
			{
				// It is intended that the (simulated) pump speed ramps down to nothing in Duration
				static const int Duration = 2000;
				int ElapsedTime = getTickCount() - m_WasRunning[TableIndex].second;
				int StatorFrequency = ((Duration - ElapsedTime) * NormalStatorFrequency) / Duration;
				if (StatorFrequency < 3)
					StatorFrequency = 3;
				setIntegerParam(TableIndex, STATORFREQUENCY, StatorFrequency);
				if (ElapsedTime >= Duration)
				{
					setIntegerParam(TableIndex, RUNNING, Moving);
					m_WasRunning[TableIndex].first = Moving;
					m_WasRunning[TableIndex].second = getTickCount();
				}
			}
			else if (m_WasRunning[TableIndex].first == Moving)
			{
				// It is intended that the pump remains in the 'Moving' state for another Duration
				static const unsigned Duration = 2000;
				unsigned ElapsedTime = getTickCount() - m_WasRunning[TableIndex].second;
				if (ElapsedTime >= Duration)
				{
					setIntegerParam(TableIndex, STATORFREQUENCY, 0);
					setIntegerParam(TableIndex, RUNNING, Off);
					m_WasRunning[TableIndex].first = Off;
					m_WasRunning[TableIndex].second = getTickCount();
				}
			}
		}
	}

	USSWritePacket.m_USSPacketStruct.m_PZD[0] = 0;
	RunState = m_WasRunning[TableIndex].first;
	if (RunState == On)
		USSWritePacket.m_USSPacketStruct.m_PZD[0] |= (1 << 10);
	else if (RunState == Accel)
		USSWritePacket.m_USSPacketStruct.m_PZD[0] |= (1 << 4);
	else if (RunState == Decel)
		USSWritePacket.m_USSPacketStruct.m_PZD[0] |= (1 << 5);
	else if (RunState == Moving)
		USSWritePacket.m_USSPacketStruct.m_PZD[0] |= (1 << 11);

	// Remote has been activated 1 = start/stop (control bit 0) and reset(control bit 7) through serial interface is possible.
	USSWritePacket.m_USSPacketStruct.m_PZD[0] |= ((RemoteActivated ? 1 : 0) << 15);

	bool Fault = (getIntegerParam(TableIndex, FAULT) != 0);
	if (Fault)
	{
		// A fault condition causes the controller to stop the pump.
		USSWritePacket.m_USSPacketStruct.m_PZD[0] |= (1 << 3);
		setIntegerParam(TableIndex, STATORFREQUENCY, 0);
		setIntegerParam(TableIndex, RUNNING, Off);
		m_WasRunning[TableIndex].first = Off;
	}
	if (getIntegerParam(TableIndex, WARNINGTEMPERATURE) != 0)
		USSWritePacket.m_USSPacketStruct.m_PZD[0] |= (1 << 2);
	if (getIntegerParam(TableIndex, WARNINGHIGHLOAD) != 0)
		USSWritePacket.m_USSPacketStruct.m_PZD[0] |= (1 << 13);
	if (getIntegerParam(TableIndex, WARNINGPURGE) != 0)
		USSWritePacket.m_USSPacketStruct.m_PZD[0] |= (1 << 14);

	// Frequency - actual value. This is equivalent to parameter 3. Both packet types have this field.
	USSWritePacket.m_USSPacketStruct.m_PZD[1] = getIntegerParam(TableIndex, STATORFREQUENCY);

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

	size_t nbytesOut;
	asynStatus status = pasynOctetSyncIO->write(IOUser,
		reinterpret_cast<char*>(&USSWritePacket.m_Bytes), USSPacketStruct<NoOfPZD>::USSPacketSize,
		-1, &nbytesOut);
	if (status == asynDisconnected)
		return false;
	if (status != asynSuccess)
		throw CException(IOUser, status, __FUNCTION__, "Can't write/read:");

	callParamCallbacks(int(TableIndex));
	asynPrint(pasynUser, ASYN_TRACE_FLOW, "Packet success %s %s\n", __FILE__, __FUNCTION__);

	return true;

}

asynStatus CLeyboldSimPortDriver::readInt32(asynUser *pasynUser, epicsInt32 *value)
{
	epicsGuard < epicsMutex > guard ( CLeyboldSimPortDriver::m_Mutex );
	asynStatus Status = CLeyboldBase::readInt32(pasynUser, value);
	return Status;
}

asynStatus CLeyboldSimPortDriver::writeInt32(asynUser *pasynUser, epicsInt32 value)
{
	epicsGuard < epicsMutex > guard ( CLeyboldSimPortDriver::m_Mutex );
	asynStatus Status = CLeyboldBase::writeInt32(pasynUser, value);
	return Status;
}

asynStatus CLeyboldSimPortDriver::readOctet(asynUser *pasynUser, char *value, size_t maxChars,
                                        size_t *nActual, int *eomReason)
{
	epicsGuard < epicsMutex > guard ( CLeyboldSimPortDriver::m_Mutex );
	asynStatus Status = CLeyboldBase::readOctet(pasynUser, value, maxChars, nActual, eomReason);
	return Status;
}

static const iocshArg initArg0 = { "asynPortName", iocshArgString};
static const iocshArg initArg1 = { "numPumps", iocshArgString};
static const iocshArg initArg2 = { "NoOfPZD", iocshArgString};
static const iocshArg * const initArgs[] = {&initArg0, &initArg1, &initArg2};
static const iocshFuncDef initFuncDef = {"LeyboldSimPortDriverConfigure",3,initArgs};

void CLeyboldSimPortDriver::exit()
{
	m_Exiting = true;
}

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
	CLeyboldSimPortDriver* Instance = static_cast<CLeyboldSimPortDriver*>(param);
	Instance->exit();
	delete Instance;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
//	static void LeyboldSimPortDriverConfigure(const iocshArgBuf *args)							//
//																								//
//	Description:																				//
//		EPICS iocsh callable function to call constructor for the CLeyboldSimPortDriver class.	//
//																								//
//	Parameters:																					//
//		args - 3 arguments:																		//
//			numPumps - how many pumps will be attached?											//
//			NoOfPZD - This will be 6 for older model pumps or									//
//					  2 for the rear port of the MAG Drive Digital model.						//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
static void LeyboldSimPortDriverConfigure(const iocshArgBuf *args)
{
	try {
		const char* asynPortName = args[0].sval;
		int numPumps = atoi(args[1].sval);
		int NoOfPZD = atoi(args[2].sval);
		CLeyboldSimPortDriver* Instance = new CLeyboldSimPortDriver(asynPortName, numPumps, NoOfPZD);
		epicsAtExit(LeyboldSimExitFunc, Instance);
	}
	catch(CLeyboldSimPortDriver::CException const&) {
	}
}

static const iocshArg addArg0 = { "IOPortName", iocshArgString};
static const iocshArg * const addArgs[] = {&addArg0};
static const iocshFuncDef addFuncDef = {"LeyboldSimAddIOPort",1,addArgs};

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
//	int LeyboldSimAddIOPort(const iocshArgBuf *args)											//
//																								//
//	Description:																				//
//		EPICS iocsh callable function to add a (simulated) pump to the IOC.						//
//																								//
//	Parameters:																					//
//		args - 1 argument, the IOC port name (e.g. PUMP:1) to be used.							//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
void LeyboldSimAddIOPort(const iocshArgBuf *args)
{
	try {
		const char* IOPortName = args[0].sval;
		// Test the driver has been configured
		if (CLeyboldSimPortDriver::Instance())
			CLeyboldSimPortDriver::Instance()->addIOPort(IOPortName);
	}
	catch(CLeyboldSimPortDriver::CException const&) {
	}
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
    iocshRegister(&initFuncDef, LeyboldSimPortDriverConfigure);
    iocshRegister(&addFuncDef, LeyboldSimAddIOPort);
}

extern "C" {

epicsExportRegistrar(LeyboldSimRegistrar);

}
