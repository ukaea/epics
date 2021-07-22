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
//	LeyboldTurbo is distributed subject to a Software License Agreement								//
//	found in file LICENSE that is included with this distribution.									//
//																									//
//////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef ParameterDefns_H
#define ParameterDefns_H

#include <asynParamType.h>

enum ParamUseCases
{
	Single, NotForSim, NotForReal, Both
};

struct ParameterDefn
{
	const char*		m_Name;
	asynParamType	m_Type;
	ParamUseCases	m_UseCase;
};

static const char* RUNNING = "RUNNING"; 
static const char* RESET = "RESET";
static const char* FAULT = "FAULT";
static const char* DISCONNECTED = "DISCONNECTED";
static const char* FAULTSTR = "FAULTSTR";
static const char* FIRMWAREVERSION = "FIRMWAREVERSION";
static const char* SOFTWAREVERSION = "SOFTWAREVERSION";
static const char* ASYNVERSION = "ASYNVERSION";
static const char* WARNINGTEMPERATURE = "WARNINGTEMPERATURE";
static const char* WARNINGTEMPERATURESTR = "WARNINGTEMPERATURESTR";
static const char* WARNINGHIGHLOAD = "WARNINGHIGHLOAD";
static const char* WARNINGHIGHLOADSTR = "WARNINGHIGHLOADSTR";
static const char* WARNINGPURGE = "WARNINGPURGE";
static const char* WARNINGPURGESTR = "WARNINGPURGESTR";
static const char* STATORFREQUENCY = "STATORFREQUENCY";
static const char* CONVERTERTEMPERATURE = "CONVERTERTEMPERATURE";
static const char* MOTORCURRENT = "MOTORCURRENT";
static const char* PUMPTEMPERATURE = "PUMPTEMPERATURE";
static const char* CIRCUITVOLTAGE = "CIRCUITVOLTAGE";

static ParameterDefn ParameterDefns[] =
{
	{RUNNING, asynParamInt32, Both},
	{RESET, asynParamInt32, NotForSim},
	{FAULT, asynParamInt32, Both},
	{DISCONNECTED, asynParamInt32, NotForReal},
	{FAULTSTR, asynParamOctet, NotForSim},
	{FIRMWAREVERSION, asynParamOctet, Both},
	{ASYNVERSION, asynParamOctet, Single},
	{SOFTWAREVERSION, asynParamOctet, Single},
	{WARNINGTEMPERATURE, asynParamInt32, Both},
	{WARNINGTEMPERATURESTR, asynParamOctet, NotForSim},
	{WARNINGHIGHLOAD, asynParamInt32, Both},
	{WARNINGHIGHLOADSTR, asynParamOctet, NotForSim},
	{WARNINGPURGE, asynParamInt32, Both},
	{WARNINGPURGESTR, asynParamOctet, NotForSim},
	{STATORFREQUENCY, asynParamInt32, Both},
	{CONVERTERTEMPERATURE, asynParamInt32, Both},
	{MOTORCURRENT, asynParamFloat64, Both},
	{PUMPTEMPERATURE, asynParamInt32, Both},
	{CIRCUITVOLTAGE, asynParamFloat64, Both}
};

static const int NUM_PARAMS = sizeof(ParameterDefns) / sizeof(ParameterDefn);

#endif // ParameterDefns_H
