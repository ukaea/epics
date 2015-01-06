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
	int DefaultValue;
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
	{RUNNING, asynParamInt32, 1},
	{RESET, asynParamInt32, 0},
	{FAULT, asynParamInt32, 0},
	{WARNINGTEMPERATURE, asynParamInt32, 0},
	{WARNINGHIGHLOAD, asynParamInt32, 0},
	{STATORFREQUENCY, asynParamInt32, 500},
	{CONVERTERTEMPERATURE, asynParamInt32, 50},
	{MOTORCURRENT, asynParamFloat64, 10},
	{PUMPTEMPERATURE, asynParamInt32, 40},
	{CIRCUITVOLTAGE, asynParamFloat64, 30}
};

static const int NUM_PARAMS = sizeof(ParameterDefns) / sizeof(ParameterDefn);

#endif // ParameterDefns_H
