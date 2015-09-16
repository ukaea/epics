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


set NUMPUMPS=6
set BAUD=19200
set NoOfPZD=6

set COMPORT1=COM1
set COMPORT2=COM2
set COMPORT3=COM3
set COMPORT4=COM4
set COMPORT5=COM7
set COMPORT5=COM10

REM Cos we're using up-to-date asyn
set DB=LeyboldTurbo.Asyn4-26

call Scripts\JETMappings.bat

..\Release_LIB\LeyboldTurboApp ..\iocBoot\iocLeyboldTurbo\st6.cmd
