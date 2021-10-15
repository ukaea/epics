SET TOPLEVEL=%1
SET BUILDIR=%2
SET DLLDIR=%3

SET USE_TYPED_DSET=false

if not exist O.Common md O.Common
if not exist %TOPLEVEL%\dbd md %TOPLEVEL%\dbd

SET PERL5LIB=C:\Strawberry\lib\perl
if not exist C:\strawberry\perl\lib\DBD.pm  mklink /h C:\strawberry\perl\lib\DBD.pm %TOPLEVEL%lib\perl\DBD.pm
if not exist C:\strawberry\perl\lib\DBD  mklink /d C:\strawberry\perl\lib\DBD %TOPLEVEL%lib\perl\DBD
if not exist C:\strawberry\perl\lib\EPICS  mklink /d C:\strawberry\perl\lib\EPICS %TOPLEVEL%lib\perl\EPICS

cd %BUILDIR%\O.Common
perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o ..\O.Common\aaiRecord.h aaiRecord.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o ..\O.Common\aaoRecord.h aaoRecord.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o ..\O.Common\aiRecord.h aiRecord.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o ..\O.Common\aoRecord.h aoRecord.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o ..\O.Common\aSubRecord.h aSubRecord.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o ..\O.Common\biRecord.h biRecord.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o ..\O.Common\boRecord.h boRecord.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o ..\O.Common\calcoutRecord.h calcoutRecord.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o ..\O.Common\calcRecord.h calcRecord.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o ..\O.Common\compressRecord.h compressRecord.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o ..\O.Common\dfanoutRecord.h dfanoutRecord.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o ..\O.Common\eventRecord.h eventRecord.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o ..\O.Common\fanoutRecord.h fanoutRecord.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o ..\O.Common\histogramRecord.h histogramRecord.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o ..\O.Common\int64inRecord.h int64inRecord.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o ..\O.Common\int64outRecord.h int64outRecord.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o ..\O.Common\longinRecord.h longinRecord.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o ..\O.Common\longoutRecord.h longoutRecord.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o ..\O.Common\lsiRecord.h lsiRecord.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o ..\O.Common\lsoRecord.h lsoRecord.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o ..\O.Common\mbbiRecord.h mbbiRecord.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o ..\O.Common\mbboRecord.h mbboRecord.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o ..\O.Common\mbbiDirectRecord.h mbbiDirectRecord.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o ..\O.Common\mbboDirectRecord.h mbboDirectRecord.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o ..\O.Common\selRecord.h selRecord.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o ..\O.Common\seqRecord.h seqRecord.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o ..\O.Common\stateRecord.h stateRecord.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o ..\O.Common\stringinRecord.h stringinRecord.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o ..\O.Common\stringoutRecord.h stringoutRecord.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o ..\O.Common\subArrayRecord.h subArrayRecord.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o ..\O.Common\subRecord.h subRecord.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o ..\O.Common\waveformRecord.h waveformRecord.dbd

cd %BUILDIR%\rec
perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o ..\O.Common\aaiRecord.h aaiRecord.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o ..\O.Common\aaoRecord.h aaoRecord.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o ..\O.Common\biRecord.h biRecord.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o ..\O.Common\boRecord.h boRecord.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o ..\O.Common\calcoutRecord.h calcoutRecord.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o ..\O.Common\calcRecord.h calcRecord.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o ..\O.Common\dfanoutRecord.h dfanoutRecord.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o ..\O.Common\eventRecord.h eventRecord.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o ..\O.Common\fanoutRecord.h fanoutRecord.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o ..\O.Common\histogramRecord.h histogramRecord.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o ..\O.Common\int64inRecord.h int64inRecord.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o ..\O.Common\int64outRecord.h int64outRecord.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o ..\O.Common\longinRecord.h longinRecord.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o ..\O.Common\longoutRecord.h longoutRecord.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o ..\O.Common\lsiRecord.h lsiRecord.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o ..\O.Common\lsoRecord.h lsoRecord.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o ..\O.Common\mbbiDirectRecord.h mbbiDirectRecord.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o ..\O.Common\mbboDirectRecord.h mbboDirectRecord.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o ..\O.Common\permissiveRecord.h permissiveRecord.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o ..\O.Common\printfRecord.h printfRecord.dbd

