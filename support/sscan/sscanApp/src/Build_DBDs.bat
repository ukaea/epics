SET TOPLEVEL=%1
SET BUILDIR=%2
SET DLLDIR=%3

if not exist O.Common md O.Common
if not exist %TOPLEVEL%\include md %TOPLEVEL%\include
if not exist %TOPLEVEL%\dbd md exist %TOPLEVEL%\dbd
if not exist C:\Strawberry\perl\lib\DBD.pm  mklink /h C:\strawberry\perl\lib\DBD.pm %TOPLEVEL%lib\perl\DBD.pm
if not exist C:\Strawberry\perl\lib\DBD  mklink /d C:\strawberry\perl\lib\DBD %TOPLEVEL%lib\perl\DBD
if not exist C:\Strawberry\perl\lib\EPICS  mklink /d C:\strawberry\perl\lib\EPICS %TOPLEVEL%lib\perl\EPICS
 
SET PERL5LIB=C:\Strawberry\lib\perl
perl -CSD %DLLDIR%dbdToMenuH.pl  -I%TOPLEVEL%%include -o .\O.Common\menuSscan.h menuSscan.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I./O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o ./O.Common/sscanparmRecord.h scanparmRecord.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I./O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o ./O.Common/sscanRecord.h sscanRecord.dbd

xcopy /Y /Q /D /R *.h  %TOPLEVEL%include > NUL
xcopy /Y /Q /D /R *.dbd  %TOPLEVEL%dbd > NUL

echo BatchCompleted: %ERRORLEVEL%