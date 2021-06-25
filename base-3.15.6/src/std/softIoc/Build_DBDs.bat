SET TOPLEVEL=%1
SET BUILDIR=%2
SET DLLDIR=%3

if not exist O.Common md O.Common
if not exist %TOPLEVEL%\dbd md %TOPLEVEL%\dbd

SET PERL5LIB=C:\Strawberry\lib\perl
if not exist C:\strawberry\perl\lib\DBD.pm  mklink /h C:\strawberry\perl\lib\DBD.pm %TOPLEVEL%lib\perl\DBD.pm
if not exist C:\strawberry\perl\lib\DBD  mklink /d C:\strawberry\perl\lib\DBD %TOPLEVEL%lib\perl\DBD
if not exist C:\strawberry\perl\lib\EPICS  mklink /d C:\strawberry\perl\lib\EPICS %TOPLEVEL%lib\perl\EPICS

perl -CSD %DLLDIR%\dbdExpand.pl  -I. -I..\O.Common -I..\filters -I%TOPLEVEL%\dbd -o O.Common\softIoc.dbd base.dbd dlload.dbd system.dbd
perl -CSD %DLLDIR%\registerRecordDeviceDriver.pl O.Common\softIoc.dbd softIoc_registerRecordDeviceDriver %TOPLEVEL%\ >O.Common\softIoc_registerRecordDeviceDriver.cpp
xcopy /Y /Q /D /R *.dbd %TOPLEVEL%\dbd > NUL

echo BatchCompleted: %ERRORLEVEL%