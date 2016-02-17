//////////////////////////////////////////////////////////////////////////////////////////////////////
//																									//
//	Module:																							//
//		VQM_ITMS_SimDriver.cpp																		//
//																									//
//	Description:																					//
//		Implements the CVQM_ITMS_SimDriver class.														//
//		This uses AsynPortDriver and asynOctetSyncIO to provide a simulated connection with			//
//		the CVQM_ITMSPortDriver class instance.													//
//																									//
//	Author:  Peter Heesterman (Tessella plc). Date: 05 Jan 2015.									//
//	Written for CCFE (Culham Centre for Fusion Energy).												//
//																									//
//	VQM_ITMS is distributed subject to a Software License Agreement								//
//	found in file LICENSE that is included with this distribution.									//
//																									//
//////////////////////////////////////////////////////////////////////////////////////////////////////

#include "VQM_ITMS_SimDriver.h"

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

epicsMutex CVQM_ITMS_SimPortDriver::m_Mutex;
CVQM_ITMS_SimPortDriver* CVQM_ITMS_SimPortDriver::m_Instance;

static const int NormalStatorFrequency = 490;

#ifndef ASYN_TRACE_WARNING
// Added with asyn4-22
static const int ASYN_TRACE_WARNING = ASYN_TRACE_ERROR;
#endif


class CVQM_ITMS_SimPortDriver::CThreadRunable : public epicsThreadRunable
{
	public:
		CThreadRunable(CVQM_ITMS_SimPortDriver* This) {
			m_This = This;
			m_Exiting = false;
		}

		virtual void run ();
		void setExiting() {
			m_Exiting = true;
		}
	private:
		CVQM_ITMS_SimPortDriver* m_This;
		volatile bool m_Exiting;		// Signals the listening thread to exit.
};

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
//	CVQM_ITMS_SimPortDriver::CVQM_ITMS_SimPortDriver(const char *asynPortName, int numTraps)		//
//	CVQM_ITMS_SimPortDriver::~CVQM_ITMS_SimPortDriver()												//
//																								//
//	Description:																				//
//		Class constructor & destructor.															//
//	Parameters:																					//
//		asynPortName - the IOC port name to be used.											//
//		numTraps - how many pumps will be attached?												//
//				 - The expectation is that addIOPort will be called this many times				//
//				 - from the st.cmd script.														//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
CVQM_ITMS_SimPortDriver::CVQM_ITMS_SimPortDriver(const char *asynPortName, int numTraps)
   : CVQM_ITMS_Base(asynPortName, 
                    numTraps,		// maxAddr
                    UsedParams()
				)
{
	m_ThreadRunable = new CThreadRunable(this);
	m_Instance = this;
}

CVQM_ITMS_SimPortDriver::~CVQM_ITMS_SimPortDriver()
{
	m_ThreadRunable->setExiting();
	for(size_t ThreadNum = 0; ThreadNum < m_Threads.size(); ThreadNum++)
		delete m_Threads[ThreadNum];
}

