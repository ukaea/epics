//////////////////////////////////////////////////////////////////////////////////////////////////////
//																									//
//	Module:																							//
//		VQM_ITMSBase.cpp																				//
//																									//
//	Description:																					//
//		Declares the CVQM_ITMS_Base class that forms an intermediate base class for both				//
//		CVQM_ITMSPortDriver and CVQM_ITMS_SimPortDriver.											//
//		Includes parameter management and error handling.											//
//																									//
//	Author:  Peter Heesterman (Tessella plc). Date: 04 Mar 2015.									//
//	Written for CCFE (Culham Centre for Fusion Energy).												//
//																									//
//	VQM_ITMS is distributed subject to a Software License Agreement								//
//	found in file LICENSE that is included with this distribution.									//
//																									//
//////////////////////////////////////////////////////////////////////////////////////////////////////

#include "VQM_ITMS_Base.h"

const char* ParameterDefn::SCANNING = "SCANNING";
const char* ParameterDefn::AVERAGES = "AVERAGES";
const char* ParameterDefn::AVERAGEMODE = "AVERAGEMODE";
const char* ParameterDefn::LASTSCANNUMBER = "LASTSCANNUMBER";
const char* ParameterDefn::EMISSION = "EMISSION";
const char* ParameterDefn::FILAMENTBIAS = "FILAMENTBIAS";
const char* ParameterDefn::FILAMENTPOWER = "FILAMENTPOWER";
const char* ParameterDefn::REPELLERBIAS = "REPELLERBIAS";
const char* ParameterDefn::ENTRYPLATE = "ENTRYPLATE";
const char* ParameterDefn::PRESSUREPLATE = "PRESSUREPLATE";
const char* ParameterDefn::CUPS = "CUPS";
const char* ParameterDefn::TRANSITION = "TRANSITION";
const char* ParameterDefn::EXITPLATE = "EXITPLATE";
const char* ParameterDefn::EMSHIELD = "EMSHIELD";
const char* ParameterDefn::EMBIAS = "EMBIAS";
const char* ParameterDefn::RFAMP = "RFAMP";
const char* ParameterDefn::MASSCAL = "MASSCAL";
const char* ParameterDefn::ELECTROMETERGAIN = "ELECTROMETERGAIN";
const char* ParameterDefn::MASSFROM = "MASSFROM";
const char* ParameterDefn::MASSTO = "MASSTO";
const char* ParameterDefn::RAWDATA = "RAWDATA";
const char* ParameterDefn::PARTIALPRESSURE = "PARTIALPRESSURE";
const char* ParameterDefn::TOTALPRESSURE = "TOTALPRESSURE";
const char* ParameterDefn::FAULT = "FAULT";
const char* ParameterDefn::DISCONNECTED = "DISCONNECTED";
const char* ParameterDefn::FIRMWAREVERSION = "FIRMWAREVERSION";
const char* ParameterDefn::HARDWAREVERSION = "HARDWAREVERSION";
const char* ParameterDefn::MACHINEID = "MACHINEID";
const char* ParameterDefn::SOFTWAREVERSION = "SOFTWAREVERSION";
const char* ParameterDefn::ASYNVERSION = "ASYNVERSION";

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
// CVQM_ITMS_Base::CVQM_ITMS_Base(const char *portName, int maxAddr, int paramTableSize)/
//																								//
//	Description:																				//
//		Class constructor.																		//
//		Mask() creates the interrupt and interface mask.										//
//	Parameters:																					//
//		portName - the IOC port name to be used.												//
//		numTraps - how many pumps will be attached?												//
//				 - The expectation is that addIOPort will be called this many times				//
//				 - from the st.cmd script.														//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
CVQM_ITMS_Base::CVQM_ITMS_Base(const char *portName, int maxAddr, int paramTableSize) :
	asynPortDriver(portName, maxAddr, paramTableSize,  Mask(), Mask(), ASYN_MULTIDEVICE | ASYN_CANBLOCK, 1, 0, 0) 
{
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
	setStringParam(ParameterDefn::ASYNVERSION, Buf);

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
		setStringParam (ParameterDefn::SOFTWAREVERSION, CBuf);
		fclose(Version);
	}
	else
		setStringParam (ParameterDefn::SOFTWAREVERSION, SoftwareVersion);
}

int CVQM_ITMS_Base::Mask()
{
	return asynDrvUserMask | asynInt32Mask | asynFloat64Mask | asynOctetMask | asynFloat32ArrayMask;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
//	void CVQM_ITMS_Base::createParam(size_t list, size_t ParamIndex)								//
//	void CVQM_ITMS_Base::createParam(size_t ParamIndex)											//
//																								//
//	Description:																				//
//		These methods create and initialise a parameter.										//
//		The latter is used only by this base class for single-instance PVs.						//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
void CVQM_ITMS_Base::createParam(size_t list, size_t ParamIndex) 
{
	int index;
	const char* ParamName = ParameterDefns[ParamIndex].m_Name;
	asynStatus Status = asynPortDriver::createParam(int(list), ParamName, ParameterDefns[ParamIndex].m_Type, &index);
	if (Status != asynSuccess)
		throw CException(pasynUserSelf, Status, __FILE__, __FUNCTION__, ParamName);
	m_Parameters[ParamName] = index;
	ParamDefaultValue(list, ParamIndex);
}

void CVQM_ITMS_Base::createParam(size_t ParamIndex) 
{
	int index;
	const char* ParamName = ParameterDefns[ParamIndex].m_Name;
	asynStatus Status = asynPortDriver::createParam(ParamName, ParameterDefns[ParamIndex].m_Type, &index);
	if (Status != asynSuccess)
		throw CException(pasynUserSelf, Status, __FILE__, __FUNCTION__, ParamName);
	m_Parameters[ParamName] = index;
	ParamDefaultValue(ParamIndex);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
//	void CVQM_ITMS_Base::ParamDefaultValue(size_t ParamIndex)										//
//																								//
//	Description:																				//
//		Private utility method, initialises each PV with a default value of the desired type.	//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
void CVQM_ITMS_Base::ParamDefaultValue(size_t ParamIndex)
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

void CVQM_ITMS_Base::ParamDefaultValue(size_t list, size_t ParamIndex)
{
	const char* ParamName = ParameterDefns[ParamIndex].m_Name;
	switch(ParameterDefns[ParamIndex].m_Type)
	{
		// Set default values.
		case asynParamInt32: 
			setIntegerParam(list, ParamName, 0);
			break;
		case asynParamFloat64: 
			setDoubleParam (list, ParamName, 0.0);
			break;
		case asynParamOctet: 
			setStringParam (list, ParamName, "");
			break;
//		default: assert(false);
	}
}
