#include "IHeaderData.h"

#include "../../Exception.h"

#include <asynDriver.h>
#include <epicsMutex.h>

#include <map>
#include <vector>

enum EnumAvgMode { Off = 0, Running_Avg = 1, Cumulative_Moving_Avg = 2, Accumulator = 3 };
enum EnumGaugeState { EnumGaugeState_OFF = 0, EnumGaugeState_STANDBY, EnumGaugeState_ON, EnumGaugeState_SCAN, EnumGaugeState_EXTERNAL_TRIGGER_SCANNING, EnumGaugeState_EXTERNAL_TRIGGER_OFF };
enum EnumLogicalInstruments { FILAMENT = 0, FILAMENTBIAS, REPELLERBIAS, ENTRYPLATE, PRESSUREPLATE, CUPS, TRANSITION, EXITPLATE, EMSHIELD, EMBIAS, RFAMP };

class GaugeDAQ
{
public:
	struct InstrumentVoltage {
		double m_Min;
		double m_Max;
		InstrumentVoltage() {
			m_Min = 0; m_Max = 0;
		}
	};
	GaugeDAQ(asynUser* IOUser);
	EnumGaugeState GetGaugeState() const;
	void SetGaugeState(EnumGaugeState gaugeState);
	void GetLogicalInstrumentMinMaxVoltage(EnumLogicalInstruments logicalInstrumentEnum);
	void DataAnalysisSetAvgMode(EnumAvgMode mode) {
		m_AvgMode = mode;
	}
	EnumAvgMode AvgMode() const {
		return m_AvgMode;
	}
	void DataAnalysisSetNumAvgs(int numAverages) {
		m_numAverages = numAverages;
	}
	int	numAverages() const {
		return m_numAverages;
	}
	double EmissionCurrent() const {
		return m_EmissionCurrent;
	}
	void SetFilamentEmissionCurrentSetpoint(double value);
	double GetLogicalInstrumentCurrentVoltage(EnumLogicalInstruments logicalInstrumentEnum) const;
	void SetLogicalInstrumentVoltageSetpoint(EnumLogicalInstruments logicalInstrumentEnum, double& value);
	double GetElectrometerGain() const {
		return m_HeaderData.m_ElectronMultiplierElectrometerGain;
	}
	std::string EnumToText(EnumLogicalInstruments logicalInstrumentEnum) const;
	double lowerRange() const {
		return m_lowerRange;
	}
	double upperRange() const {
		return m_upperRange;
	}
	void SetScanRange(double lowerRange, double upperRange);
	bool GrabScanData();
	float AverageNoise() const;
	std::vector<epicsUInt16> const& NoisePacket() const {
		return m_NoisePacket;
	}
	std::vector<epicsUInt16> const& DataPacket() const {
		return m_DataPacket;
	}
	epicsUInt16 DataPacket(size_t Sample) const {
		return m_DataPacket[Sample];
	}
	int lastScanNumber() const {
		return m_lastScanNumber;
	}
	int FindRawPt(size_t& Segment, size_t ScaledPt) const;
	IHeaderData const& HeaderData() {
		return m_HeaderData;
	}

private:
	void connect();
	void GetTSETingsValues(std::string const&  TSETingsValues);
	size_t FindMarkerPos(std::string const& HeaderData, size_t Offset, const char* FirstMarker) const;
	size_t FindMarkerPos(std::string const& HeaderData, size_t Offset, const char* FirstMarker, const char* SecondMarker) const;
	void ThrowException(asynStatus Status, const char* Function, std::string const& what) const;
	void write(std::string const& WritePacket) const;
	void writeRead(std::string const& WritePacket, std::string& ReadPacket) const;
	void readTill(std::string& ReadPacket, std::string const& Termination, int AdditionalChars) const;
	void read(std::string& ReadPacket) const;
	void read(std::vector<epicsUInt16>& ReadPacket) const;
	int CheckExtraData();

	class CException : public std::runtime_error
	{
		asynStatus m_Status;
	public:
		CException(asynUser* AsynUser, asynStatus Status, const char* functionName, std::string const& what);
		asynStatus Status() const {
			return m_Status;
		}
	};
	struct SegmentBoundary {
		SegmentBoundary() {
		}
		size_t m_RawPoint;
		double m_ScaledPoint;
	};
	asynUser* m_IOUser;
	std::vector<SegmentBoundary> m_SegmentBoundaries;
	std::vector<epicsUInt16> m_NoisePacket;
	std::vector<epicsUInt16> m_DataPacket;
	IHeaderData m_HeaderData;
	EnumAvgMode m_AvgMode;
	int	m_numAverages; 
	int m_lastScanNumber;
	EnumGaugeState m_GaugeState;
	double m_lowerRange;
	double m_upperRange;
	double m_EmissionCurrent;
	std::map<EnumLogicalInstruments, InstrumentVoltage> m_InstrumentVoltages;
	mutable epicsMutex m_Mutex;
};

