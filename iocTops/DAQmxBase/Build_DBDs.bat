if not exist O.Common md O.Common
if not exist %TOPLEVEL%\DBD md %TOPLEVEL%\DBD
SET PERL5LIB=C:\Strawberrylib\perl
perl -CSD %DLLDIR%\dbdExpand.pl  -I. -I.. -I../O.Common -I%TOPLEVEL%\dbd  -o O.Common\DAQmxBase.dbd base.dbd src/DAQmxBaseSupport.dbd asyn.dbd
perl -CSD %DLLDIR%\registerRecordDeviceDriver.pl O.Common\DAQmxBase.dbd DAQmxBase_registerRecordDeviceDriver %TOPLEVEL% >O.Common\DAQmxBase_registerRecordDeviceDriver.cpp