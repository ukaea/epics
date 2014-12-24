set Run=%1
set Pump=%2
if "%Run%"=="" set Run=1
if "%Pump%"=="" set Pump=1

caput TURBO:%Pump%:Running %Run%
caput TURBO:%Pump%:Running.PROC 1

