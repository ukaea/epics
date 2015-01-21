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
	const char* ParamName;
	asynParamType ParamType;
};

static const char* RUNNING = "RUNNING"; 
static const char* RESET = "RESET";
static const char* FAULT = "FAULT";
static const char* WARNINGTEMPERATURE = "WARNINGTEMPERATURE";
static const char* WARNINGHIGHLOAD = "WARNINGHIGHLOAD";
static const char* STATORFREQUENCY = "STATORFREQUENCY";
static const char* CONVERTERTEMPERATURE = "CONVERTERTEMPERATURE";
static const char* MOTORCURRENT = "MOTORCURRENT";
static const char* PUMPTEMPERATURE = "PUMPTEMPERATURE";
static const char* CIRCUITVOLTAGE = "CIRCUITVOLTAGE";

ParameterDefn ParameterDefns[] =
{
	{RUNNING, asynParamInt32},
	{RESET, asynParamInt32},
	{FAULT, asynParamInt32},
	{WARNINGTEMPERATURE, asynParamInt32},
	{WARNINGHIGHLOAD, asynParamInt32},
	{STATORFREQUENCY, asynParamInt32},
	{CONVERTERTEMPERATURE, asynParamInt32},
	{MOTORCURRENT, asynParamFloat64},
	{PUMPTEMPERATURE, asynParamInt32},
	{CIRCUITVOLTAGE, asynParamFloat64}
};

static const int NUM_PARAMS = sizeof(ParameterDefns) / sizeof(ParameterDefn);

#endif // ParameterDefns_H
