set EPICS_CA_SERVER_PORT=5071

set Run=%1
set Pump=%2
if "%Run%"=="" set Run=1
if "%Pump%"=="" set Pump=1

caput TURBOSIM:%Pump%:Running %Run%
caput TURBOSIM:%Pump%:Running.PROC 1

