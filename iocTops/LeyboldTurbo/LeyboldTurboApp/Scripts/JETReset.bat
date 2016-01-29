call JETMappings.txt

set PUMP=-1
if %1==%ASYNPORT1% set PUMP=1
if %1==%ASYNPORT2% set PUMP=2
if %1==%ASYNPORT3% set PUMP=3
if %1==%ASYNPORT4% set PUMP=4
if %1==%ASYNPORT5% set PUMP=5
if %1==%ASYNPORT6% set PUMP=6
if not %PUMP%==-1 goto OK

echo "Invalid pump %1"
goto EXIT

:OK
echo "Reseting pump %1 (%PUMP%)


Reset.py %PUMP% %PUMP%

:EXIT
