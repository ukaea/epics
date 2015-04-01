call %~dp0..\..\..\iocBoot\iocLeyboldTurbo\dllPath.bat

camonitor.exe ^
	TURBO:1:Running ^
	TURBO:1:FirmwareVersion ^
	TURBO:1:Reset ^
	TURBO:1:Fault ^
	TURBO:1:FaultStr ^
	TURBO:1:WarningTemperature ^
	TURBO:1:WarningTemperatureStr ^
	TURBO:1:WarningHighLoad ^
	TURBO:1:WarningHighLoadStr ^
	TURBO:1:WarningPurge ^
	TURBO:1:WarningPurgeStr ^
	TURBO:1:StatorFrequency ^
	TURBO:1:ConverterTemperature ^
	TURBO:1:MotorCurrent ^
	TURBO:1:PumpTemperature ^
	TURBO:1:CircuitVoltage ^
	2>camonitor1.err
