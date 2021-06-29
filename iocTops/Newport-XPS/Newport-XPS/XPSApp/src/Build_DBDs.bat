SET TOPLEVEL=%1
SET BUILDIR=%2
SET DLLDIR=%3

if not exist O.Common md O.Common
if not exist %TOPLEVEL%\DBD md %TOPLEVEL%\DBD

SET PERL5LIB=C:\Strawberry\lib\perl
if not exist C:\strawberry\perl\lib\DBD.pm  mklink /h C:\strawberry\perl\lib\DBD.pm %TOPLEVEL%lib\perl\DBD.pm
if not exist C:\strawberry\perl\lib\DBD  mklink /d C:\strawberry\perl\lib\DBD %TOPLEVEL%lib\perl\DBD
if not exist C:\strawberry\perl\lib\EPICS  mklink /d C:\strawberry\perl\lib\EPICS %TOPLEVEL%lib\perl\EPICS

perl -CSD %DLLDIR%\dbdExpand.pl  -I. -I.. -I../O.Common -I%TOPLEVEL%\DBD  -o O.Common\Newport-XPS.dbd base.dbd asyn.dbd motorSupport.dbd devNewport.dbd busySupport.dbd sscan.dbd asSupport.dbd
perl -CSD %DLLDIR%\registerRecordDeviceDriver.pl O.Common\Newport-XPS.dbd Newport-XPS_registerRecordDeviceDriver %TOPLEVEL%\ >O.Common\Newport-XPS_registerRecordDeviceDriver.cpp

echo BatchCompleted: %ERRORLEVEL%