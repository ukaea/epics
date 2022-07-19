SET TOPLEVEL=%1
SET BUILDIR=%2
SET DLLDIR=%3

echo Symbol DLL build probs. TS 18 Jul 2022?
SET EPICS_BUILD_COMPILER_CLASS=cl

SET PERL5LIB=C:\Strawberry\lib\perl

if not exist C:\strawberry\perl\lib\DBD.pm  mklink /h C:\strawberry\perl\lib\DBD.pm %TOPLEVEL%lib\perl\DBD.pm
if not exist C:\strawberry\perl\lib\DBD  mklink /d C:\strawberry\perl\lib\DBD %TOPLEVEL%lib\perl\DBD
if not exist C:\strawberry\perl\lib\EPICS  mklink /d C:\strawberry\perl\lib\EPICS %TOPLEVEL%lib\perl\EPICS

perl -CSD %DLLDIR%\dbdExpand.pl  -I. -I.. -I../O.Common -I%TOPLEVEL%DBD -o O.Common\parker6ksupport.dbd base.dbd src/parker6k8.dbd asyn.dbd drvAsynIPPort.dbd drvAsynSerialPort.dbd motorSupport.dbd asSupport.dbd calcSupport.dbd 
perl -CSD %DLLDIR%\registerRecordDeviceDriver.pl O.Common\parker6k8.dbd parker6k8_registerRecordDeviceDriver %TOPLEVEL% >O.Common\parker6k8_registerRecordDeviceDriver.cpp 

echo BatchCompleted: %ERRORLEVEL%