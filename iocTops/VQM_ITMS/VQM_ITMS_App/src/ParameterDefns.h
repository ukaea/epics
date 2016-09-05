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

enum ParamUseCases
{
	Single, Instance
};

struct ParameterDefn
{
	const char*		m_Name;
	asynParamType	m_Type;
	ParamUseCases	m_UseCase;
	static const char* SCANNING;
	static const char* AVERAGES;
	static const char* AVERAGEMODE;
	static const char* LASTSCANNUMBER;
	static const char* EMISSION;
	static const char* FILAMENTBIAS;
	static const char* FILAMENTPOWER;
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
	static const char* PARTIALPRESSURE;
	static const char* TOTALPRESSURE;
	static const char* FAULT;
	static const char* DISCONNECTED;
	static const char* FIRMWAREVERSION;
	static const char* HARDWAREVERSION;
	static const char* MACHINEID;
	static const char* SOFTWAREVERSION;
	static const char* ASYNVERSION;
};

static ParameterDefn ParameterDefns[] =
{
	{ParameterDefn::SCANNING, asynParamInt32, Instance},
	{ParameterDefn::AVERAGES, asynParamInt32, Instance},
	{ParameterDefn::AVERAGEMODE, asynParamInt32, Instance},
	{ParameterDefn::LASTSCANNUMBER, asynParamInt32, Instance},
	{ParameterDefn::EMISSION, asynParamFloat64, Instance},
	{ParameterDefn::FILAMENTBIAS, asynParamFloat64, Instance},
	{ParameterDefn::FILAMENTPOWER, asynParamFloat64, Instance},
	{ParameterDefn::REPELLERBIAS, asynParamFloat64, Instance},
	{ParameterDefn::ENTRYPLATE, asynParamFloat64, Instance},
	{ParameterDefn::PRESSUREPLATE, asynParamFloat64, Instance},
	{ParameterDefn::CUPS, asynParamFloat64, Instance},
	{ParameterDefn::TRANSITION, asynParamFloat64, Instance},
	{ParameterDefn::EXITPLATE, asynParamFloat64, Instance},
	{ParameterDefn::EMSHIELD, asynParamFloat64, Instance},
	{ParameterDefn::EMBIAS, asynParamFloat64, Instance},
	{ParameterDefn::RFAMP, asynParamFloat64, Instance},
	{ParameterDefn::MASSCAL, asynParamFloat64, Instance},
	{ParameterDefn::ELECTROMETERGAIN, asynParamFloat64, Instance},
	{ParameterDefn::MASSFROM, asynParamFloat64, Instance},
	{ParameterDefn::MASSTO, asynParamFloat64, Instance},
	{ParameterDefn::RAWDATA, asynParamFloat32Array, Instance},
	{ParameterDefn::PARTIALPRESSURE, asynParamFloat32Array, Instance},
	{ParameterDefn::TOTALPRESSURE, asynParamFloat64, Instance},
	{ParameterDefn::FAULT, asynParamInt32, Instance},
	{ParameterDefn::FIRMWAREVERSION, asynParamOctet, Instance},
	{ParameterDefn::HARDWAREVERSION, asynParamOctet, Instance},
	{ParameterDefn::MACHINEID, asynParamOctet, Instance},
	{ParameterDefn::ASYNVERSION, asynParamOctet, Single},
	{ParameterDefn::SOFTWAREVERSION, asynParamOctet, Single}
};

static const int NUM_PARAMS = sizeof(ParameterDefns) / sizeof(ParameterDefn);

#endif // ParameterDefns_H
