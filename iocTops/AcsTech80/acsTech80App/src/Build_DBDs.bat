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

perl -CSD %DLLDIR%dbdExpand.pl -I. -I.. -I..\O.Common -I%TOPLEVEL%DBD -o .\O.Common\acsTechIOC.dbd  base.dbd calcSupport.dbd asSupport.dbd asyn.dbd motorSupport.dbd motorRecord.dbd drvAsynIPPort.dbd devSpiiPlus.dbd
perl -CSD %DLLDIR%registerRecordDeviceDriver.pl O.Common\acsTechIOC.dbd acsTech80_registerRecordDeviceDriver %TOPLEVEL% > O.Common\acsTech80_registerRecordDeviceDriver.cpp


xcopy /Y /Q basic_asyn_motor_clf.template %BUILDIR%iocBoot\iocAcsTech80
xcopy /Y /Q O.Common\*.dbd  %TOPLEVEL%DBD > NUL

echo BatchCompleted: %ERRORLEVEL%