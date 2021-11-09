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

perl -CSD %DLLDIR%dbdExpand.pl -I%TOPLEVEL%DBD -o O.Common\as.dbd asInclude.dbd

xcopy /Y /Q *.dbd %TOPLEVEL%dbd > NUL

echo BatchCompleted: %ERRORLEVEL%