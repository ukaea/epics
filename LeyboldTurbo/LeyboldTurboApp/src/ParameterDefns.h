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

struct ParameterDefn
{
	const char*		m_ParamName;
	asynParamType	m_ParamType;
	bool			m_NotForSim;
};

static const char* RUNNING = "RUNNING"; 
static const char* RESET = "RESET";
static const char* FAULT = "FAULT";
static const char* FAULTSTR = "FAULTSTR";
static const char* FIRMWAREVERSION = "FIRMWAREVERSION";
static const char* SOFTWAREVERSION = "SOFTWAREVERSION";
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

ParameterDefn ParameterDefns[] =
{
	{RUNNING, asynParamInt32, false},
	{RESET, asynParamInt32, true},
	{FAULT, asynParamInt32, false},
	{FAULTSTR, asynParamOctet, true},
	{FIRMWAREVERSION, asynParamOctet, false},
	{SOFTWAREVERSION, asynParamOctet, true},
	{WARNINGTEMPERATURE, asynParamInt32, false},
	{WARNINGTEMPERATURESTR, asynParamOctet, true},
	{WARNINGHIGHLOAD, asynParamInt32, false},
	{WARNINGHIGHLOADSTR, asynParamOctet, true},
	{WARNINGPURGE, asynParamInt32, false},
	{WARNINGPURGESTR, asynParamOctet, true},
	{STATORFREQUENCY, asynParamInt32, false},
	{CONVERTERTEMPERATURE, asynParamInt32, false},
	{MOTORCURRENT, asynParamFloat64, false},
	{PUMPTEMPERATURE, asynParamInt32, false},
	{CIRCUITVOLTAGE, asynParamFloat64, false}
};

static const int NUM_PARAMS = sizeof(ParameterDefns) / sizeof(ParameterDefn);

#endif // ParameterDefns_H
