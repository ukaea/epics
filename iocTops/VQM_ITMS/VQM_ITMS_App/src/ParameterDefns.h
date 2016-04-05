//////////////////////////////////////////////////////////////////////////////////////////////////////
//																									//
//	Module:																							//
//		ParameterDefns.h																			//
//																									//
//	Description:																					//
//		Declares the ParameterDefn struct.															//
//		The structure is used by both the 'real' IOC and the simulator.								//
//		Defines the strings and usage of parameters.												// 
//																									//
//	Author:  Peter Heesterman (Tessella plc). Date: 05 Jan 2015.									//
//	Written for CCFE (Culham Centre for Fusion Energy).												//
//																									//
//	VQM_ITMS is distributed subject to a Software License Agreement								//
//	found in file LICENSE that is included with this distribution.									//
//																									//
//////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef ParameterDefns_H
#define ParameterDefns_H

#include <asynParamType.h>
#include <shareLib.h>

enum ParamUseCases
{
	Single, NotForSim, NotForReal, Both
};

struct epicsShareClass ParameterDefn
{
	const char*		m_Name;
	asynParamType	m_Type;
	ParamUseCases	m_UseCase;
	static const char* SCANNING;
	static const char* AVERAGES;
	static const char* LASTSCANNUMBER;
	static const char* EMISSION;
	static const char* FILAMENTBIAS;
	static const char* REPELLERBIAS;
	static const char* ENTRYPLATE;
	static const char* PRESSUREPLATE;
	static const char* CUPS;
	static const char* TRANSITION;
	static const char* EXITPLATE;
	static const char* EMSHIELD;
	static const char* EMBIAS;
	static const char* RFAMP;
	static const char* MASSCAL;
	static const char* ELECTROMETERGAIN;
	static const char* MASSFROM;
	static const char* MASSTO;
	static const char* RAWDATA;
	static const char* PEAKAREA;
	static const char* FAULT;
	static const char* DISCONNECTED;
	static const char* FIRMWAREVERSION;
	static const char* HARDWAREVERSION;
	static const char* SOFTWAREVERSION;
	static const char* ASYNVERSION;
};

static ParameterDefn ParameterDefns[] =
{
	{ParameterDefn::SCANNING, asynParamInt32, Both},
	{ParameterDefn::AVERAGES, asynParamInt32, Both},
	{ParameterDefn::LASTSCANNUMBER, asynParamInt32, Both},
	{ParameterDefn::EMISSION, asynParamFloat64, Both},
	{ParameterDefn::FILAMENTBIAS, asynParamFloat64, Both},
	{ParameterDefn::REPELLERBIAS, asynParamFloat64, Both},
	{ParameterDefn::ENTRYPLATE, asynParamFloat64, Both},
	{ParameterDefn::PRESSUREPLATE, asynParamFloat64, Both},
	{ParameterDefn::CUPS, asynParamFloat64, Both},
	{ParameterDefn::TRANSITION, asynParamFloat64, Both},
	{ParameterDefn::EXITPLATE, asynParamFloat64, Both},
	{ParameterDefn::EMSHIELD, asynParamFloat64, Both},
	{ParameterDefn::EMBIAS, asynParamFloat64, Both},
	{ParameterDefn::RFAMP, asynParamFloat64, Both},
	{ParameterDefn::MASSCAL, asynParamFloat64, Both},
	{ParameterDefn::ELECTROMETERGAIN, asynParamFloat64, Both},
	{ParameterDefn::MASSFROM, asynParamFloat64, Both},
	{ParameterDefn::MASSTO, asynParamFloat64, Both},
	{ParameterDefn::RAWDATA, asynParamFloat32Array, Both},
	{ParameterDefn::PEAKAREA, asynParamFloat32Array, Both},
	{ParameterDefn::FAULT, asynParamInt32, Both},
	{ParameterDefn::DISCONNECTED, asynParamInt32, NotForReal},
	{ParameterDefn::FIRMWAREVERSION, asynParamOctet, Both},
	{ParameterDefn::HARDWAREVERSION, asynParamOctet, Both},
	{ParameterDefn::ASYNVERSION, asynParamOctet, Single},
	{ParameterDefn::SOFTWAREVERSION, asynParamOctet, Single}
};

static const int NUM_PARAMS = sizeof(ParameterDefns) / sizeof(ParameterDefn);

#endif // ParameterDefns_H
