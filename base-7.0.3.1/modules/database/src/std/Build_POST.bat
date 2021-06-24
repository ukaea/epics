SET TOPLEVEL=%1
SET BUILDIR=%2
SET DLLDIR=%3

if not exist %TOPLEVEL%\include  md %TOPLEVEL%\include
if not exist %TOPLEVEL%\dbd  md %TOPLEVEL%\dbd

xcopy /Y /Q  /D O.Common\*.h %TOPLEVEL%\include > NUL
SET PERL5LIB=C:\Strawberry\lib\perl

perl -CSD %DLLDIR%\makeIncludeDbd.pl aaiRecord.dbd aaoRecord.dbd aiRecord.dbd aoRecord.dbd aSubRecord.dbd biRecord.dbd boRecord.dbd calcRecord.dbd calcoutRecord.dbd compressRecord.dbd dfanoutRecord.dbd eventRecord.dbd fanoutRecord.dbd histogramRecord.dbd int64inRecord.dbd int64outRecord.dbd longinRecord.dbd longoutRecord.dbd lsiRecord.dbd lsoRecord.dbd mbbiRecord.dbd mbbiDirectRecord.dbd mbboRecord.dbd mbboDirectRecord.dbd permissiveRecord.dbd printfRecord.dbd selRecord.dbd seqRecord.dbd stateRecord.dbd stringinRecord.dbd stringoutRecord.dbd subRecord.dbd subArrayRecord.dbd waveformRecord.dbd O.Common\stdRecords.dbd

xcopy /Y /Q  /D O.Common\*.dbd %TOPLEVEL%\dbd

echo BatchCompleted: %ERRORLEVEL%