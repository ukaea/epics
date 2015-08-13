call %~dp0..\..\..\iocBoot\iocLeyboldLEYBOLDTURBO\dllPath.bat

camonitor.exe ^
	LEYBOLDTURBO:1:Running ^
	LEYBOLDTURBO:1:FirmwareVersion ^
	LEYBOLDTURBO:1:Reset ^
	LEYBOLDTURBO:1:Fault ^
	LEYBOLDTURBO:1:FaultStr ^
	LEYBOLDTURBO:1:WarningTemperature ^
	LEYBOLDTURBO:1:WarningTemperatureStr ^
	LEYBOLDTURBO:1:WarningHighLoad ^
	LEYBOLDTURBO:1:WarningHighLoadStr ^
	LEYBOLDTURBO:1:WarningPurge ^
	LEYBOLDTURBO:1:WarningPurgeStr ^
	LEYBOLDTURBO:1:StatorFrequency ^
	LEYBOLDTURBO:1:ConverterTemperature ^
	LEYBOLDTURBO:1:MotorCurrent ^
	LEYBOLDTURBO:1:PumpTemperature ^
	LEYBOLDTURBO:1:CircuitVoltage ^
	2>camonitor1.err
