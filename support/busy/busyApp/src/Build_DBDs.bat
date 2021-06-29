SET TOPLEVEL=%1
SET BUILDIR=%2
SET DLLDIR=%3

if not exist O.Common md O.Common
if not exist %TOPLEVEL%\include  md %TOPLEVEL%\include
if not exist %TOPLEVEL%\dbd  md %TOPLEVEL%\dbd

xcopy /Y /Q /D /R  *.dbd  %TOPLEVEL%\DBD > NUL

SET PERL5LIB=C:\Strawberry\lib\perl
if not exist C:\strawberry\perl\lib\DBD.pm  mklink /h C:\strawberry\perl\lib\DBD.pm %TOPLEVEL%lib\perl\DBD.pm
if not exist C:\strawberry\perl\lib\DBD  mklink /d C:\strawberry\perl\lib\DBD %TOPLEVEL%lib\perl\DBD
if not exist C:\strawberry\perl\lib\EPICS  mklink /d C:\strawberry\perl\lib\EPICS %TOPLEVEL%lib\perl\EPICS

perl -CSD %DLLDIR%dbdToRecordtypeH.pl -I. -I.. -I../O.Common -I%TOPLEVEL%DBD -o  O.Common/busyRecord.h busyRecord.dbd
xcopy /Y /Q /D /R  O.Common\*.h  %TOPLEVEL%\include > NUL

perl cat.pl busySupport_LOCAL.dbd busySupport_withASYN.dbd   %TOPLEVEL%\dbd\busySupport.dbd

echo BatchCompleted: %ERRORLEVEL%