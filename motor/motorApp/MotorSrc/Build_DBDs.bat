SET TOPLEVEL=%1
SET BUILDIR=%2
SET DLLDIR=%3

if not exist DBD  md %TOPLEVEL%DBD
if not exist %TOPLEVEL%include  md %TOPLEVEL%include
if not exist %BUILDIR%O.Common md %BUILDIR%O.Common

SET PERL5LIB=C:\Strawberry\lib\perl
if not exist C:\strawberry\perl\lib\DBD.pm  mklink /h C:\strawberry\perl\lib\DBD.pm %TOPLEVEL%lib\perl\DBD.pm
if not exist C:\strawberry\perl\lib\DBD  mklink /d C:\strawberry\perl\lib\DBD %TOPLEVEL%lib\perl\DBD
if not exist C:\strawberry\perl\lib\EPICS  mklink /d C:\strawberry\perl\lib\EPICS %TOPLEVEL%lib\perl\EPICS
perl -CSD %DLLDIR%dbdToRecordtypeH.pl -I. -I.. -I../O.Common -I%TOPLEVEL%DBD -o  O.Common/motorRecord.h motorRecord.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl -I. -I.. -I../O.Common -I%TOPLEVEL%DBD -o  O.Common/motorSupport.h motorSupport.dbd

xcopy /Y /Q /D /R *.dbd  %TOPLEVEL%dbd > NUL
xcopy /Y /Q /D /R *.h  %TOPLEVEL%include > NUL

echo BatchCompleted: %ERRORLEVEL%