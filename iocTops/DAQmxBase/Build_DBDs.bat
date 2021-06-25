SET TOPLEVEL=%1
SET BUILDIR=%2
SET DLLDIR=%3

if not exist O.Common md O.Common
if not exist %TOPLEVEL%\DBD md %TOPLEVEL%\DBD

SET PERL5LIB=C:\Strawberrylib\perl
if not exist C:\strawberry\perl\lib\DBD.pm  mklink /h C:\strawberry\perl\lib\DBD.pm %TOPLEVEL%lib\perl\DBD.pm
if not exist C:\strawberry\perl\lib\DBD  mklink /d C:\strawberry\perl\lib\DBD %TOPLEVEL%lib\perl\DBD
if not exist C:\strawberry\perl\lib\EPICS  mklink /d C:\strawberry\perl\lib\EPICS %TOPLEVEL%lib\perl\EPICS

perl -CSD %DLLDIR%\dbdExpand.pl  -I. -I.. -I../O.Common -I%TOPLEVEL%\dbd  -o O.Common\DAQmxBase.dbd base.dbd src/DAQmxBaseSupport.dbd asyn.dbd
perl -CSD %DLLDIR%\registerRecordDeviceDriver.pl O.Common\DAQmxBase.dbd DAQmxBase_registerRecordDeviceDriver %TOPLEVEL% >O.Common\DAQmxBase_registerRecordDeviceDriver.cpp

echo BatchCompleted: %ERRORLEVEL%