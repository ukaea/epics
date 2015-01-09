set EPICS_CA_SERVER_PORT=5071

camonitor.exe ^
	TURBOSIM:1:Running TURBOSIM:2:Running TURBOSIM:3:Running TURBOSIM:4:Running TURBOSIM:5:Running ^
	TURBOSIM:1:Fault TURBOSIM:2:Fault TURBOSIM:3:Fault TURBOSIM:4:Fault TURBOSIM:5:Fault ^
	TURBOSIM:1:WarningTemperature TURBOSIM:2:WarningTemperature TURBOSIM:3:WarningTemperature TURBOSIM:4:WarningTemperature TURBOSIM:5:WarningTemperature ^
	TURBOSIM:1:WarningHighLoad TURBOSIM:2:WarningHighLoad TURBOSIM:3:WarningHighLoad TURBOSIM:4:WarningHighLoad TURBOSIM:5:WarningHighLoad ^
	TURBOSIM:1:StatorFrequency TURBOSIM:2:StatorFrequency TURBOSIM:3:StatorFrequency TURBOSIM:4:StatorFrequency TURBOSIM:5:StatorFrequency ^
	TURBOSIM:1:ConverterTemperature TURBOSIM:2:ConverterTemperature TURBOSIM:3:ConverterTemperature TURBOSIM:4:ConverterTemperature TURBOSIM:5:ConverterTemperature ^
	TURBOSIM:1:MotorCurrent TURBOSIM:2:MotorCurrent TURBOSIM:3:MotorCurrent TURBOSIM:4:MotorCurrent TURBOSIM:5:MotorCurrent ^
	TURBOSIM:1:PumpTemperature TURBOSIM:2:PumpTemperature TURBOSIM:3:PumpTemperature TURBOSIM:4:PumpTemperature TURBOSIM:5:PumpTemperature ^
	TURBOSIM:1:CircuitVoltage TURBOSIM:2:CircuitVoltage TURBOSIM:3:CircuitVoltage TURBOSIM:4:CircuitVoltage TURBOSIM:5:CircuitVoltage ^
	2>turbosim.err



