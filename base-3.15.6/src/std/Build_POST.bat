if not exist %TOPLEVEL%\include md %TOPLEVEL%\include

if not exist %TOPLEVEL%\dbd md %TOPLEVEL%\dbd

xcopy /Y /Q  /D O.Common\*.h %TOPLEVEL%\include > NUL
SET PERL5LIB=C:\Strawberry\lib\perl

perl -CSD %DLLDIR%\dbdExpand.pl  -I. -I.. -I../O.Common -I%TOPLEVEL%\dbd -o O.Common/stdRecords.dbd aaiRecord.dbd aaoRecord.dbd O.Common/aiRecord.dbd O.Common/aoRecord.dbd biRecord.dbd boRecord.dbd calcRecord.dbd calcoutRecord.dbd O.Common/compressRecord.dbd dfanoutRecord.dbd eventRecord.dbd fanoutRecord.dbd histogramRecord.dbd longinRecord.dbd longoutRecord.dbd lsiRecord.dbd lsoRecord.dbd mbbiRecord.dbd mbbiDirectRecord.dbd mbboRecord.dbd mbboDirectRecord.dbd permissiveRecord.dbd printfRecord.dbd selRecord.dbd seqRecord.dbd stateRecord.dbd stringinRecord.dbd stringoutRecord.dbd subRecord.dbd subArrayRecord.dbd O.Common/waveformRecord.dbd

xcopy /Y /Q  /D O.Common\*.dbd %TOPLEVEL%\dbd
