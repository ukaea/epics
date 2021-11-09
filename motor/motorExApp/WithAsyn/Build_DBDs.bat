SET TOPLEVEL=%1
SET BUILDIR=%2
SET DLLDIR=%3

if not exist O.Common  md O.Common

set PERL5LIB=C:\Strawberry\lib\perl
if not exist C:\strawberry\perl\lib\DBD.pm  mklink /h C:\strawberry\perl\lib\DBD.pm %TOPLEVEL%lib\perl\DBD.pm
if not exist C:\strawberry\perl\lib\DBD  mklink /d C:\strawberry\perl\lib\DBD %TOPLEVEL%lib\perl\DBD
if not exist C:\strawberry\perl\lib\EPICS  mklink /d C:\strawberry\perl\lib\EPICS %TOPLEVEL%lib\perl\EPICS

perl -CSD %DLLDIR%dbdExpand.pl  -I. -I.. -I..\O.Common -I%TOPLEVEL%DBD  -o O.Common\WithAsyn.dbd base.dbd motorSupport.dbd devNewport.dbd LdevNewportTS.dbd devAerotech.dbd drvAsynSerialPort.dbd drvAsynIPPort.dbd busySupport.dbd
perl -CSD %DLLDIR%registerRecordDeviceDriver.pl  -I. -I./O.Common -I%TOPLEVEL%DBD -o  O.Common\WithAsyn_registerRecordDeviceDriver.cpp O.Common\WithAsyn.dbd WithAsyn_registerRecordDeviceDriver ..\..\..\

echo BatchCompleted: %ERRORLEVEL%