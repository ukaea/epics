REM	Module:
REM		JETLeyboldTurboApp.bat
REM
REM	Description:
REM		This is a Windows wrapper that provides the name and port mappings that are required
REM 	for the turbo pumps that are installed at JET.
REM 	This script is JET-specific.
REM
REM	Author:  Peter Heesterman (Tessella plc). Date: 03 Sep 2015.
REM	Written for CCFE (Culham Centre for Fusion Energy).
REM
REM	LeyboldTurbo is distributed subject to a Software License Agreement
REM	found in file LICENSE that is included with this distribution.


set BAUD=9600
set NoOfPZD=2

set COMPORT1=COM1
set COMPORT2=COM2
set COMPORT3=COM3
set COMPORT4=COM4
set COMPORT5=COM7
set COMPORT6=COM10

call JETMappings.bat

start ..\..\Release_LIB\LeyboldTurboApp.exe ..\..\iocBoot\iocLeyboldTurbo\st6.cmd
start ..\..\Release_LIB\LeyboldTurboGUI.exe

md d:\LocalData\
camonitor.py 1 %NUMPUMPS% D:\LocalData\
