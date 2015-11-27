REM	Module:
REM		DebugSettings.bat
REM
REM	Description:
REM		This script makes environment variable settings that are required in order to execute the
REM 	simulator for debugging purpose.
REM
REM	Author:  Peter Heesterman (Tessella plc). Date: 25 Nov 2015.
REM	Written for CCFE (Culham Centre for Fusion Energy).
REM
REM	LeyboldTurbo is distributed subject to a Software License Agreement
REM	found in file LICENSE that is included with this distribution.

call ..\..\..\base\startup\win32.bat
set EPICS_CA_SERVER_PORT=5072
set EPICS_CA_AUTO_ADDR_LIST=NO
set EPICS_CA_ADDR_LIST=localhost
