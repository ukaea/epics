call %~dp0..\..\..\iocBoot\iocLeyboldTurbo\dllPath.bat

set Pump=%1
if "%Pump%"=="" set Pump=1

caput TURBO:%Pump%:Reset 1

