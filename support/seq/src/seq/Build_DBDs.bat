# TIMI PUTBACK
#==============
#SET TOPLEVEL=%1
#SET BUILDIR=%2
#SET DLLDIR=%3

SET TOPLEVEL=C:\Users\ktn98257\source\repos\epics.dotnet\
SET BUILDIR=C:\Users\ktn98257\source\repos\epics.dotnet\base-7.0.3.1\support\database\src\std\softIoc
SET DLLDIR=C:\Users\ktn98257\source\repos\epics.dotnet\Debug_DLL\

if not exist %TOPLEVEL%\include md %TOPLEVEL%\include
if not exist O.Common md O.Common

xcopy /Y /Q /D /R *.h  %TOPLEVEL%\include > NUL
xcopy /Y /Q /D /R ..\Common\*.h  %TOPLEVEL%\include > NUL

SET PERL5LIB=C:\Strawberry\lib\perl
perl -CSD %DLLDIR%\dbdExpand.pl  -I ./O.Common -I. -I.. -I./O.Common -I%TOPLEVEL%\DBD -o O.Common\seqSoftIoc.dbd base.dbd ..\..\test\validate\testSupport.dbd
perl -CSD %DLLDIR%\registerRecordDeviceDriver.pl  -I ./O.Common -I. -I%TOPLEVEL%\dbd -o ./O.Common/seqSoftIoc_registerRecordDeviceDriver.cpp ./O.Common/seqSoftIoc.dbd seqSoftIoc_registerRecordDeviceDriver %TOPLEVEL%\
perl -CSD seq_release.pl 2.1.15 > O.Common/seq_release.h

echo BatchCompleted: %ERRORLEVEL%