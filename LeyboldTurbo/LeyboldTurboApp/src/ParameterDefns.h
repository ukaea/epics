#include <asynParamType.h>

struct ParameterDefn
{
	const char* ParamName;
	asynParamType ParamType;
};

static const char* STARTSTOP = "STARTSTOP"; 
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
	{STARTSTOP, asynParamUInt32Digital},
	{RESET, asynParamUInt32Digital},
	{FAULT, asynParamUInt32Digital},
	{WARNINGTEMPERATURE, asynParamUInt32Digital},
	{WARNINGHIGHLOAD, asynParamUInt32Digital},
	{STATORFREQUENCY, asynParamInt32},
	{CONVERTERTEMPERATURE, asynParamInt32},
	{MOTORCURRENT, asynParamFloat64},
	{PUMPTEMPERATURE, asynParamInt32},
	{CIRCUITVOLTAGE, asynParamFloat64}
};

static const int NUM_PARAMS = sizeof(ParameterDefns) / sizeof(ParameterDefn);
