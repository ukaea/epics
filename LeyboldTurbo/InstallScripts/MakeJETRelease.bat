REM	Module:
REM		MakeJETRelease.bat
REM
REM	Description:
REM		This script copies the binary files required to deploy the application to CatsServer.
REM 	CatsServer is uniquely accessible from both on and off-line networks at JET.
REM 	This allows the binaries to be transferred from an office-based PC to a deployment PC.
REM
REM	Author:  Peter Heesterman (Tessella plc). Date: 25 Nov 2015.
REM	Written for CCFE (Culham Centre for Fusion Energy).
REM
REM	LeyboldTurbo is distributed subject to a Software License Agreement
REM	found in file LICENSE that is included with this distribution.

md \\catsserver.jetcats.jet.efda.org\CopyArea\LeyboldTurbo\Release
md \\catsserver.jetcats.jet.efda.org\CopyArea\LeyboldTurbo\Release\dbd
md \\catsserver.jetcats.jet.efda.org\CopyArea\LeyboldTurbo\Release\LeyboldSimApp
md \\catsserver.jetcats.jet.efda.org\CopyArea\LeyboldTurbo\Release\LeyboldSimApp\Db
md \\catsserver.jetcats.jet.efda.org\CopyArea\LeyboldTurbo\Release\LeyboldSimApp\TestScripts
md \\catsserver.jetcats.jet.efda.org\CopyArea\LeyboldTurbo\Release\LeyboldTurboApp
md \\catsserver.jetcats.jet.efda.org\CopyArea\LeyboldTurbo\Release\LeyboldTurboApp\Db
md \\catsserver.jetcats.jet.efda.org\CopyArea\LeyboldTurbo\Release\LeyboldTurboApp\Scripts
md \\catsserver.jetcats.jet.efda.org\CopyArea\LeyboldTurbo\Release\iocBoot
md \\catsserver.jetcats.jet.efda.org\CopyArea\LeyboldTurbo\Release\iocBoot\iocLeyboldTurbo
md \\catsserver.jetcats.jet.efda.org\CopyArea\LeyboldTurbo\Release\iocBoot\iocLeyboldSim
md \\catsserver.jetcats.jet.efda.org\CopyArea\LeyboldTurbo\Release\Release_LIB
md \\catsserver.jetcats.jet.efda.org\CopyArea\LeyboldTurbo\Release\Release_LIB\data


copy %~dp0..\dbd\LeyboldSim.dbd \\catsserver.jetcats.jet.efda.org\CopyArea\LeyboldTurbo\Release\dbd
copy %~dp0..\dbd\LeyboldTurbo.dbd \\catsserver.jetcats.jet.efda.org\CopyArea\LeyboldTurbo\Release\dbd

copy %~dp0..\LeyboldSimApp\Db\*.db \\catsserver.jetcats.jet.efda.org\CopyArea\LeyboldTurbo\Release\LeyboldSimApp\Db

copy %~dp0..\LeyboldSimApp\TestScripts\*.py \\catsserver.jetcats.jet.efda.org\CopyArea\LeyboldTurbo\Release\LeyboldSimApp\TestScripts
copy %~dp0..\LeyboldSimApp\TestScripts\*.bat \\catsserver.jetcats.jet.efda.org\CopyArea\LeyboldTurbo\Release\LeyboldSimApp\TestScripts

copy %~dp0..\LeyboldTurboApp\Db\*.db \\catsserver.jetcats.jet.efda.org\CopyArea\LeyboldTurbo\Release\LeyboldTurboApp\Db
copy %~dp0..\LeyboldTurboApp\Scripts\*.py \\catsserver.jetcats.jet.efda.org\CopyArea\LeyboldTurbo\Release\LeyboldTurboApp\Scripts
copy %~dp0..\LeyboldTurboApp\Scripts\*.bat \\catsserver.jetcats.jet.efda.org\CopyArea\LeyboldTurbo\Release\LeyboldTurboApp\Scripts
copy %~dp0..\LeyboldTurboApp\version.txt \\catsserver.jetcats.jet.efda.org\CopyArea\LeyboldTurbo\Release\LeyboldTurboApp

copy %~dp0..\iocBoot\iocLeyboldSim\*.cmd \\catsserver.jetcats.jet.efda.org\CopyArea\LeyboldTurbo\Release\iocBoot\iocLeyboldSim
copy %~dp0..\iocBoot\iocLeyboldTurbo\*.cmd \\catsserver.jetcats.jet.efda.org\CopyArea\LeyboldTurbo\Release\iocBoot\iocLeyboldTurbo

copy %~dp0..\Release_LIB\LeyboldSim.exe \\catsserver.jetcats.jet.efda.org\CopyArea\LeyboldTurbo\Release\Release_LIB
copy %~dp0..\Release_LIB\LeyboldTurboApp.exe \\catsserver.jetcats.jet.efda.org\CopyArea\LeyboldTurbo\Release\Release_LIB
copy %~dp0..\Release_LIB\LeyboldTurboGUI.exe \\catsserver.jetcats.jet.efda.org\CopyArea\LeyboldTurbo\Release\Release_LIB
copy %~dp0..\Release_LIB\data\LeyboldTurboGUI.exe \\catsserver.jetcats.jet.efda.org\CopyArea\LeyboldTurbo\Release\Release_LIB
copy "C:\Program Files (x86)\National Instruments\LabVIEW 2014\user.lib\caLab\lib\*.*" \\catsserver.jetcats.jet.efda.org\CopyArea\LeyboldTurbo\Release\Release_LIB\data
