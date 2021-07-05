SET TOPLEVEL=%1
SET BUILDIR=%2
SET DLLDIR=%3

SET PERL5LIB=C:\Strawberry\lib\perl
if not exist C:\strawberry\perl\lib\DBD.pm  mklink /h C:\strawberry\perl\lib\DBD.pm %TOPLEVEL%lib\perl\DBD.pm
if not exist C:\strawberry\perl\lib\DBD  mklink /d C:\strawberry\perl\lib\DBD %TOPLEVEL%lib\perl\DBD
if not exist C:\strawberry\perl\lib\EPICS  mklink /d C:\strawberry\perl\lib\EPICS %TOPLEVEL%lib\perl\EPICS

cd %TOPLEVEL%src
if not exist O.Common md O.Common
perl -CSD %DLLDIR%dbdExpand.pl  -I. -I.. -I../O.Common -Ifilters -I%TOPLEVEL%dbd -o O.Common/testGpibSerial.dbd testGpibSerialSupport.dbd

cd O.Common
perl -CSD %DLLDIR%registerRecordDeviceDriver.pl  -I. -I.. -I../O.Common -I../../../dbd -o testGpibSerial_registerRecordDeviceDriver.cpp testGpibSerial.dbd testGpibSerial_registerRecordDeviceDriver
cd %TOPLEVEL%