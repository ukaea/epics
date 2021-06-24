# TIMI PUTBACK
#==============
#SET TOPLEVEL=%1
#SET BUILDIR=%2
#SET DLLDIR=%3

SET TOPLEVEL=C:\Users\ktn98257\source\repos\epics.dotnet\
SET BUILDIR=C:\Users\ktn98257\source\repos\epics.dotnet\base-7.0.3.1\modules\database\src\std\softIoc
SET DLLDIR=C:\Users\ktn98257\source\repos\epics.dotnet\Debug_DLL\

if not exist O.Common md O.Common
if not exist %TOPLEVEL%\dbd  md %TOPLEVEL%\dbd
SET PERL5LIB=%TOPLEVEL%\lib\perl

perl -CSD %DLLDIR%\dbdExpand.pl  -I. -I..\O.Common -I..\filters -I%TOPLEVEL%\dbd -o O.Common\softIoc.dbd base.dbd dlload.dbd system.dbd
perl -CSD %DLLDIR%\registerRecordDeviceDriver.pl O.Common\softIoc.dbd softIoc_registerRecordDeviceDriver %TOPLEVEL%\ >O.Common\softIoc_registerRecordDeviceDriver.cpp
xcopy /Y /Q /D /R *.dbd %TOPLEVEL%\dbd > NUL

echo BatchCompleted: %ERRORLEVEL%