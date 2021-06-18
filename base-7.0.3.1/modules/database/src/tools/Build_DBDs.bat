if not exist %TOPLEVEL%\lib\perl\DBD md %TOPLEVEL%\lib\perl\DBD
if not exist %TOPLEVEL%\lib\perl\EPICS md %TOPLEVEL%\lib\perl\EPICS
if not exist %TOPLEVEL%\lib md %TOPLEVEL%\lib
if not exist %TOPLEVEL%\lib\perl md %TOPLEVEL%\lib\perl

xcopy /Y /Q /D /R *.pl %DLLDIR%\ > NUL
xcopy /Y /Q /D /R *.pm %TOPLEVEL%\lib\perl > NUL
xcopy /Y /Q /D /R DBD\*.pm %TOPLEVEL%\lib\perl\DBD > NUL
xcopy /Y /Q /D /R EPICS\*.pm %TOPLEVEL%\lib\perl\EPICS > NUL

SET PERL5LIB=C:\Strawberry\lib\perl
perl -CSD %DLLDIR%\convertRelease.pl -T %TOPLEVEL%\ %DLLDIR%\databaseModuleDirs.pm