perl -CSD %DLLDIR%podRemove.pl  -o ..\O.Common\aiRecord.dbd .\aiRecord.dbd.pod
perl -CSD %DLLDIR%podRemove.pl  -o ..\O.Common\aoRecord.dbd .\aoRecord.dbd.pod
perl -CSD %DLLDIR%podRemove.pl  -o ..\O.Common\aSubRecord.dbd .\aSubRecord.dbd.pod
perl -CSD %DLLDIR%podRemove.pl  -o ..\O.Common\biRecord.dbd .\biRecord.dbd.pod
perl -CSD %DLLDIR%podRemove.pl  -o ..\O.Common\boRecord.dbd .\boRecord.dbd.pod
perl -CSD %DLLDIR%podRemove.pl  -o ..\O.Common\calcoutRecord.dbd .\calcoutRecord.dbd.pod
perl -CSD %DLLDIR%podRemove.pl  -o ..\O.Common\calcRecord.dbd .\calcRecord.dbd.pod
perl -CSD %DLLDIR%podRemove.pl  -o ..\O.Common\compressRecord.dbd .\compressRecord.dbd.pod
perl -CSD %DLLDIR%podRemove.pl  -o ..\O.Common\dfanoutRecord.dbd .\dfanoutRecord.dbd.pod
perl -CSD %DLLDIR%podRemove.pl  -o ..\O.Common\eventRecord.dbd .\eventRecord.dbd.pod
perl -CSD %DLLDIR%podRemove.pl  -o ..\O.Common\fanoutRecord.dbd .\fanoutRecord.dbd.pod
perl -CSD %DLLDIR%podRemove.pl  -o ..\O.Common\filters.dbd ..\filters\filters.dbd.pod
perl -CSD %DLLDIR%podRemove.pl  -o ..\O.Common\int64inRecord.dbd .\int64inRecord.dbd.pod
perl -CSD %DLLDIR%podRemove.pl  -o ..\O.Common\int64outRecord.dbd .\int64outRecord.dbd.pod
perl -CSD %DLLDIR%podRemove.pl  -o ..\O.Common\links.dbd ..\link\links.dbd.pod
perl -CSD %DLLDIR%podRemove.pl  -o ..\O.Common\lsiRecord.dbd .\lsiRecord.dbd.pod
perl -CSD %DLLDIR%podRemove.pl  -o ..\O.Common\lsoRecord.dbd .\lsoRecord.dbd.pod
perl -CSD %DLLDIR%podRemove.pl  -o ..\O.Common\longinRecord.dbd .\longinRecord.dbd.pod
perl -CSD %DLLDIR%podRemove.pl  -o ..\O.Common\longoutRecord.dbd .\longoutRecord.dbd.pod
perl -CSD %DLLDIR%podRemove.pl  -o ..\O.Common\mbbiDirectRecord.dbd .\mbbiDirectRecord.dbd.pod
perl -CSD %DLLDIR%podRemove.pl  -o ..\O.Common\mbboDirectRecord.dbd .\mbboDirectRecord.dbd.pod
perl -CSD %DLLDIR%podRemove.pl  -o ..\O.Common\mbbiRecord.dbd .\mbbiRecord.dbd.pod
perl -CSD %DLLDIR%podRemove.pl  -o ..\O.Common\mbboRecord.dbd .\mbboRecord.dbd.pod
perl -CSD %DLLDIR%podRemove.pl  -o ..\O.Common\permissiveRecord.dbd .\permissiveRecord.dbd.pod
perl -CSD %DLLDIR%podRemove.pl  -o ..\O.Common\selRecord.dbd .\selRecord.dbd.pod
perl -CSD %DLLDIR%podRemove.pl  -o ..\O.Common\seqRecord.dbd .\seqRecord.dbd.pod
perl -CSD %DLLDIR%podRemove.pl  -o ..\O.Common\stateRecord.dbd .\stateRecord.dbd.pod
perl -CSD %DLLDIR%podRemove.pl  -o ..\O.Common\stringinRecord.dbd .\stringinRecord.dbd.pod
perl -CSD %DLLDIR%podRemove.pl  -o ..\O.Common\stringoutRecord.dbd .\stringoutRecord.dbd.pod
perl -CSD %DLLDIR%podRemove.pl  -o ..\O.Common\subArrayRecord.dbd .\subArrayRecord.dbd.pod
perl -CSD %DLLDIR%podRemove.pl  -o ..\O.Common\subRecord.dbd .\subRecord.dbd.pod
perl -CSD %DLLDIR%podRemove.pl  -o ..\O.Common\waveformRecord.dbd .\waveformRecord.dbd.pod

cd %BUILDIR%\dev
perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o ..\O.Common\devSoft.h devSoft.dbd

cd %BUILDIR%\O.Common
REM xcopy /y /q *.dbd %TOPLEVEL%DBD > NUL

cd %BUILDIR%\
REM xcopy /y /q  dev\*.dbd %TOPLEVEL%DBD > NUL

echo BatchCompleted: %ERRORLEVEL%