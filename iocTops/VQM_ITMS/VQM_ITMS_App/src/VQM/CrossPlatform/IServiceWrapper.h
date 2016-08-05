#ifndef ISERVIVEWRAPPER_H_INCLUDED
#define ISERVIVEWRAPPER_H_INCLUDED

#ifdef epicsExportSharedSymbols
#define IServiceWrapperepicsExportSharedSymbols
#undef epicsExportSharedSymbols
#endif

#include <epicsEvent.h>
#include <epicsTypes.h>

#include <queue>
#include <map>

#ifdef IServiceWrapperepicsExportSharedSymbols
#define epicsExportSharedSymbols
#undef IServiceWrapperepicsExportSharedSymbols
#include <shareLib.h>
#endif

#include "SVQM_800_Error.h"
#include "GaugeDAQ.h"

//Maps to the list of logical instruments.
enum EnumPressureSource { EnumPressureSource_None = 0, _390_TPMK, _307_350_01mA, _307_350_1mA, _307_350_10mA, _354015, _354019_354075, _360_370, _358 };

struct SAnalyzedData;
struct SAverageData;

struct epicsShareClass IServiceWrapper
{
public:
	IServiceWrapper();
	~IServiceWrapper();
	SVQM_800_Error ConnectToDevice(asynUser* IOUser, bool isMaster);
	SVQM_800_Error GetExtendedRangeCapabilities(bool& isExtendedRange, asynUser* IOUser) const;
	SVQM_800_Error DataAnalysisSetAvgMode(EnumAvgMode mode, asynUser* IOUser);
	SVQM_800_Error DataAnalysisSetNumAvgs(int numAverages, asynUser* IOUser);
	SVQM_800_Error SetFilamentEmissionCurrentSetpoint(double value, asynUser* IOUser);
	SVQM_800_Error SetElectrometerGainSetpoint(double value, asynUser* IOUser);
	SVQM_800_Error SetLogicalInstrumentVoltageSetpoint(EnumLogicalInstruments logicalInstrumentEnum, double& value, asynUser* IOUser);
	SVQM_800_Error GetScanRange(double& lowerRange, double& upperRange, asynUser* IOUser) const;
	SVQM_800_Error SetScanRange(double& lowerRange, double& upperRange, asynUser* IOUser);
	SVQM_800_Error GetLogicalInstrumentMinMaxVoltage(EnumLogicalInstruments logicalInstrumentEnum, asynUser* IOUser);
	SVQM_800_Error GetScanData(int& lastScanNumber, SAnalyzedData& analyzedData, IHeaderData** headerDataPtr,
                               SAverageData& averageData, asynUser* IOUser, bool& isValidData, EnumGaugeState& controllerState);
	SVQM_800_Error SetGaugeState(EnumGaugeState gaugeState, asynUser* IOUser);
	SVQM_800_Error GetGaugeState(EnumGaugeState& gaugeState, asynUser* IOUser) const;
private:
	GaugeDAQ const& getGaugeDAQ(asynUser* IOUser) const;
	GaugeDAQ& getGaugeDAQ(asynUser* IOUser);

private:
	std::map<asynUser*, GaugeDAQ*> m_GaugeDAQs;
};

#endif // ISERVIVEWRAPPER_H_INCLUDED
