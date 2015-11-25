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


REM These are the name mappings that are used by the simulator IOC in further use of thius script.
set ASYNSIMPORT=TTSIM
set ASYNSIMPORT1=TTSIM:01
set ASYNSIMPORT2=TTSIM:02
set ASYNSIMPORT3=TTSIM:03
set ASYNSIMPORT4=TTSIM:04
set ASYNSIMPORT5=NTSIM:07
set ASYNSIMPORT6=NTSIM:10

REM Creates the name mappings that are used for the 'real' IOC.
REM Thes reflect the names that are actually used for the JET pumps.
call ..\..\LeyboldTurboApp\Scripts\JETMappings.bat

REM This is the LabVIEW - written GUI.
start ..\..\Release_LIB\LeyboldTurboGUI.exe

REM This is the simulator
start ..\..\Release_LIB\LeyboldSim.exe ..\..\iocBoot\iocLeyboldSim\st6.simulator.cmd

REM This is the 'real' pump application; it will connect with the simulator above.
start ..\..\Release_LIB\LeyboldTurboApp.exe ..\..\iocBoot\iocLeyboldTurbo\st6.simulated.cmd

REM This script logs simulator values to D:\LocalData (by default).
start python camonitor.py 1 %NUMPUMPS%

REM This script logs 'real' application data values to D:\LocalData (by default).
start python ..\..\LeyboldTurboApp\Scripts\camonitor.py 1 %NUMPUMPS%

REM This script excercises the simulator through a sequence of on/off, value changes, 
REM and fault states for each of the 6 pumps.
testsequence.py %NUMPUMPS%

REM timeout 5
REM This script excercises a reset on the pump fault state.
REM But it is superceded by use of the GUI.
REM start /WAIT ..\..\LeyboldTurboApp\Scripts\Reset.py 1 %NUMPUMPS%

REM timeout 5
REM This script excercises a restart on the pump.
REM But it is superceded by use of the GUI.
REM start /WAIT ..\..\LeyboldTurboApp\Scripts\SetRunning.py 1 %NUMPUMPS% 1
