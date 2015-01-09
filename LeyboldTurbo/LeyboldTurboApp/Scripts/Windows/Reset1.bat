set Pump=%1
if "%Pump%"=="" set Pump=1

caput TURBO:%Pump%:Reset 1

