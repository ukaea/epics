#include "IHeaderData.h"

#include "../../Exception.h"

#include <asynDriver.h>
#include <epicsMutex.h>

#include <map>
#include <vector>
#include <deque>

enum EnumAvgMode { Off = 0, Running_Avg = 1, Cumulative_Moving_Avg = 2, Accumulator = 3 };
enum EnumGaugeState { EnumGaugeState_OFF = 0, EnumGaugeState_STANDBY, EnumGaugeState_ON, EnumGaugeState_SCAN, EnumGaugeState_EXTERNAL_TRIGGER_SCANNING, EnumGaugeState_EXTERNAL_TRIGGER_OFF };
enum EnumLogicalInstruments { FILAMENT = 0, FILAMENTBIAS, REPELLERBIAS, ENTRYPLATE, PRESSUREPLATE, CUPS, TRANSITION, EXITPLATE, EMSHIELD, EMBIAS, RFAMP };

class GaugeDAQ
{
public:
	static const size_t MaxNrSegments;
	static size_t SegmentSizes[];
	struct InstrumentVoltage {
		double m_Min;
		double m_Max;
		InstrumentVoltage() {
			m_Min = 0; m_Max = 0;
		}
	};
	struct SegmentBoundary {
		SegmentBoundary(size_t RawPoint, double ScaledPoint) {
			m_RawPoint = RawPoint;
			m_ScaledPoint = ScaledPoint;
		}
		size_t m_RawPoint;
		double m_ScaledPoint;
	};
	GaugeDAQ(asynUser* IOUser);
	bool GetExtendedRangeCapabilities() const;
	EnumGaugeState GetGaugeState() const;
	EnumGaugeState GaugeState() const {
		return m_GaugeState;
	}
	void SetGaugeState(EnumGaugeState gaugeState);
	void GetLogicalInstrumentMinMaxVoltage(EnumLogicalInstruments logicalInstrumentEnum);
	void DataAnalysisSetAvgMode(EnumAvgMode AvgMode);
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
	void SetElectrometerGainSetpoint(double value);
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
	void GetScanRange(double& lowerRange, double& upperRange) const {
		lowerRange = m_lowerRange;
		upperRange = m_upperRange;
	}
	void SetScanRange(double lowerRange, double upperRange);
	void GrabScanData();
	float AverageNoise() const;
	std::vector<epicsUInt16> const& NoiseData() const {
		return m_NoiseData;
	}
	std::deque<std::vector<epicsUInt16> > const& RawData() const {
		return m_RawData;
	}
	epicsUInt16 RawData(size_t Sample) const {
		return m_RawData.front()[Sample];
	}
	size_t RawDataSize() const {
		if (m_RawData.size() == 0)
			return 0;
		return m_RawData.front().size();
	}
	int lastScanNumber() const {
		return m_lastScanNumber;
	}
	size_t FindRawPt(size_t& RawPt, double ScaledPt) const;
	IHeaderData const& HeaderData() {
		return m_HeaderData;
	}

private:
	void connect();
	void GetScanRange();
	void GetIDENtifyValues(std::string const& IDENtify);
	void GetTSETingsValues(std::string const&  MEASurementTSETtingsValues);
	void GetSEGMentValues(std::string const&  MEASurementSEGMentValues);
	size_t GetDATAAMUValues(std::string const&  DATAAMUValues);
	size_t FindMarkerPos(std::string const& HeaderData, size_t Offset, const char* Marker) const;
	void ThrowException(asynStatus Status, const char* Function, std::string const& what) const;
	void write(std::string const& WritePacket) const;
	void writeRead(std::string const& WritePacket, std::string& ReadPacket) const;
	void readTill(std::string& ReadPacket, std::string const& Termination, int AdditionalChars) const;
	void read(std::string& ReadPacket) const;
	template<class T> void read(std::vector<T>& ReadPacket) const;
	int Flush();

	class CException : public std::runtime_error
	{
		asynStatus m_Status;
	public:
		CException(asynUser* AsynUser, asynStatus Status, const char* functionName, std::string const& what);
		asynStatus Status() const {
			return m_Status;
		}
	};
	asynUser* m_IOUser;
	std::vector<epicsUInt16> m_NoiseData;
	std::vector<epicsFloat32> m_ScanVector;
	std::deque<std::vector<epicsUInt16> > m_RawData;
	IHeaderData m_HeaderData;
	EnumAvgMode m_AvgMode;
	size_t	m_numAverages; 
	int m_lastScanNumber;
	EnumGaugeState m_GaugeState;
	double m_lowerRange;
	double m_upperRange;
	double m_EmissionCurrent;
	std::map<EnumLogicalInstruments, InstrumentVoltage> m_InstrumentVoltages;
	mutable epicsMutex m_Mutex;
};

