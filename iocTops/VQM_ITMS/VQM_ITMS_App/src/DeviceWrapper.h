#include <epicsEvent.h>
#include <epicsMutex.h>

#include <queue>

enum EnumAvgMode { Off = 0, Running_Avg = 1, Cumulative_Moving_Avg = 2, Accumulator = 3 };
//Maps to the list of logical instruments.
enum EnumLogicalInstruments { FILAMENT = 0, FILAMENTBIAS, REPELLERBIAS, ENTRYPLATE, PRESSUREPLATE, CUPS, TRANSITION, EXITPLATE, EMSHIELD, EMBIAS, RFAMP };
enum EnumGaugeState { EnumGaugeState_OFF = 0, EnumGaugeState_STANDBY, EnumGaugeState_ON, EnumGaugeState_SCAN, EnumGaugeState_EXTERNAL_TRIGGER_SCANNING, EnumGaugeState_EXTERNAL_TRIGGER_OFF };

#include <asynDriver.h>

class IHeaderData;

class DeviceWrapper
{
public:
	struct InstrumentVoltage {
		double m_Min;
		double m_Current;
		double m_Max;
		InstrumentVoltage() {
			m_Min = 0; m_Current = 0; m_Max = 0;
		}
	};
	DeviceWrapper(asynUser* IOUser);
	asynStatus DataAnalysisSetAvgMode(EnumAvgMode mode, asynUser* IOUser);
	asynStatus DataAnalysisSetNumAvgs(int numAverages, asynUser* IOUser);
	asynStatus GetFilamentEmissionCurrent(double& value, asynUser* IOUser) const;
	asynStatus SetFilamentEmissionCurrentSetpoint(double& value, asynUser* IOUser);
	asynStatus GetLogicalInstrumentCurrentVoltage(double& value, EnumLogicalInstruments logicalInstrumentEnum, asynUser* IOUser) const;
	asynStatus SetLogicalInstrumentVoltageSetpoint(EnumLogicalInstruments logicalInstrumentEnum, double& value, asynUser* IOUser);
	asynStatus GetMassCalibrationFactor(float& value, asynUser* IOUser) const;	
	asynStatus GetElectrometerGain(double& value, asynUser* IOUser) const;
	asynStatus GetScanRange(double& lowerRange, double& upperRange, asynUser* IOUser);
	asynStatus SetScanRange(double& lowerRange, double& upperRange, asynUser* IOUser);
	asynStatus GetLogicalInstrumentMinMaxVoltage(double& min, double& max, EnumLogicalInstruments logicalInstrumentEnum, asynUser* IOUser) const;
	asynStatus GetScanData(int& lastScanNumber, IHeaderData** headerDataPtr,
                                            asynUser* IOUser, bool& isValidData, EnumGaugeState& controllerState);
	asynStatus SetGaugeState(EnumGaugeState gaugeState, asynUser* IOUser);
	asynStatus GetGaugeState(EnumGaugeState& gaugeState, asynUser* IOUser);
private:
	size_t FindMarkerPos(std::string const& HeaderData, size_t Offset, const char* FirstMarker, const char* SecondMarker);
	asynStatus write(asynUser* IOUser, std::string const& WritePacket) const;
	asynStatus DeviceWrapper::writeRead(asynUser* IOUser, std::string const& WritePacket, std::string& ReadPacket) const;
	asynStatus DeviceWrapper::readTill(asynUser* IOUser, std::string& ReadPacket, std::string const& Termination) const;
	asynStatus DeviceWrapper::read(asynUser* IOUser, std::vector<char>& ReadPacket) const;
	std::string EnumToText(EnumLogicalInstruments logicalInstrumentEnum) const;
private:
	EnumGaugeState m_GaugeState;
	InstrumentVoltage m_Filament;
	InstrumentVoltage m_Repeller;
	InstrumentVoltage m_EntryPlate;
	InstrumentVoltage m_PressurePlate;
	InstrumentVoltage m_Cups;
	InstrumentVoltage m_Transition;
	InstrumentVoltage m_ExitPlate;
	InstrumentVoltage m_EMShield;
	InstrumentVoltage m_EMBias;
	InstrumentVoltage m_RFAmp;
	mutable epicsMutex m_Mutex;
};
