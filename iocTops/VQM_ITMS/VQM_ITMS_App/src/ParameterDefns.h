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
	static const char* FILAMENT;
	static const char* EMISSION;
	static const char* MASSRANGE;
	static const char* RAWDATA;
	static const char* DETECTOR;
	static const char* FAULT;
	static const char* FAULTSTR;
	static const char* DISCONNECTED;
	static const char* FIRMWAREVERSION;
	static const char* SOFTWAREVERSION;
	static const char* ASYNVERSION;
};

static ParameterDefn ParameterDefns[] =
{
	{ParameterDefn::FILAMENT, asynParamInt32, Both},
	{ParameterDefn::EMISSION, asynParamFloat64, Both},
	{ParameterDefn::DETECTOR, asynParamInt32, Both},
	{ParameterDefn::MASSRANGE, asynParamFloat64Array, Both},
	{ParameterDefn::RAWDATA, asynParamFloat32Array, Both},
	{ParameterDefn::FAULT, asynParamInt32, Both},
	{ParameterDefn::FAULTSTR, asynParamOctet, NotForSim},
	{ParameterDefn::DISCONNECTED, asynParamInt32, NotForReal},
	{ParameterDefn::FIRMWAREVERSION, asynParamOctet, Both},
	{ParameterDefn::ASYNVERSION, asynParamOctet, Single},
	{ParameterDefn::SOFTWAREVERSION, asynParamOctet, Single}
};

static const int NUM_PARAMS = sizeof(ParameterDefns) / sizeof(ParameterDefn);

#endif // ParameterDefns_H
