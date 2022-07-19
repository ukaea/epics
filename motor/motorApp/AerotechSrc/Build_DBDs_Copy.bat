SET TOPLEVEL=%1
SET BUILDIR=%2
SET DLLDIR=%3

if not exist %TOPLEVEL%DBD  md exist %TOPLEVEL%DBD
if not exist %BUILDIR%O.Common  md exist %BUILDIR%O.Common

SET PERL5LIB=C:\Strawberry\lib\perl
if not exist C:\strawberry\perl\lib\DBD.pm  mklink /h C:\strawberry\perl\lib\DBD.pm %TOPLEVEL%lib\perl\DBD.pm
if not exist C:\strawberry\perl\lib\DBD  mklink /d C:\strawberry\perl\lib\DBD %TOPLEVEL%lib\perl\DBD
if not exist C:\strawberry\perl\lib\EPICS  mklink /d C:\strawberry\perl\lib\EPICS %TOPLEVEL%lib\perl\EPICS

cl -C -E -nologo -D__STDC__=0 -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE -I.\O.Common -I%TOPLEVEL%include\compiler\msvc -I%TOPLEVEL%include\os\WIN32 -I%TOPLEVEL%include  EnsembleTrajectoryScan.st > O.Common\EnsembleTrajectoryScan.i
%DLLDIR%snc.exe .\O.Common\EnsembleTrajectoryScan.i -o .\O.Common\EnsembleTrajectoryScan.c

xcopy /Y /Q *.dbd  %TOPLEVEL%dbd > NUL

echo BatchCompleted: %ERRORLEVEL%