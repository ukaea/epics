SET TOPLEVEL=%1
SET BUILDIR=%2
SET DLLDIR=%3

REM SET TOPLEVEL=C:\Users\ktn98257\source\repos\epics.dotnet\
REM SET DLLDIR=C:\Users\ktn98257\source\repos\epics.dotnet\x64\Debug_DLL\
REM SET BUILDIR=C:\Users\ktn98257\source\repos\epics.dotnet\iocTops\AcsMotionControl\acsMotionApp\src

if not exist O.Common  md O.Common
if not exist %TOPLEVEL%include  md %TOPLEVEL%include
if not exist %TOPLEVEL%dbd  md %TOPLEVEL%dbd

SET PERL5LIB=C:\Strawberry\lib\perl
if not exist C:\strawberry\perl\lib\DBD.pm  mklink /h C:\strawberry\perl\lib\DBD.pm %TOPLEVEL%lib\perl\DBD.pm
if not exist C:\strawberry\perl\lib\DBD  mklink /d C:\strawberry\perl\lib\DBD %TOPLEVEL%lib\perl\DBD
if not exist C:\strawberry\perl\lib\EPICS  mklink /d C:\strawberry\perl\lib\EPICS %TOPLEVEL%lib\perl\EPICS

perl -CSD %DLLDIR%dbdExpand.pl -I. -I.. -I..\O.Common -I%TOPLEVEL%DBD -o O.Common\acsMotionIOC.dbd  base.dbd calcSupport.dbd asSupport.dbd asyn.dbd motorSupport.dbd motorRecord.dbd AcsMotionSupport.dbd
perl -CSD %DLLDIR%registerRecordDeviceDriver.pl O.Common\acsMotionIOC.dbd acsMotion_registerRecordDeviceDriver %TOPLEVEL% > O.Common\acsMotion_registerRecordDeviceDriver.cpp

xcopy /Y /Q basic_asyn_motor_clf.template %BUILDIR%iocBoot\iocAcsMotion
xcopy /Y /Q O.Common\*.dbd  %TOPLEVEL%DBD > NUL

echo BatchCompleted: %ERRORLEVEL%