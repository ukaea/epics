export EPICS_CA_SERVER_PORT=5071

camonitor \
	TURBOSIM:1:Running TURBOSIM:1:Fault TURBOSIM:1:WarningTemperature TURBOSIM:1:WarningHighLoad TURBOSIM:1:Fault TURBOSIM:1:StatorFrequency TURBOSIM:1:ConverterTemperature TURBOSIM:1:MotorCurrent TURBOSIM:1:PumpTemperature TURBOSIM:1:CircuitVoltage \
