#ifndef ISERVIVEWRAPPER_H_INCLUDED
#define ISERVIVEWRAPPER_H_INCLUDED

#ifdef epicsExportSharedSymbols
#define IServiceWrapperepicsExportSharedSymbols
#undef epicsExportSharedSymbols
#endif

#include <epicsEvent.h>
#include <epicsMutex.h>

#include <queue>
#include <map>

#ifdef IServiceWrapperepicsExportSharedSymbols
#define epicsExportSharedSymbols
#undef IServiceWrapperepicsExportSharedSymbols
#include <shareLib.h>
#endif

#include "SVQM_800_Error.h"

#include "IHeaderData.h"

enum EnumAvgMode { Off = 0, Running_Avg = 1, Cumulative_Moving_Avg = 2, Accumulator = 3 };
//Maps to the list of logical instruments.
enum EnumLogicalInstruments { FILAMENT = 0, FILAMENTBIAS, REPELLERBIAS, ENTRYPLATE, PRESSUREPLATE, CUPS, TRANSITION, EXITPLATE, EMSHIELD, EMBIAS, RFAMP };
enum EnumGaugeState { EnumGaugeState_OFF = 0, EnumGaugeState_STANDBY, EnumGaugeState_ON, EnumGaugeState_SCAN, EnumGaugeState_EXTERNAL_TRIGGER_SCANNING, EnumGaugeState_EXTERNAL_TRIGGER_OFF };
enum EnumPressureSource { EnumPressureSource_None = 0, _390_TPMK, _307_350_01mA, _307_350_1mA, _307_350_10mA, _354015, _354019_354075, _360_370, _358 };

#include <asynDriver.h>

struct SAnalyzedData;
struct SAverageData;

struct epicsShareClass IServiceWrapper
{
private:
	//////////////////////////////////////////////////////////////////////////////////////////////////
	//																								//
	//	class CVQM_ITMS_Driver::CException : public CVQM_ITMS_Base::CException						//
	//	Description:																				//
	//		If an error ocurrs, an object of this type is thrown.									//
	//																								//
	//////////////////////////////////////////////////////////////////////////////////////////////////
	class CException : public std::runtime_error
	{
		asynStatus m_Status;
	public:
		CException(asynUser* AsynUser, asynStatus Status, const char* functionName, std::string const& what);
		asynStatus Status() const {
			return m_Status;
		}
	};
	struct GaugeState;
public:
	struct InstrumentVoltage {
		double m_Min;
		double m_Current;
		double m_Max;
		InstrumentVoltage() {
			m_Min = 0; m_Current = 0; m_Max = 0;
		}
	};
	IServiceWrapper() {
	}
	SVQM_800_Error ConnectToDevice(asynUser* IOUser, bool isMaster);
	SVQM_800_Error DataAnalysisSetAvgMode(EnumAvgMode mode, asynUser* IOUser);
	SVQM_800_Error DataAnalysisSetNumAvgs(int numAverages, asynUser* IOUser);
	SVQM_800_Error GetFilamentEmissionCurrent(double& value, asynUser* IOUser) const;
	SVQM_800_Error SetFilamentEmissionCurrentSetpoint(double& value, asynUser* IOUser);
	SVQM_800_Error GetLogicalInstrumentCurrentVoltage(double& value, EnumLogicalInstruments logicalInstrumentEnum, asynUser* IOUser) const;
	SVQM_800_Error SetLogicalInstrumentVoltageSetpoint(EnumLogicalInstruments logicalInstrumentEnum, double& value, asynUser* IOUser);
	SVQM_800_Error GetMassCalibrationFactor(float& value, asynUser* IOUser) const;	
	SVQM_800_Error GetElectrometerGain(double& value, asynUser* IOUser) const;
	SVQM_800_Error GetScanRange(double& lowerRange, double& upperRange, asynUser* IOUser);
	SVQM_800_Error SetScanRange(double& lowerRange, double& upperRange, asynUser* IOUser);
	SVQM_800_Error GetLogicalInstrumentMinMaxVoltage(EnumLogicalInstruments logicalInstrumentEnum, asynUser* IOUser);
	SVQM_800_Error GetScanData(int& lastScanNumber, SAnalyzedData& analyzedData, IHeaderData** headerDataPtr,
                               SAverageData& averageData, asynUser* IOUser, bool& isValidData, EnumGaugeState& controllerState);
	SVQM_800_Error SetGaugeState(EnumGaugeState gaugeState, asynUser* IOUser);
	SVQM_800_Error GetGaugeState(EnumGaugeState& gaugeState, asynUser* IOUser) const;
private:
	size_t FindMarkerPos(std::string const& HeaderData, size_t Offset, const char* FirstMarker);
	size_t FindMarkerPos(std::string const& HeaderData, size_t Offset, const char* FirstMarker, const char* SecondMarker);
	void write(asynUser* IOUser, std::string const& WritePacket) const;
	void writeRead(asynUser* IOUser, std::string const& WritePacket, std::string& ReadPacket) const;
	void readTill(asynUser* IOUser, std::string& ReadPacket, std::string const& Termination) const;
	void read(asynUser* IOUser, std::vector<char>& ReadPacket) const;
	std::string EnumToText(EnumLogicalInstruments logicalInstrumentEnum) const;
	SVQM_800_Error GetTSETingsValues(GaugeState& GaugeState, std::string const&  TSETingsValues);
	void ThrowException(asynUser* pasynUser, asynStatus Status, const char* Function, std::string const& what) const;
	GaugeState const& getGaugeState(asynUser* IOUser) const {
		std::map<asynUser*, GaugeState>::const_iterator Iter = m_GaugeStates.find(IOUser);
		return Iter->second;
	}
	GaugeState& getGaugeState(asynUser* IOUser) {
		std::map<asynUser*, GaugeState>::iterator Iter = m_GaugeStates.find(IOUser);
		return Iter->second;
	}

private:
	struct GaugeState
	{
		GaugeState() {
			m_Mutex = new epicsMutex;
		}
		~GaugeState() {
			delete m_Mutex;
		}
		IHeaderData m_HeaderData;
		EnumGaugeState m_GaugeState;
		double m_lowerRange;
		double m_upperRange;
		double m_EmissionCurrent;
		std::map<EnumLogicalInstruments, InstrumentVoltage> m_InstrumentVoltages;
		mutable epicsMutex* m_Mutex;
		epicsMutex& Mutex() const {
			return *m_Mutex;
		}
	};
	std::map<asynUser*, GaugeState> m_GaugeStates;
};

#endif // ISERVIVEWRAPPER_H_INCLUDED
