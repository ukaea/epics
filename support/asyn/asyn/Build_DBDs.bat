SET TOPLEVEL=%1
SET BUILDIR=%2
SET DLLDIR=%3

if not exist %TOPLEVEL%include  md %TOPLEVEL%include
if not exist %TOPLEVEL%dbd  md %TOPLEVEL%dbd
if not exist O.Common  md O.Common

SET PERL5LIB=C:\strawberry\lib\perl
if not exist C:\strawberry\perl\lib\DBD.pm  mklink /h C:\strawberry\perl\lib\DBD.pm %TOPLEVEL%lib\perl\DBD.pm
if not exist C:\strawberry\perl\lib\DBD  mklink /d C:\strawberry\perl\lib\DBD %TOPLEVEL%lib\perl\DBD
if not exist C:\strawberry\perl\lib\EPICS  mklink /d C:\strawberry\perl\lib\EPICS %TOPLEVEL%lib\perl\EPICS

cd asynRecord
perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%dbd -o ..\O.Common\asynRecord.h asynRecord.dbd

cd %BUILDIR%
xcopy /Y /Q  .\asynDriver\*.h  %TOPLEVEL%include > NUL
xcopy /Y /Q  .\asynPortDriver\*.h  %TOPLEVEL%include > NUL
xcopy /Y /Q  .\asynPortClient\*.h  %TOPLEVEL%include > NUL
xcopy /Y /Q  .\asynGpib\*.h  %TOPLEVEL%include > NUL
xcopy /Y /Q  .\devEpics\*.h   %TOPLEVEL%include > NUL
xcopy /Y /Q  .\interfaces\*.h  %TOPLEVEL%include > NUL
xcopy /Y /Q  .\miscellaneous\*.h  %TOPLEVEL%include > NUL
xcopy /Y /Q  .\drvAsynSerial\*.h %TOPLEVEL%include > NUL
xcopy /Y /Q  .\devGpib\*.h %TOPLEVEL%include > NUL
xcopy /Y /Q  .\asynPortDriver\exceptions %TOPLEVEL%include > NUL
xcopy /Y /Q  .\drvAsynSerial\*.dbd %TOPLEVEL%dbd > NUL
xcopy /Y /Q  .\miscellaneous\*.dbd  %TOPLEVEL%dbd > NUL
xcopy /Y /Q  .\asynRecord\*.dbd  %TOPLEVEL%dbd > NUL
xcopy /Y /Q  .\devEpics\*.dbd   %TOPLEVEL%dbd > NUL

echo BatchCompleted: %ERRORLEVEL%