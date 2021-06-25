SET TOPLEVEL=%1
SET BUILDIR=%2
SET DLLDIR=%3

if not exist %TOPLEVEL%\include md %TOPLEVEL%\include
if not exist %TOPLEVEL%\dbd  md %TOPLEVEL%\dbd
if not exist O.Common md O.Common

xcopy /Y /Q /D /R *.h  %TOPLEVEL%\include > NUL
xcopy /Y /Q /D /R ..\Common\*.h  %TOPLEVEL%\include > NUL
xcopy /Y /Q /D /R  *.dbd  %TOPLEVEL%\dbd > NUL

cd %BUILDIR%
SET PERL5LIB=C:\Strawberry\lib\perl
if not exist C:\strawberry\perl\lib\DBD.pm  mklink /h C:\strawberry\perl\lib\DBD.pm %TOPLEVEL%lib\perl\DBD.pm
if not exist C:\strawberry\perl\lib\DBD  mklink /d C:\strawberry\perl\lib\DBD %TOPLEVEL%lib\perl\DBD
if not exist C:\strawberry\perl\lib\EPICS  mklink /d C:\strawberry\perl\lib\EPICS %TOPLEVEL%lib\perl\EPICS

perl -CSD %DLLDIR%\dbdExpand.pl  -I ./O.Common -I. -I.. -I./O.Common -I%TOPLEVEL%DBD -I%TOPLEVEL%lib\perl -o O.Common\seqSoftIoc.dbd base.dbd ..\..\test\validate\testSupport.dbd
perl -CSD %DLLDIR%\registerRecordDeviceDriver.pl  -I ./O.Common -I. -I%TOPLEVEL%DBD -I%TOPLEVEL%lib\perl -o ./O.Common/seqSoftIoc_registerRecordDeviceDriver.cpp ./O.Common/seqSoftIoc.dbd seqSoftIoc_registerRecordDeviceDriver %TOPLEVEL%\
perl -CSD seq_release.pl 2.1.15 > O.Common/seq_release.h

echo BatchCompleted: %ERRORLEVEL%