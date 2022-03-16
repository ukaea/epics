SET TOPLEVEL=%1
SET BUILDIR=%2
SET DLLDIR=%3

if not exist O.Common  md O.Common
if not exist %TOPLEVEL%include  md %TOPLEVEL%include
if not exist %TOPLEVEL%dbd  md %TOPLEVEL%dbd

SET PERL5LIB=C:\Strawberry\lib\perl
if not exist C:\strawberry\perl\lib\DBD.pm  mklink /h C:\strawberry\perl\lib\DBD.pm %TOPLEVEL%lib\perl\DBD.pm
if not exist C:\strawberry\perl\lib\DBD  mklink /d C:\strawberry\perl\lib\DBD %TOPLEVEL%lib\perl\DBD
if not exist C:\strawberry\perl\lib\EPICS  mklink /d C:\strawberry\perl\lib\EPICS %TOPLEVEL%lib\perl\EPICS

perl %DLLDIR%makeInstallDir.pl %TOPLEVEL% > O.Common\epicsInstallDir.h

perl -CSD %DLLDIR%dbdExpand.pl -I. -I.. -I..\O.Common -I%TOPLEVEL%DBD -o .\O.Common\thinBaseIOC.dbd  base.dbd
perl -CSD %DLLDIR%registerRecordDeviceDriver.pl O.Common\thinBaseIOC.dbd thinIoc_registerRecordDeviceDriver %TOPLEVEL% > O.Common\thinIoc_registerRecordDeviceDriver.cpp

xcopy /Y /Q *.dbd %TOPLEVEL%DBD > NUL
xcopy /Y /Q O.Common\*.dbd %TOPLEVEL%DBD > NUL

echo BatchCompleted: %ERRORLEVEL%