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

REM Is this needed TS:3-Mar-2022
REM perl -CSD %DLLDIR%dbdExpand.pl -I. -I.. -I../O.Common -I%TOPLEVEL%DBD -o O.Common\acsTech80.dbd base.dbd devSPiiPlus.dbd 

xcopy /Y /Q *.dbd %TOPLEVEL%DBD > NUL
xcopy /Y /Q %BUILDIR%O.Common\*.dbd  %TOPLEVEL%DBD > NUL

echo BatchCompleted: %ERRORLEVEL%