unsigned getTickCount()
{
	epicsTimeStamp TimeStamp = epicsTime::getCurrent();
	return TimeStamp.secPastEpoch * 1000 + TimeStamp.nsec / 1000000;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
//	void CVQM_ITMS_SimPortDriver::CThreadRunable::run()											//
//																								//
//	Description:																				//
//		static method, implements a thread that waits for connecting sockets and responds		//
///		to packet requests.																		//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
void CVQM_ITMS_SimPortDriver::CThreadRunable::run()
{
	asynUser* IOUser;
	const char* IOPortName = epicsThreadGetNameSelf();
	size_t TableIndex;
	{
		epicsGuard < epicsMutex > guard ( CVQM_ITMS_SimPortDriver::m_Mutex );
		std::string LookupPortName = IOPortName;
		size_t Colon1Pos = LookupPortName.rfind(":1");
		LookupPortName = LookupPortName.substr(0, Colon1Pos);
		std::map<std::string, size_t>::const_iterator Iter = m_This->m_TableLookup.find(LookupPortName);
		if (Iter == m_This->m_TableLookup.end())
			throw CException(m_This->pasynUserSelf, asynError, __FUNCTION__, "ITMS name not found");
		TableIndex = int(Iter->second);
	}
	asynUser* asynUser = m_This->m_asynUsers[TableIndex];
	try {
		asynStatus Status = pasynOctetSyncIO->connect(IOPortName, int(TableIndex), &IOUser, NULL);
		if (Status != asynSuccess)
			throw CException(m_This->pasynUserSelf, Status, __FUNCTION__, "connecting to IO port=" + std::string(IOPortName));
		{
			epicsGuard < epicsMutex > guard ( CVQM_ITMS_SimPortDriver::m_Mutex );
		}
		while (!m_Exiting)
		{
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
		epicsGuard < epicsMutex > guard ( CVQM_ITMS_SimPortDriver::m_Mutex );

		status = pasynManager->freeAsynUser(IOUser);
		if (status != asynSuccess)
			asynPrint(asynUser, ASYN_TRACE_ERROR,
								"echoListener: Can't free port %s IOUser\n",
                                                               IOPortName);
		status = pasynManager->freeAsynUser(asynUser);
		if (status != asynSuccess)
			asynPrint(m_This->pasynUserSelf, ASYN_TRACE_ERROR,
                              "echoListener: Can't free port %s asynUser\n",
                                                               IOPortName);
		if (m_This->m_asynUsers.size() > TableIndex)
			m_This->m_asynUsers.erase(m_This->m_asynUsers.begin()+TableIndex);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
//	void CVQM_ITMS_SimPortDriver::octetConnectionCallback(										//
//		void *drvPvt, asynUser *pasynUser, char *portName, size_t len, int eomReason)			//
//																								//
//	Description:																				//
//		static method, callback is invoked when a client connects.								//
//		NB, one thread for each simulated ITMS connection.										//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
void CVQM_ITMS_SimPortDriver::octetConnectionCallback(void *drvPvt, asynUser *pasynUser, char *portName, 
                               size_t len, int eomReason)
{
	CVQM_ITMS_SimPortDriver* This = reinterpret_cast<CVQM_ITMS_SimPortDriver*>(drvPvt);
    // Create a new thread to communicate with this port
	This->m_Threads.push_back(new epicsThread(*This->m_ThreadRunable, portName, epicsThreadGetStackSize(epicsThreadStackSmall)));
	(*This->m_Threads.rbegin())->start();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
//	void CVQM_ITMS_SimPortDriver::addIOPort(const char* IOPortName)								//
//																								//
//	Description:																				//
//		Called once (from VQM_ITMS_SimAddIOPort) for each ITMS,									//
//		in response to the st.cmd startup script.												//
//		Adds a ITMS, and the parameters to support it, to the configuration.					//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
void CVQM_ITMS_SimPortDriver::addIOPort(const char* IOPortName)
{
	epicsGuard < epicsMutex > guard ( CVQM_ITMS_SimPortDriver::m_Mutex );
	for (size_t ParamIndex = 0; ParamIndex < size_t(NUM_PARAMS); ParamIndex++)
	{
		if (ParameterDefns[ParamIndex].m_UseCase == NotForSim)
			// Not implemented, because not meaningful for the simulater.
			continue;
		if (ParameterDefns[ParamIndex].m_UseCase == Single)
			// Single instance parameter
			continue;

		createParam(m_asynUsers.size(), ParamIndex);
	}
	setDefaultValues(m_asynUsers.size());
	setIntegerParam(m_asynUsers.size(), ParameterDefn::FAULT, 0);

    asynUser *asynUser = pasynManager->createAsynUser(0,0);
	m_TableLookup[IOPortName] = m_asynUsers.size();
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
//	int CVQM_ITMS_SimPortDriver::UsedParams()														//
//																								//
//	Description:																				//
//		Gives a count of how many parameters are required for this IOC.							//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
int CVQM_ITMS_SimPortDriver::UsedParams()
{
	int UsedParams = 0;
	for (size_t ParamIndex = 0; ParamIndex < size_t(NUM_PARAMS); ParamIndex++)
	{
		if (ParameterDefns[ParamIndex].m_UseCase == NotForSim)
			// Not implemented, because not meaningful for the simulater.
			continue;
		// But the Single parameter list is required.
		UsedParams++;
	}
	return UsedParams;
}


//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
//	void CVQM_ITMS_SimPortDriver::setDefaultValues(size_t TableIndex)								//
//																								//
//	Descreiption:																				//
//		This method sets a set of normal 'typical' values that would be expected from a			//
//		ITMS running in steady state.															//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
void CVQM_ITMS_SimPortDriver::setDefaultValues(size_t TableIndex)
{
	// Not set here : FAULT
	// Reset, FaultStr, WarningTemperatureStr, WarningHighLoadStr and WarningPurgeStr are not used.

	setIntegerParam(TableIndex, ParameterDefn::DISCONNECTED, 0);
}

static const iocshArg initArg0 = { "asynPortName", iocshArgString};
static const iocshArg initArg1 = { "numTraps", iocshArgString};
static const iocshArg * const initArgs[] = {&initArg0, &initArg1};
static const iocshFuncDef initFuncDef = {"VQM_ITMS_SimPortDriverConfigure",3,initArgs};

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
//	void VQM_ITMS_SimExitFunc(void * param)														//
//																								//
//	Description:																				//
//		This function will be invoked when the IOC exits.										//
//		In order to not leak resources, it destroys the object that's been created.				//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
void VQM_ITMS_SimExitFunc(void * param)
{
	CVQM_ITMS_SimPortDriver* Instance = static_cast<CVQM_ITMS_SimPortDriver*>(param);
	delete Instance;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
//	static void VQM_ITMS_SimPortDriverConfigure(const iocshArgBuf *args)							//
//																								//
//	Description:																				//
//		EPICS iocsh callable function to call constructor for the CVQM_ITMS_SimPortDriver class.	//
//																								//
//	Parameters:																					//
//		args - 3 arguments:																		//
//			numTraps - how many pumps will be attached?											//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
void CVQM_ITMS_SimPortDriver::VQM_ITMS_SimPortDriverConfigure(const iocshArgBuf *args)
{
	try {
		const char* asynPortName = args[0].sval;
		int numTraps = atoi(args[1].sval);
		CVQM_ITMS_SimPortDriver* Instance = new CVQM_ITMS_SimPortDriver(asynPortName, numTraps);
		epicsAtExit(VQM_ITMS_SimExitFunc, Instance);
	}
	catch(CVQM_ITMS_SimPortDriver::CException const&) {
	}
}

static const iocshArg addArg0 = { "IOPortName", iocshArgString};
static const iocshArg * const addArgs[] = {&addArg0};
static const iocshFuncDef addFuncDef = {"VQM_ITMS_SimAddIOPort",1,addArgs};

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
//	int CVQM_ITMS_SimPortDriver::VQM_ITMS_SimAddIOPort(const iocshArgBuf *args)						//
//																								//
//	Description:																				//
//		EPICS iocsh callable function to add a (simulated) ITMS to the IOC.						//
//																								//
//	Parameters:																					//
//		args - 1 argument, the IOC port name (e.g. ITMS:1) to be used.							//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
void CVQM_ITMS_SimPortDriver::VQM_ITMS_SimAddIOPort(const iocshArgBuf *args)
{
	try {
		const char* IOPortName = args[0].sval;
		// Test the driver has been configured
		if (CVQM_ITMS_SimPortDriver::Instance())
			CVQM_ITMS_SimPortDriver::Instance()->addIOPort(IOPortName);
	}
	catch(CVQM_ITMS_SimPortDriver::CException const&) {
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
//	static void VQM_ITMS_SimRegistrar(void)														//
//																								//
//	Description:																				//
//		Registers the functions to be called within the IOC.									//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
static void VQM_ITMS_SimRegistrar(void)
{
    iocshRegister(&initFuncDef, CVQM_ITMS_SimPortDriver::VQM_ITMS_SimPortDriverConfigure);
    iocshRegister(&addFuncDef, CVQM_ITMS_SimPortDriver::VQM_ITMS_SimAddIOPort);
}

extern "C" {

epicsExportRegistrar(VQM_ITMS_SimRegistrar);

}
