#pragma once

#include <string>
#include <windows.h>

#define SYMBOL_DECLSPEC __declspec(dllexport)

////////////////////////////////////////////////////////////////////////////////
// #defines

#ifdef _UNICODE
#define STRING_TYPE std::wstring
#define CHAR_TYPE wchar_t
#else
#define STRING_TYPE std::string
#define CHAR_TYPE char
#endif

////////////////////////////////////////////////////////////////////////////////
// Enumerations

//Describes connection status of the service, to the hardware controller.
enum EConnectionType { UNCONNECTED, VQM_830, VQM_835 };
enum EAvailabilityType { AVAILABLE, IN_USE, UNAVAILABLE };
enum EServiceConnectionType { LOCALHOST, TCP };

//Maps to the list of logical instruments.
enum EnumLogicalInstruments { FILAMENT = 0, FILAMENTBIAS, REPELLERBIAS, ENTRYPLATE, PRESSUREPLATE, CUPS, TRANSITION, EXITPLATE, EMSHIELD, EMBIAS, RFAMP };

//Describes the status of the service.
enum EnumServiceStatus { UNKNOWN, INITIALIZING, FETCHING, LOGGING, AUTOTUNING };

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

//Data Analysis
enum EnumAvgMode { Off = 0, Running_Avg = 1, Cumulative_Moving_Avg = 2, Accumulator = 3 };
enum EnumPressureSource { EnumPressureSource_None = 0, _390_TPMK, _307_350_01mA, _307_350_1mA, _307_350_10mA, _354015, _354019_354075, _360_370, _358 };
enum EnumLoggingOptions { EnumLoggingOptions_None = 0, EnumLoggingOptions_Averaging, EnumLoggingOptions_Analysis, EnumLoggingOptions_Timing, EnumLoggingOptions_All };
enum EnumThresholdRule { EnumThresholdRule_HYBRID = 0, EnumThresholdRule_MINMAX, EnumThresholdRule_SURE, EnumThresholdRule_UNIVERSAL };

enum EnumGaugeState { EnumGaugeState_OFF = 0, EnumGaugeState_STANDBY, EnumGaugeState_ON, EnumGaugeState_SCAN, EnumGaugeState_EXTERNAL_TRIGGER_SCANNING, EnumGaugeState_EXTERNAL_TRIGGER_OFF };

//Autotune
enum EnumAutotuneStatus { AutotuneUnavailable = 0, AutotuneIdle, AutotuneBusy, AutotunePassed, AutotuneExitPlateOutOfRange, \
        AutotuneElectronMultiplierShieldOutOfRange, AutotuneElectronMultiplierBiasOutOfRange, \
        AutotuneVacuumSystemUnstable, AutotuneRepellerOutOfRange, AutoTuneFailedToStart, AutoTuneRFAmpOutOfRange };

////////////////////////////////////////////////////////////////////////////////

/*
  Encapsulate all the parameters required to connect to a device.
 */
struct SDeviceConnectionInfo
{
public:
	EConnectionType m_DeviceType;
	STRING_TYPE m_DeviceAddress;
	EAvailabilityType m_Availability;
	STRING_TYPE m_SerialNumber;

	SDeviceConnectionInfo()
	{
		m_DeviceType = UNCONNECTED;
		m_Availability = UNAVAILABLE;
	}
};

/*
 Forwards
 */
struct IHeaderData;
struct SVQM_800_Error;
struct SAnalyzedData;
struct SAverageData;
struct SComboFileData;
struct SRawConnectionOptions;

/*
 Main ServiceWrapper interface. Instances are obtained via CreateServiceHandle.
 */
