if not exist O.Common md O.Common
if not exist %TOPLEVEL%\include  md %TOPLEVEL%\include
if not exist %TOPLEVEL%\dbd  md exist %TOPLEVEL%\dbd

xcopy /Y /Q /D /R  *.dbd  %TOPLEVEL%\dbd > NUL

SET PERL5LIB=C:\Strawberry\lib\perl
perl -CSD %DLLDIR%\dbdToRecordtypeH.pl  -I. -I.. -I../O.Common -I%TOPLEVEL%\dbd -o  O.Common/busyRecord.h busyRecord.dbd
xcopy /Y /Q /D /R  O.Common\*.h  %TOPLEVEL%\include > NUL

perl cat.pl busySupport_LOCAL.dbd busySupport_withASYN.dbd   %TOPLEVEL%\dbd\busySupport.dbd

