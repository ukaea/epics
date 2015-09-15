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


set ASYNPORT=TT
set ASYNSIMPORT=TTSIM

set ASYNPORT1=TT:01
set ASYNSIMPORT1=TTSIM:01

set ASYNPORT2=TT:02
set ASYNSIMPORT2=TTSIM:02

set ASYNPORT3=TT:03
set ASYNSIMPORT3=TPSIM:03

set ASYNPORT4=TT:04
set ASYNSIMPORT4=TPSIM:04

set ASYNPORT5=NT:07
set ASYNSIMPORT5=NTSIM:07

set ASYNPORT6=NT:07
set ASYNSIMPORT6=NTSIM:07

TestSequence.bat 6 2