struct IServiceWrapper
{
public:	
	/*
	 Return the connections available based on the connection type.
	 The caller is responsible for deleting the array "buffer" that is returned.
	 */
	virtual SVQM_800_Error GetValidConnections(int& count, SDeviceConnectionInfo*& buffer, EServiceConnectionType serviceConnectionType = LOCALHOST, const char* serviceConnection = NULL) = 0;
	/*
	 Initiates scanning.
	 */
	virtual SVQM_800_Error StartScan(const SDeviceConnectionInfo& connectInfo) = 0;
	/*
	 Start raw data logging.
	 */
	virtual SVQM_800_Error StartLogging(const SDeviceConnectionInfo& connectInfo, STRING_TYPE* logPath = NULL) = 0;
	/*
	 Stop raw data logging. Scanning is not disrupted.
	 */
	virtual SVQM_800_Error StopLogging(const SDeviceConnectionInfo& connectInfo) = 0;
	/*
	 Stop scanning. Logging, if enabled, is stopped.
	 */
    virtual SVQM_800_Error StopScan(const SDeviceConnectionInfo& connectInfo) = 0;
	/*
	 Opens a connection to a selected Controller.
	 */
	virtual SVQM_800_Error ConnectToDevice(const SDeviceConnectionInfo& connectInfo, bool& isMaster) = 0;
	/*
	 Closes an existing connection to a Controller.
	 */
	virtual SVQM_800_Error DisconnectFromDevice(const SDeviceConnectionInfo& connectInfo) = 0;
	/*
	 Return the scan data. The caller is responsible for deleting IHeaderData interface.
	 */
	virtual SVQM_800_Error GetScanData(int& lastScanNumber, SAnalyzedData& analyzedData, IHeaderData** headerDataPtr,
                                            SAverageData& averageData, const SDeviceConnectionInfo& connectInfo, bool& isValidData, EnumGaugeState& controllerState) = 0;
	/*
	 Return the header data. The caller is responsible for deleting IHeaderData.
	 */
	virtual SVQM_800_Error GetHeaderData(IHeaderData** headerDataPtr, const SDeviceConnectionInfo& connectInfo) = 0;
	/*
	 Send SCPI command(s) to the Controller. A batch of commands can be sent using ";:" as a delimiter between each SCPI command.
	 */
	virtual STRING_TYPE SendInstruction(const CHAR_TYPE* instr, bool requireResponse, const SDeviceConnectionInfo& connectInfo) = 0;
	/*
	 Return the External Total Pressure.
	 */
	virtual SVQM_800_Error GetExternalTotalPressure(double& value, const SDeviceConnectionInfo& connectInfo) const = 0;
	/*
	 Return the current voltage value of the logical instrument.
	 */
	virtual SVQM_800_Error GetLogicalInstrumentCurrentVoltage(double& value, EnumLogicalInstruments logicalInstrumentEnum, const SDeviceConnectionInfo& connectInfo) const = 0;
	/*
	 Return the maximum and minimum voltage values of the logical instrument.
	 */
	virtual SVQM_800_Error GetLogicalInstrumentMinMaxVoltage(double& min, double& max, EnumLogicalInstruments logicalInstrumentEnum, const SDeviceConnectionInfo& connectInfo) const = 0;
	/*
	 Return/Set the voltage setpoint value of the logical instrument.
	 */
	virtual SVQM_800_Error GetLogicalInstrumentVoltageSetpoint(double& value, EnumLogicalInstruments logicalInstrumentEnum, const SDeviceConnectionInfo& connectInfo) const = 0;
	virtual SVQM_800_Error SetLogicalInstrumentVoltageSetpoint(EnumLogicalInstruments logicalInstrumentEnum, double& value, const SDeviceConnectionInfo& connectInfo) = 0;
	/*
	 Return the Filament Emission Current.
	 */
	virtual SVQM_800_Error GetFilamentEmissionCurrent(double& value, const SDeviceConnectionInfo& connectInfo) const = 0;
	/*
	 Return/Set the Filament Emission Current setpoint.
	 */
	virtual SVQM_800_Error GetFilamentEmissionCurrentSetpoint(double& value, const SDeviceConnectionInfo& connectInfo) const = 0;
	virtual SVQM_800_Error SetFilamentEmissionCurrentSetpoint(double& value, const SDeviceConnectionInfo& connectInfo) = 0;
	/*
	 Return the Filament Emission Current Min and Max settings.
	 */
	virtual SVQM_800_Error GetFilamentEmissionCurrentMinMax(double& minValue, double& outValue, const SDeviceConnectionInfo& connectInfo) const = 0;
	/* 
	 Return the Analog Input Voltage.
	 */
	virtual SVQM_800_Error GetAnalogInputVoltage(double& value, const SDeviceConnectionInfo& connectInfo) const = 0;
	/*
	 Store the current setpoints to the EEPROM.
	 */
	virtual SVQM_800_Error SaveUserSettingsToEEPROM(const SDeviceConnectionInfo& connectInfo) = 0;
	/*
	 Restore factory defaults from EEPROM.
	 */
	virtual SVQM_800_Error RestoreFactoryDefaults(const SDeviceConnectionInfo& connectInfo) = 0;
	/*
	 Restore user settings from EEPROM.
	 */
	virtual SVQM_800_Error RestoreUserSettings(const SDeviceConnectionInfo& connectInfo) = 0;
	/*
	 Return/Set the Mass Calibration Factor.
	 */
	virtual SVQM_800_Error GetMassCalibrationFactor(float& value, const SDeviceConnectionInfo& connectInfo) const = 0;	
	virtual SVQM_800_Error SetMassCalibrationFactor(float value, const SDeviceConnectionInfo& connectInfo) = 0;
	/*
	 Return the Mass Calibration Factor Min and Max settings.
	 */
	virtual SVQM_800_Error GetMinMaxMassCalibrationFactor(double& minValue, double& maxValue, const SDeviceConnectionInfo& connectInfo) = 0;
	/*
	 Return the Electrometer Gain.
	 */
	virtual SVQM_800_Error GetElectrometerGain(double& value, const SDeviceConnectionInfo& connectInfo) const = 0;
	/*
	 Return the Electrometer Gain Min and Max settings.
	 */
	virtual SVQM_800_Error GetElectrometerGainMinMax(double& minValue, double& maxValue, const SDeviceConnectionInfo& connectInfo) const = 0;
	/*
	 Return/Set the Electrometer Gain.
	 */
	virtual SVQM_800_Error GetElectrometerGainSetpoint(double& value, const SDeviceConnectionInfo& connectInfo) const = 0;
	virtual SVQM_800_Error SetElectrometerGainSetpoint(double value, const SDeviceConnectionInfo& connectInfo) = 0;
	/*
	 Return the Controller's Error Count and Queue. 
	 The caller is responsible for deleting the array "errorQueue" that is returned.
	 If errorCount is 0, errorQueue is NULL.
	*/
	virtual SVQM_800_Error GetErrorCountAndQueue(int& errorCount, STRING_TYPE* errorQueue, const SDeviceConnectionInfo& connectInfo) const = 0;
	/*
	 Return/Set the path (absolute) where the raw log files will be created.
	 */
	virtual SVQM_800_Error GetLogFileFolderPath(STRING_TYPE& folder, const SDeviceConnectionInfo& connectInfo) const = 0;
	virtual SVQM_800_Error SetLogFileFolderPath(const STRING_TYPE& folder, const SDeviceConnectionInfo& connectInfo) = 0;
	/*
	 Return/Set the scan count to log in each log file.
	 */
	virtual SVQM_800_Error GetLogFileScanCount(int& value, const SDeviceConnectionInfo& connectInfo) const = 0;
	virtual SVQM_800_Error SetLogFileScanCount(int scanCount, const SDeviceConnectionInfo& connectInfo) = 0;
	/*
	 Turn on or off Gauge.
	 */
	virtual SVQM_800_Error TurnOnGauge(const SDeviceConnectionInfo& connectInfo) = 0;
	virtual SVQM_800_Error TurnOffGauge(const SDeviceConnectionInfo& connectInfo) = 0;        
	/*
	 Data Analysis functions.
	 */
	virtual SVQM_800_Error DataAnalysisClearAvgHistory(const SDeviceConnectionInfo& connectInfo) = 0;
	virtual SVQM_800_Error DataAnalysisSetAvgMode(EnumAvgMode mode, const SDeviceConnectionInfo& connectInfo) = 0;
	virtual SVQM_800_Error DataAnalysisSetNumAvgs(int numAverages, const SDeviceConnectionInfo& connectInfo) = 0;
	virtual SVQM_800_Error DataAnalysisSetPressureSource(EnumPressureSource source, const SDeviceConnectionInfo& connectInfo) = 0;
	virtual SVQM_800_Error DataAnalysisEnableLogging(EnumLoggingOptions loggingOption, const SDeviceConnectionInfo& connectInfo) = 0;
	virtual SVQM_800_Error DataAnalysisSetHighMassIndex(int highMass, const SDeviceConnectionInfo& connectInfo) = 0;
	/*
	 Get the combo file data that is used for gas fitting for this connection.
	 */
	virtual SVQM_800_Error GetComboFileData(SComboFileData* dataArray, const SDeviceConnectionInfo& connectInfo) = 0;
	/*
	 Autotune functions.
	 */
	virtual SVQM_800_Error StartAutotune(const CHAR_TYPE* logFileName, const SDeviceConnectionInfo& connectInfo, bool normalMode = true) = 0;
	virtual SVQM_800_Error CancelAutotune(const SDeviceConnectionInfo& connectInfo) = 0;
	virtual SVQM_800_Error GetAutotuneStatus(EnumAutotuneStatus& status, int& percentDone, double& emBias, double& repeller, double& exitPlate, double& shield, double& rfamp, const SDeviceConnectionInfo& connectInfo) = 0;
	/*
	 Set the scan range. The resultant range values can be different and are returned from this function.
	 */
	virtual SVQM_800_Error SetScanRange(double& lowerRange, double& upperRange, const SDeviceConnectionInfo& connectInfo) = 0;
	/*
	 Get the current scan range.
	 */
	virtual SVQM_800_Error GetScanRange(double& lowerRange, double& upperRange, const SDeviceConnectionInfo& connectInfo) = 0;
	/*
	 Return/Set the gauge state.
	 */
	virtual SVQM_800_Error SetGaugeState(EnumGaugeState gaugeState, const SDeviceConnectionInfo& connectInfo) = 0;
	virtual SVQM_800_Error GetGaugeState(EnumGaugeState& gaugeState, const SDeviceConnectionInfo& connectInfo) = 0;
	/*
	 Set the state of the Digital Out to true or false.
	 */
	virtual SVQM_800_Error SetDigitalOut(bool state, const SDeviceConnectionInfo& connectInfo) = 0;
	/*
	 Return the current state of the Digital Out.
	 */
	virtual SVQM_800_Error GetDigitalOut(bool& state, const SDeviceConnectionInfo& connectInfo) = 0;
	/*
	 Return if Controller has extended range capabilities.
	 */
	virtual SVQM_800_Error GetExtendedRangeCapabilities(bool& isExtendedRange, const SDeviceConnectionInfo& connectInfo) = 0;
	/*
	 Initiate a connection to raw data.
	 */
	virtual SVQM_800_Error ConnectToRawData(const char* fullPath, const SDeviceConnectionInfo& connectInfo, SRawConnectionOptions* options = NULL) = 0;
	
	/*
	 Dispose of this instance when it is no longer needed, to prevent any memory leaks.
	 */
	virtual void Release() = 0;	
};
 
// Handle type.
typedef IServiceWrapper* SERVICEHANDLE;

////////////////////////////////////////////////////////////////////////////////
// 
#define EXTERN_C     extern "C"

namespace CLIWrapperDLL
{

////////////////////////////////////////////////////////////////////////////////
// C Factory function to create instances of the IServiceWrapper interface.
EXTERN_C
SYMBOL_DECLSPEC
SERVICEHANDLE
WINAPI
CreateServiceHandle(VOID);

/////////////////////////////////////////////////////////////////////////////////
// C Factory function to dispose of instances of the IServiceWrapper interface.
EXTERN_C
SYMBOL_DECLSPEC
VOID
APIENTRY
ReleaseServiceHandle(SERVICEHANDLE handle);

}
////////////////////////////////////////////////////////////////////////////////
