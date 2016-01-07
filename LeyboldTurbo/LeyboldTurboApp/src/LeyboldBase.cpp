//////////////////////////////////////////////////////////////////////////////////////////////////////
//																									//
//	Module:																							//
//		LeyboldBase.cpp																				//
//																									//
//	Description:																					//
//		Declares the CLeyboldBase class that forms an intermediate base class for both				//
//		CLeyboldTurboPortDriver and CLeyboldSimPortDriver.											//
//		Includes parameter management and error handling.											//
//																									//
//	Author:  Peter Heesterman (Tessella plc). Date: 04 Mar 2015.									//
//	Written for CCFE (Culham Centre for Fusion Energy).												//
//																									//
//	LeyboldTurbo is distributed subject to a Software License Agreement								//
//	found in file LICENSE that is included with this distribution.									//
//																									//
//////////////////////////////////////////////////////////////////////////////////////////////////////

#define epicsExportSharedSymbols
#include "LeyboldBase.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
// CLeyboldBase::CLeyboldBase(const char *portName, int maxAddr, int paramTableSize, int NoOfPZD)/
//																								//
//	Description:																				//
//		Class constructor.																		//
//		Mask() creates the interrupt and interface mask.										//
//	Parameters:																					//
//		portName - the IOC port name to be used.												//
//		numPumps - how many pumps will be attached?												//
//				 - The expectation is that addIOPort will be called this many times				//
//				 - from the st.cmd script.														//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
CLeyboldBase::CLeyboldBase(const char *portName, int maxAddr, int paramTableSize, int NoOfPZD) :
	asynPortDriver(portName, maxAddr, paramTableSize,  Mask(), Mask(), ASYN_MULTIDEVICE, 1, 0, 0) 
{
	m_NoOfPZD = NoOfPZD;
	for (size_t ParamIndex = 0; ParamIndex < size_t(NUM_PARAMS); ParamIndex++)
	{
		if (ParameterDefns[ParamIndex].m_UseCase != Single)
			// Non-single instance parameter
			continue;

		createParam(ParamIndex);
	}
	// Get the Asyn version values from AsynDriver.h.
	// The parameter is avasilable for use by o/s scrips that need to know whether, or not, to invoke the PROC field
	// in order to force output of the I/O parameter.
	char Buf[5];
	epicsSnprintf(Buf, sizeof(Buf), "%1d-%02d", ASYN_VERSION, ASYN_REVISION);
	setStringParam(ASYNVERSION, Buf);

	if ((ASYN_VERSION >= 4) && (ASYN_REVISION >= 26))
		// This version includes the asyn:READBACK feature as per http://www.aps.anl.gov/epics/tech-talk/2015/msg00431.php
		// The environment variable set here is used by IOC start scripts to select the .db file that should be loaded.
		epicsEnvSet("ASYN_VERSION_GE426", " ");

	const char* SoftwareVersion = "1.0";
	FILE* Version = fopen("version.txt", "rt");
	if (Version)
	{
		// For CCFE, this file would typically constain an SU number, e.g. SU18923.
		char CBuf[MaxEPICSStrLen];
		fgets(CBuf, MaxEPICSStrLen, Version);
		setStringParam (SOFTWAREVERSION, CBuf);
		fclose(Version);
	}
	else
		setStringParam (SOFTWAREVERSION, SoftwareVersion);
}

int CLeyboldBase::Mask()
{
	return asynDrvUserMask | asynInt32Mask | asynFloat64Mask | asynOctetMask;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
//	void CLeyboldBase::createParam(size_t list, size_t ParamIndex)								//
//	void CLeyboldBase::createParam(size_t ParamIndex)											//
//																								//
//	Description:																				//
//		These methods create and initialise a parameter.										//
//		The latter is used only by this base class for single-instance PVs.						//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
void CLeyboldBase::createParam(size_t list, size_t ParamIndex) 
{
	int index;
	const char* ParamName = ParameterDefns[ParamIndex].m_Name;
	asynStatus Status = asynPortDriver::createParam(int(list), ParamName, ParameterDefns[ParamIndex].m_Type, &index);
	if (Status != asynSuccess)
		throw CException(pasynUserSelf, Status, __FUNCTION__, ParamName);
	m_Parameters[ParamName] = index;
	ParamDefaultValue(ParamIndex);
}

void CLeyboldBase::createParam(size_t ParamIndex) 
{
	int index;
	const char* ParamName = ParameterDefns[ParamIndex].m_Name;
	asynStatus Status = asynPortDriver::createParam(ParamName, ParameterDefns[ParamIndex].m_Type, &index);
	if (Status != asynSuccess)
		throw CException(pasynUserSelf, Status, __FUNCTION__, ParamName);
	m_Parameters[ParamName] = index;
	ParamDefaultValue(ParamIndex);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
//	void CLeyboldBase::ParamDefaultValue(size_t ParamIndex)										//
//																								//
//	Description:																				//
//		Private utility method, initialises each PV with a default value of the desired type.	//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
void CLeyboldBase::ParamDefaultValue(size_t ParamIndex)
{
	const char* ParamName = ParameterDefns[ParamIndex].m_Name;
	switch(ParameterDefns[ParamIndex].m_Type)
	{
		// Set default values.
		case asynParamInt32: 
			setIntegerParam(ParamName, 0);
			break;
		case asynParamFloat64: 
			setDoubleParam (ParamName, 0.0);
			break;
		case asynParamOctet: 
			setStringParam (ParamName, "");
			break;
		default: assert(false);
	}
}
