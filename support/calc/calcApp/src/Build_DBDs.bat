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

cd %BUILDIR%

perl -CSD %DLLDIR%dbdToRecordtypeH.pl -I.\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%dbd -o .\O.Common\aCalcoutRecord.h aCalcoutRecord.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl -I.\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%dbd -o .\O.Common\sCalcoutRecord.h sCalcoutRecord.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl -I.\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%dbd -o .\O.Common\sseqRecord.h sseqRecord.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl -I.\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%dbd -o .\O.Common\swaitRecord.h swaitRecord.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl -I.\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%dbd -o .\O.Common\transformRecord.h transformRecord.dbd

perl cat.pl calcSupport_LOCAL.dbd calcSupport_withSSCAN.dbd %TOPLEVEL%dbd\calcSupport.dbd
perl cat.pl calcSupport_LOCAL.dbd calcSupport_withSSCAN.dbd %TOPLEVEL%dbd\calc.dbd

xcopy /Y /Q *.dbd %TOPLEVEL%DBD > NUL
xcopy /Y /Q O.Common\*.h %TOPLEVEL%include > NUL

echo BatchCompleted: %ERRORLEVEL%