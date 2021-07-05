SET TOPLEVEL=%1

SET PERL5LIB=C:\Strawberry\lib\perl
if not exist C:\strawberry\perl\lib\DBD.pm  mklink /h C:\strawberry\perl\lib\DBD.pm %TOPLEVEL%lib\perl\DBD.pm
if not exist C:\strawberry\perl\lib\DBD  mklink /d C:\strawberry\perl\lib\DBD %TOPLEVEL%lib\perl\DBD
if not exist C:\strawberry\perl\lib\EPICS  mklink /d C:\strawberry\perl\lib\EPICS %TOPLEVEL%lib\perl\EPICS

cd O.Common
perl -CSD %DLLDIR%registerRecordDeviceDriver.pl  -I. -I.. -I../O.Common -I../../../dbd -o testGpibSerial_registerRecordDeviceDriver.cpp testGpibSerial.dbd testGpibSerial_registerRecordDeviceDriver
cd %TOPLEVEL%