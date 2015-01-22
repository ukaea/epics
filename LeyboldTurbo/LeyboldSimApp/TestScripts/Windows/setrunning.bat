call %~dp0..\..\..\iocBoot\iocLeyboldTurbo\dllPath.bat

set EPICS_CA_SERVER_PORT=5071

set Pump=%1
if "%Pump%"=="" set Pump=1
set Run=%2
if "%Run%"=="" set Run=1

caput TURBOSIM:%Pump%:Running %Run%
caput TURBOSIM:%Pump%:Running.PROC 1

