echo off
set EPICS_CA_SERVER_PORT=5071

set Pump=%1
if "%Pump%"=="" set Pump=1
set Run=%2
if "%Run%"=="" set Run=1

REM Some non-default values indicating hig pump stress...
caput TURBOSIM:%Pump%:StatorFrequency 200
caput TURBOSIM:%Pump%:StatorFrequency.PROC 1
caput TURBOSIM:%Pump%:ConverterTemperature 70
caput TURBOSIM:%Pump%:ConverterTemperature.PROC 1
caput TURBOSIM:%Pump%:MotorCurrent 15
caput TURBOSIM:%Pump%:MotorCurrent.PROC 1
caput TURBOSIM:%Pump%:CircuitVoltage 20
caput TURBOSIM:%Pump%:CircuitVoltage.PROC 1
caput TURBOSIM:%Pump%:WarningHighLoad 1
caput TURBOSIM:%Pump%:WarningHighLoad.PROC 1
timeout 10

caput TURBOSIM:%Pump%:PumpTemperature 80

caput TURBOSIM:%Pump%:WarningTemperature 1
caput TURBOSIM:%Pump%:WarningTemperature.PROC 1

timeout 10
caput TURBOSIM:%Pump%:Fault 1
caput TURBOSIM:%Pump%:Fault.PROC 1

