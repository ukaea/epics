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


set ASYNSIMPORT=TTSIM
set ASYNSIMPORT1=TTSIM:01
set ASYNSIMPORT2=TTSIM:02
set ASYNSIMPORT3=TPSIM:03
set ASYNSIMPORT4=TPSIM:04
set ASYNSIMPORT5=NTSIM:07
set ASYNSIMPORT6=NTSIM:10
set NUMPUMPS=6

call ..\..\LeyboldTurboApp\Scripts\JETMappings.bat

start ..\..\Release_LIB\LeyboldSim.exe ..\..\iocBoot\iocLeyboldSim\st6.simulator.cmd

start ..\..\Release_LIB\LeyboldTurboApp.exe ..\..\iocBoot\iocLeyboldTurbo\st6.simulated.cmd

start python camonitor.py 1 %NUMPUMPS%
start python ..\..\LeyboldTurboApp\Scripts\camonitor.py 1 %NUMPUMPS%

testsequence.py %NUMPUMPS%

REM timeout 5
REM start /WAIT ..\..\LeyboldTurboApp\Scripts\Reset.py 1 %NUMPUMPS%

REM timeout 5
REM start /WAIT ..\..\LeyboldTurboApp\Scripts\SetRunning.py 1 %NUMPUMPS% 1
