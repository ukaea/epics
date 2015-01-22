call %~dp0..\..\..\iocBoot\iocLeyboldTurbo\dllPath.bat

set Pump=%1
set Run=%2

if "%Pump%"=="" set Pump=1
if "%Run%"=="" set Run=1

caput TURBO:%Pump%:Running %Run%
caput TURBO:%Pump%:Running.PROC 1

