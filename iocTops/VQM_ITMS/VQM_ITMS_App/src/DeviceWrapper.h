#include <string>

enum EnumAvgMode { Off = 0, Running_Avg = 1, Cumulative_Moving_Avg = 2, Accumulator = 3 };
//Maps to the list of logical instruments.
enum EnumLogicalInstruments { FILAMENT = 0, FILAMENTBIAS, REPELLERBIAS, ENTRYPLATE, PRESSUREPLATE, CUPS, TRANSITION, EXITPLATE, EMSHIELD, EMBIAS, RFAMP };
enum EnumGaugeState { EnumGaugeState_OFF = 0, EnumGaugeState_STANDBY, EnumGaugeState_ON, EnumGaugeState_SCAN, EnumGaugeState_EXTERNAL_TRIGGER_SCANNING, EnumGaugeState_EXTERNAL_TRIGGER_OFF };

#include <asynDriver.h>

class DeviceWrapper
{
public:
	asynStatus DataAnalysisSetAvgMode(EnumAvgMode mode, asynUser* IOUser);
	asynStatus DataAnalysisSetNumAvgs(int numAverages, asynUser* IOUser);
	asynStatus GetFilamentEmissionCurrent(double& value, asynUser* IOUser) const;
	asynStatus SetFilamentEmissionCurrentSetpoint(double& value, asynUser* IOUser);
	asynStatus GetLogicalInstrumentCurrentVoltage(double& value, EnumLogicalInstruments logicalInstrumentEnum, asynUser* IOUser) const;
	asynStatus SetLogicalInstrumentVoltageSetpoint(EnumLogicalInstruments logicalInstrumentEnum, double& value, asynUser* IOUser);
	asynStatus GetMassCalibrationFactor(float& value, asynUser* IOUser) const;	
	asynStatus GetElectrometerGain(double& value, asynUser* IOUser) const;
	asynStatus GetScanRange(double& lowerRange, double& upperRange, asynUser* IOUser) const;
	asynStatus SetScanRange(double& lowerRange, double& upperRange, asynUser* IOUser);
//	asynStatus GetScanData(int& lastScanNumber, SAnalyzedData& analyzedData, IHeaderData** headerDataPtr,
//                                            SAverageData& averageData, const SDeviceConnectionInfo& connectInfo, bool& isValidData, EnumGaugeState& controllerState) = 0;
	asynStatus SetGaugeState(EnumGaugeState gaugeState, asynUser* IOUser);
	asynStatus GetGaugeState(EnumGaugeState& gaugeState, asynUser* IOUser);
private:
	asynStatus write(asynUser* IOUser, std::string const& WritePacket);
	asynStatus writeRead(asynUser* IOUser, std::string const& WritePacket);
	std::string EnumToText(EnumLogicalInstruments logicalInstrumentEnum) const;

private:
	std::string m_ReadBuffer;
};
