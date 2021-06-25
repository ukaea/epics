SET TOPLEVEL=C:\Users\ktn98257\source\repos\epics.dotnet\
SET BUILDIR=C:\Users\ktn98257\source\repos\epics.dotnet\motor\motorExApp\WithAsyn
SET DLLDIR=C:\Users\ktn98257\source\repos\epics.dotnet\Debug_DLL\

if not exist O.Common  md O.Common

set PERL5LIB=C:\Strawberry\lib\perl
if not exist C:\strawberry\perl\lib\DBD.pm  mklink /h C:\strawberry\perl\lib\DBD.pm %TOPLEVEL%lib\perl\DBD.pm
if not exist C:\strawberry\perl\lib\DBD  mklink /d C:\strawberry\perl\lib\DBD %TOPLEVEL%lib\perl\DBD
if not exist C:\strawberry\perl\lib\EPICS  mklink /d C:\strawberry\perl\lib\EPICS %TOPLEVEL%lib\perl\EPICS

perl -CSD %DLLDIR%\dbdExpand.pl  -I. -I.. -I../O.Common -I%TOPLEVEL%\dbd  -o O.Common\WithAsyn.dbd base.dbd motorSupport.dbd devNewport.dbd LdevNewportTS.dbd devAerotech.dbd drvAsynSerialPort.dbd drvAsynIPPort.dbd busySupport.dbd
perl -CSD %DLLDIR%\registerRecordDeviceDriver.pl  -I. -I./O.Common -I%TOPLEVEL%\dbd -o  O.Common\WithAsyn_registerRecordDeviceDriver.cpp O.Common/WithAsyn.dbd WithAsyn_registerRecordDeviceDriver ..\..\..\

echo BatchCompleted: %ERRORLEVEL%