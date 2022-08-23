SET TOPLEVEL=%1
SET BUILDIR=%2
SET DLLDIR=%3

REM SET TOPLEVEL=C:\Users\ktn98257\source\repos\epics.dotnet\
REM SET DLLDIR=C:\Users\ktn98257\source\repos\epics.dotnet\x64\Release_DLL\
REM SET BUILDIR=C:\Users\ktn98257\source\repos\epics.dotnet\base-3.15.9\src\std\

if not exist %TOPLEVEL%include  md %TOPLEVEL%include
if not exist %TOPLEVEL%dbd  md %TOPLEVEL%dbd

xcopy /Y /Q O.Common\*.h %TOPLEVEL%include > NUL
SET PERL5LIB=C:\Strawberry\lib\perl
if not exist C:\strawberry\perl\lib\DBD.pm  mklink /h C:\strawberry\perl\lib\DBD.pm %TOPLEVEL%lib\perl\DBD.pm
if not exist C:\strawberry\perl\lib\DBD  mklink /d C:\strawberry\perl\lib\DBD %TOPLEVEL%lib\perl\DBD
if not exist C:\strawberry\perl\lib\EPICS  mklink /d C:\strawberry\perl\lib\EPICS %TOPLEVEL%lib\perl\EPICS

perl -CSD %DLLDIR%\dbdExpand.pl  -I. -I.. -I..\O.Common -I%TOPLEVEL%dbd -o O.Common\stdRecords.dbd aaiRecord.dbd aaoRecord.dbd aiRecord.dbd aoRecord.dbd biRecord.dbd boRecord.dbd calcRecord.dbd calcoutRecord.dbd compressRecord.dbd dfanoutRecord.dbd eventRecord.dbd fanoutRecord.dbd histogramRecord.dbd longinRecord.dbd longoutRecord.dbd lsiRecord.dbd lsoRecord.dbd mbbiRecord.dbd mbbiDirectRecord.dbd mbboRecord.dbd mbboDirectRecord.dbd permissiveRecord.dbd printfRecord.dbd selRecord.dbd seqRecord.dbd stateRecord.dbd stringinRecord.dbd stringoutRecord.dbd subRecord.dbd subArrayRecord.dbd waveformRecord.dbd

xcopy /Y /Q O.Common\stdRecords.dbd %TOPLEVEL%dbd > NUL

echo BatchCompleted: %ERRORLEVEL%