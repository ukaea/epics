#include <epicsEvent.h>
#include <epicsMutex.h>

#include <queue>
#include <map>

enum EnumAvgMode { Off = 0, Running_Avg = 1, Cumulative_Moving_Avg = 2, Accumulator = 3 };
//Maps to the list of logical instruments.
enum EnumLogicalInstruments { FILAMENT = 0, FILAMENTBIAS, REPELLERBIAS, ENTRYPLATE, PRESSUREPLATE, CUPS, TRANSITION, EXITPLATE, EMSHIELD, EMBIAS, RFAMP };
enum EnumGaugeState { EnumGaugeState_OFF = 0, EnumGaugeState_STANDBY, EnumGaugeState_ON, EnumGaugeState_SCAN, EnumGaugeState_EXTERNAL_TRIGGER_SCANNING, EnumGaugeState_EXTERNAL_TRIGGER_OFF };
enum EnumPressureSource { EnumPressureSource_None = 0, _390_TPMK, _307_350_01mA, _307_350_1mA, _307_350_10mA, _354015, _354019_354075, _360_370, _358 };

#include <asynDriver.h>

//List of error types.
enum EnumErrorType { NO_ERRORS = 0, NET_EXCEPTION, NO_CONTROLLERS_FOUND, COMMUNICATION_EXCEPTION,\
        INSUFFICIENT_PRIVELEGES, NO_HEADER_DATA, NO_CONNECTION_TO_SERVICE, FAILED_TO_CONNECT_TO_CONTROLLER,\
        NO_MORE_AVAILABLE_SERVICES, NO_SERVICES_INSTALLED, SERVICE_FAILED_TO_START, CANNOT_CONNECT_TO_SERVICE, UNKNOWN_SERVICE_CONNECTION_ERROR,\
        INVALID_PATH,\
        CONTROLLER_ERRORS,\
        NO_ANALYZED_DATA,\
        UNKNOWN_ERROR,\
        LABVIEW_EXCEPTION,\
		INVALID_CONNECTION_DEVICE,\
		INVALID_SCAN_COUNT};

struct SVQM_800_Error
{
public:
	EnumErrorType m_ErrorType;
	std::wstring m_ErrorString;
	std::wstring m_ErrorTroubleShooting;

	SVQM_800_Error(EnumErrorType errorType = NO_ERRORS)
	{
		m_ErrorType = errorType;
	}

	SVQM_800_Error(EnumErrorType errorType, const std::wstring& errorString, const std::wstring& errorTroubleShooting)
	{
		m_ErrorType = errorType;
		m_ErrorString = errorString;
		m_ErrorTroubleShooting = errorTroubleShooting;
	}
};

struct IHeaderData
{
	friend struct DeviceWrapper;
public:
	double EmissionCurrent() const {
		return m_EmissionCurrent;
	}
	double FilamentBiasVoltage() const {
		return m_FilamentBiasVoltage;
	}
	double RepellerVoltage() const {
		return m_RepellerVoltage;
	}
	double EntryPlateVoltage() const {
		return m_EntryPlateVoltage;
	}
	double PressurePlateVoltage() const {
		return m_PressurePlateVoltage;
	}
	double CupsVoltage() const {
		return m_CupsVoltage;
	}
	double TransitionVoltage() const {
		return m_TransitionVoltage;
	}
	double ExitPlateVoltage() const {
		return m_ExitPlateVoltage;
	}
	double ElectronMultiplierShieldVoltage() const {
		return m_ElectronMultiplierShieldVoltage;
	}
	double ElectronMultiplierVoltage() const {
		return m_ElectronMultiplierVoltage;
	}
	double DDSAmplitude() const {
		return m_DDSAmplitude;
	}
	double ElectronMultiplierElectrometerGain() const {
		return m_ElectronMultiplierElectrometerGain;
	}
	double MassAxisCalibrationFactor() const {
		return m_MassAxisCalibrationFactor;
	}
	std::wstring FirmwareRevision() const {
		return m_FirmwareRevision;
	}
	std::wstring HardwareRevision() const {
		return m_HardwareRevision;
	}
	std::wstring MID() {
		return m_MID;
	}
private:
	double m_EmissionCurrent;
	double m_FilamentBiasVoltage;
	double m_RepellerVoltage;
	double m_EntryPlateVoltage;
	double m_PressurePlateVoltage;
	double m_CupsVoltage;
	double m_TransitionVoltage;
	double m_ExitPlateVoltage;
	double m_ElectronMultiplierShieldVoltage;
	double m_ElectronMultiplierVoltage;
	double m_DDSAmplitude;
	double m_ElectronMultiplierElectrometerGain;
	double m_MassAxisCalibrationFactor;
	std::wstring m_FirmwareRevision;
	std::wstring m_HardwareRevision;
	std::wstring m_MID;
};

struct SAnalyzedData;
struct SAverageData;

struct DeviceWrapper
{
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
	SVQM_800_Error GetGaugeState(EnumGaugeState& gaugeState, asynUser* IOUser);
private:
	size_t FindMarkerPos(std::string const& HeaderData, size_t Offset, const char* FirstMarker);
	size_t FindMarkerPos(std::string const& HeaderData, size_t Offset, const char* FirstMarker, const char* SecondMarker);
	void write(asynUser* IOUser, std::string const& WritePacket) const;
	void writeRead(asynUser* IOUser, std::string const& WritePacket, std::string& ReadPacket) const;
	void readTill(asynUser* IOUser, std::string& ReadPacket, std::string const& Termination) const;
	void DeviceWrapper::read(asynUser* IOUser, std::vector<char>& ReadPacket) const;
	std::string EnumToText(EnumLogicalInstruments logicalInstrumentEnum) const;
	SVQM_800_Error GetTSETingsValues(std::string const&  TSETingsValues);
	void ThrowException(asynUser* pasynUser, asynStatus Status, const char* Function, std::string const& what) const;
private:
	IHeaderData m_HeaderData;
	EnumGaugeState m_GaugeState;
	double m_lowerRange;
	double m_upperRange;
	double m_EmissionCurrent;
	std::map<EnumLogicalInstruments, InstrumentVoltage> m_InstrumentVoltages;
	mutable epicsMutex m_Mutex;
};
