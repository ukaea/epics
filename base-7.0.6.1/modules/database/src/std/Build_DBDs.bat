SET TOPLEVEL=%1
SET BUILDIR=%2
SET DLLDIR=%3

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

cd %BUILDIR%\O.Common
perl -CSD %DLLDIR%podRemove.pl -o .\aiRecord.dbd ..\rec\aiRecord.dbd.pod
perl -CSD %DLLDIR%podRemove.pl -o .\aoRecord.dbd ..\rec\aoRecord.dbd.pod
perl -CSD %DLLDIR%podRemove.pl -o .\aSubRecord.dbd ..\rec\aSubRecord.dbd.pod
perl -CSD %DLLDIR%podRemove.pl -o .\biRecord.dbd ..\rec\biRecord.dbd.pod
perl -CSD %DLLDIR%podRemove.pl -o .\boRecord.dbd ..\rec\boRecord.dbd.pod
perl -CSD %DLLDIR%podRemove.pl -o .\calcRecord.dbd ..\rec\calcRecord.dbd.pod
perl -CSD %DLLDIR%podRemove.pl -o .\compressRecord.dbd ..\rec\compressRecord.dbd.pod
perl -CSD %DLLDIR%podRemove.pl -o .\dfanoutRecord.dbd ..\rec\dfanoutRecord.dbd.pod
perl -CSD %DLLDIR%podRemove.pl -o .\eventRecord.dbd ..\rec\eventRecord.dbd.pod
perl -CSD %DLLDIR%podRemove.pl -o .\fanoutRecord.dbd ..\rec\fanoutRecord.dbd.pod
perl -CSD %DLLDIR%podRemove.pl -o .\filters.dbd ..\filters\filters.dbd.pod
perl -CSD %DLLDIR%podRemove.pl -o .\int64inRecord.dbd ..\rec\int64inRecord.dbd.pod
perl -CSD %DLLDIR%podRemove.pl -o .\int64outRecord.dbd ..\rec\int64outRecord.dbd.pod
perl -CSD %DLLDIR%podRemove.pl -o .\links.dbd ..\link\links.dbd.pod
perl -CSD %DLLDIR%podRemove.pl -o .\lsiRecord.dbd ..\rec\lsiRecord.dbd.pod
perl -CSD %DLLDIR%podRemove.pl -o .\lsoRecord.dbd ..\rec\lsoRecord.dbd.pod
perl -CSD %DLLDIR%podRemove.pl -o .\longinRecord.dbd ..\rec\longinRecord.dbd.pod
perl -CSD %DLLDIR%podRemove.pl -o .\longoutRecord.dbd ..\rec\longoutRecord.dbd.pod
perl -CSD %DLLDIR%podRemove.pl -o .\mbbiDirectRecord.dbd ..\rec\mbbiDirectRecord.dbd.pod
perl -CSD %DLLDIR%podRemove.pl -o .\mbboDirectRecord.dbd ..\rec\mbboDirectRecord.dbd.pod
perl -CSD %DLLDIR%podRemove.pl -o .\mbbiRecord.dbd ..\rec\mbbiRecord.dbd.pod
perl -CSD %DLLDIR%podRemove.pl -o .\mbboRecord.dbd ..\rec\mbboRecord.dbd.pod
perl -CSD %DLLDIR%podRemove.pl -o .\permissiveRecord.dbd ..\rec\permissiveRecord.dbd.pod
perl -CSD %DLLDIR%podRemove.pl -o .\selRecord.dbd ..\rec\selRecord.dbd.pod
perl -CSD %DLLDIR%podRemove.pl -o .\seqRecord.dbd ..\rec\seqRecord.dbd.pod
perl -CSD %DLLDIR%podRemove.pl -o .\stateRecord.dbd ..\rec\stateRecord.dbd.pod
perl -CSD %DLLDIR%podRemove.pl -o .\stringinRecord.dbd ..\rec\stringinRecord.dbd.pod
perl -CSD %DLLDIR%podRemove.pl -o .\stringoutRecord.dbd ..\rec\stringoutRecord.dbd.pod
perl -CSD %DLLDIR%podRemove.pl -o .\subArrayRecord.dbd ..\rec\subArrayRecord.dbd.pod
perl -CSD %DLLDIR%podRemove.pl -o .\subRecord.dbd ..\rec\subRecord.dbd.pod
perl -CSD %DLLDIR%podRemove.pl -o .\waveformRecord.dbd ..\rec\waveformRecord.dbd.pod

cd %BUILDIR%\dev
perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o ..\O.Common\devSoft.h devSoft.dbd

cd %BUILDIR%\O.Common
xcopy /y /q *.dbd %TOPLEVEL%DBD > NUL

cd %BUILDIR%\
xcopy /y /q  dev\*.dbd %TOPLEVEL%DBD > NUL

xcopy /y /q  dev\*.dbd %TOPLEVEL%DBD > NUL

echo BatchCompleted: %ERRORLEVEL%