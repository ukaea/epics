if not exist O.Common md O.Common
if not exist %TOPLEVEL%\dbd md %TOPLEVEL%\dbd
SET PERL5LIB=C:\Strawberry\lib\perl
perl -CSD $(OutDir)dbdExpand.pl  -I. -I..\O.Common -I..\filters -I%TOPLEVEL%\dbd -o O.Common\softIoc.dbd base.dbd dlload.dbd system.dbd
perl -CSD $(OutDir)registerRecordDeviceDriver.pl O.Common\softIoc.dbd softIoc_registerRecordDeviceDriver %TOPLEVEL%\ >O.Common\softIoc_registerRecordDeviceDriver.cpp
xcopy /Y /Q /D /R *.dbd %TOPLEVEL%\dbd > NUL
