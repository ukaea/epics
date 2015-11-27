REM	Module:
REM		JETcamonitor.bat
REM
REM	Description:
REM		This is a Windows script that provides the name mappings - and file path - that are required
REM 	for the turbo pumps that are installed at JET.
REM 	This script is JET-specific.
REM
REM	Author:  Peter Heesterman (Tessella plc). Date: 03 Sep 2015.
REM	Written for CCFE (Culham Centre for Fusion Energy).
REM
REM	LeyboldTurbo is distributed subject to a Software License Agreement
REM	found in file LICENSE that is included with this distribution.


md D:\LocalData\

call JETMappings.bat

camonitor.py 1 6 D:\LocalData\
