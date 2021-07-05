SET TOPLEVEL=%1
SET BUILDIR=%2
SET DLLDIR=%3

if not exist %TOPLEVEL%include  md %TOPLEVEL%include
if not exist %TOPLEVEL%dbd  md %TOPLEVEL%\dbd
if not exist O.Common  md O.Common

SET PERL5LIB=C:\strawberry\lib\perl
if not exist C:\strawberry\perl\lib\DBD.pm  mklink /h C:\strawberry\perl\lib\DBD.pm %TOPLEVEL%lib\perl\DBD.pm
if not exist C:\strawberry\perl\lib\DBD  mklink /d C:\strawberry\perl\lib\DBD %TOPLEVEL%lib\perl\DBD
if not exist C:\strawberry\perl\lib\EPICS  mklink /d C:\strawberry\perl\lib\EPICS %TOPLEVEL%lib\perl\EPICS

perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I../O.Common -I%TOPLEVEL%include -I%TOPLEVEL%dbd -o ../O.Common/asynRecord.h %asynRecord.dbd

cd $(ProjectDir)
xcopy /Y /Q /D /R .\asynDriver\*.h  %TOPLEVEL%\include > NUL
xcopy /Y /Q /D /R .\asynPortDriver\*.h  %TOPLEVEL%\include > NUL
xcopy /Y /Q /D /R .\asynGpib\*.h  %TOPLEVEL%\include > NUL
xcopy /Y /Q /D /R .\devEpics\*.h   %TOPLEVEL%\include > NUL
xcopy /Y /Q /D /R .\interfaces\*.h  %TOPLEVEL%\include > NUL
xcopy /Y /Q /D /R .\miscellaneous\*.h  %TOPLEVEL%\include > NUL
xcopy /Y /Q /D /R .\drvAsynSerial\*.h %TOPLEVEL%\include > NUL
xcopy /Y /Q /D /R .\devGpib\*.h %TOPLEVEL%\include > NUL
xcopy /Y /Q /D /R .\asynPortDriver\exceptions %TOPLEVEL%\include > NUL
xcopy /Y /Q /D /R .\drvAsynSerial\*.dbd %TOPLEVEL%\dbd > NUL
xcopy /Y /Q /D /R .\miscellaneous\*.dbd  %TOPLEVEL%\dbd > NUL
xcopy /Y /Q /D /R .\asynRecord\*.dbd  %TOPLEVEL%\dbd > NUL
xcopy /Y /Q /D /R .\devEpics\*.dbd   %TOPLEVEL%\dbd > NUL

echo BatchCompleted: %ERRORLEVEL%