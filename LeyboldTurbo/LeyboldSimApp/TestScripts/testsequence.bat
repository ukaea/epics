set NUMPUMPS=1 
if not "%1"=="" set NUMPUMPS=%1
echo "Number of pumps " %NUMPUMPS%

set NOOFPZD=6
if not "%2"=="" set NOOFPZD=%1
echo "Number of pzd " %NOOFPZD%

start ..\LeyboldSim.bat %NUMPUMPS% %NOOFPZD%

start ..\..\LeyboldTurboApp\LeyboldTurbo.bat %NUMPUMPS% %NOOFPZD% Sim

start camonitor.py %NUMPUMPS%
start ..\..\LeyboldTurboApp\Scripts\camonitor.py %NUMPUMPS%

testsequence.py %NUMPUMPS%

timeout 10
..\..\LeyboldTurboApp\Scripts\Reset.py 1 %NUMPUMPS%

timeout 10
..\..\LeyboldTurboApp\Scripts\SetRunning.py 1 %NUMPUMPS% 1

