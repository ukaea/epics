REM	Module:
REM		testsequence.bat
REM
REM	Description:
REM		This is a Windows wrapper that invokes a series of seperate test scripts in order to 
REM 	excercise a test sequence.
REM 	Most of the individual test scripts are Python and Pyepics based.
REM
REM	Author:  Peter Heesterman (Tessella plc). Date: 03 Sep 2015.
REM	Written for CCFE (Culham Centre for Fusion Energy).
REM
REM	LeyboldTurbo is distributed subject to a Software License Agreement
REM	found in file LICENSE that is included with this distribution.


set NUMPUMPS=1 
if not "%1"=="" set NUMPUMPS=%1
echo "Number of pumps " %NUMPUMPS%

set NOOFPZD=6
if not "%2"=="" set NOOFPZD=%2
echo "Number of pzd " %NOOFPZD%

start cmd /C "LeyboldSim.py %NUMPUMPS% %NOOFPZD%"

start cmd /C "..\..\LeyboldTurboApp\Scripts\LeyboldTurboApp.py %NUMPUMPS% %NOOFPZD% Sim"

start python camonitor.py 1 %NUMPUMPS%
start python ..\..\LeyboldTurboApp\Scripts\camonitor.py 1 %NUMPUMPS%

testsequence.py %NUMPUMPS%

timeout 5
start /WAIT ..\..\LeyboldTurboApp\Scripts\Reset.py 1 %NUMPUMPS%

timeout 5
start /WAIT ..\..\LeyboldTurboApp\Scripts\SetRunning.py 1 %NUMPUMPS% 1

