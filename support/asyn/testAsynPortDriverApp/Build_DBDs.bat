SET TOPLEVEL=%1
SET BUILDIR=%2
SET DLLDIR=%3

if not exist O.Common  md O.Common
SET PERL5LIB=C:\Strawberry\lib\perl

if not exist C:\strawberry\perl\lib\DBD.pm  mklink /h C:\strawberry\perl\lib\DBD.pm %TOPLEVEL%lib\perl\DBD.pm
if not exist C:\strawberry\perl\lib\DBD  mklink /d C:\strawberry\perl\lib\DBD %TOPLEVEL%lib\perl\DBD
if not exist C:\strawberry\perl\lib\EPICS  mklink /d C:\strawberry\perl\lib\EPICS %TOPLEVEL%lib\perl\EPICS

perl -CSD %DLLDIR%registerRecordDeviceDriver.pl  -I%TOPLEVEL%dbd -o O.Common\testAsynPortDriver_registerRecordDeviceDriver.cpp src\testAsynPortDriverInclude.dbd testAsynPortDriver_registerRecordDeviceDriver %TOPLEVEL%
xcopy /Y /Q Db\*.db %TOPLEVEL%db > NUL
xcopy /Y /Q O.Common\*.dbd %TOPLEVEL%dbd > NUL

echo BatchCompleted: %ERRORLEVEL%