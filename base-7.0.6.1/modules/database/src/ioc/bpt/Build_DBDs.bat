SET TOPLEVEL=%1
SET BUILDIR=%2
SET DLLDIR=%3

if not exist %TOPLEVEL%\include md %TOPLEVEL%\include
if not exist %TOPLEVEL%\dbd md %TOPLEVEL%\dbd
xcopy /Y /Q /D /R *.h %TOPLEVEL%\include > NUL

if not exist O.Common md O.Common

SET PERL5LIB=C:\Strawberry\lib\perl
if not exist C:\strawberry\perl\lib\DBD.pm  mklink /h C:\strawberry\perl\lib\DBD.pm %TOPLEVEL%lib\perl\DBD.pm
if not exist C:\strawberry\perl\lib\DBD  mklink /d C:\strawberry\perl\lib\DBD %TOPLEVEL%lib\perl\DBD
if not exist C:\strawberry\perl\lib\EPICS  mklink /d C:\strawberry\perl\lib\EPICS %TOPLEVEL%lib\perl\EPICS

cd %BUILDIR%	
perl -CSD %DLLDIR%dbdToMenuH.pl -I%TOPLEVEL%%include -o .\O.Common\menuConvert.h menuConvert.dbd
perl -CSD %DLLDIR%podRemove.pl  -o O.Common\menuConvert.dbd menuConvert.dbd.pod

xcopy /Y /Q /D /R *.h %TOPLEVEL%\DBD > NUL

echo BatchCompleted: %ERRORLEVEL%