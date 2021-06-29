SET TOPLEVEL=%1
SET BUILDIR=%2
SET DLLDIR=%3

if not exist DBD  md %TOPLEVEL%\DBD
if not exist %TOPLEVEL%include  md %TOPLEVEL%\include

SET PERL5LIB=C:\Strawberry\lib\perl
perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I./O.Common -I$%TOPLEVEL%dbd -o O.Common/%(Filename).h %(Filename).dbd

xcopy /Y /Q /D /R *.dbd  %TOPLEVEL%\dbd > NUL
xcopy /Y /Q /D /R *.h  %TOPLEVEL%\include > NUL

echo BatchCompleted: %ERRORLEVEL%