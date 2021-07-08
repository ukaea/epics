SET TOPLEVEL=%1
SET BUILDIR=%2
SET DLLDIR=%3

SET PERL5LIB=C:\Strawberry\lib\perl

if not exist C:\strawberry\perl\lib\DBD.pm  mklink /h C:\strawberry\perl\lib\DBD.pm %TOPLEVEL%lib\perl\DBD.pm
if not exist C:\strawberry\perl\lib\DBD  mklink /d C:\strawberry\perl\lib\DBD %TOPLEVEL%lib\perl\DBD
if not exist C:\strawberry\perl\lib\EPICS  mklink /d C:\strawberry\perl\lib\EPICS %TOPLEVEL%lib\perl\EPICS

perl -CSD %DLLDIR%dbdExpand.pl -I%TOPLEVEL%DBD -I%BUILDIR%src -o %TOPLEVEL%dbd\test.dbd %BUILDIR%src\testInclude.dbd

echo BatchCompleted: %ERRORLEVEL%