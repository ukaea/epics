call %~dp0..\..\..\iocBoot\iocLeyboldTurbo\dllPath.bat

camonitor.exe ^
	TURBO:1:Running TURBO:2:Running TURBO:3:Running TURBO:4:Running TURBO:5:Running ^
	TURBO:1:Reset TURBO:2:Reset TURBO:3:Reset TURBO:4:Reset TURBO:5:Reset ^
	TURBO:1:Fault TURBO:2:Fault TURBO:3:Fault TURBO:4:Fault TURBO:5:Fault ^
	TURBO:1:WarningTemperature TURBO:2:WarningTemperature TURBO:3:WarningTemperature TURBO:4:WarningTemperature TURBO:5:WarningTemperature ^
	TURBO:1:WarningHighLoad TURBO:2:WarningHighLoad TURBO:3:WarningHighLoad TURBO:4:WarningHighLoad TURBO:5:WarningHighLoad ^
	TURBO:1:StatorFrequency TURBO:2:StatorFrequency TURBO:3:StatorFrequency TURBO:4:StatorFrequency TURBO:5:StatorFrequency ^
	TURBO:1:ConverterTemperature TURBO:2:ConverterTemperature TURBO:3:ConverterTemperature TURBO:4:ConverterTemperature TURBO:5:ConverterTemperature ^
	TURBO:1:MotorCurrent TURBO:2:MotorCurrent TURBO:3:MotorCurrent TURBO:4:MotorCurrent TURBO:5:MotorCurrent ^
	TURBO:1:PumpTemperature TURBO:2:PumpTemperature TURBO:3:PumpTemperature TURBO:4:PumpTemperature TURBO:5:PumpTemperature ^
	TURBO:1:CircuitVoltage TURBO:2:CircuitVoltage TURBO:3:CircuitVoltage TURBO:4:CircuitVoltage TURBO:5:CircuitVoltage ^
	2>camonitor.err


