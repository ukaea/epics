if not exist O.Common md O.Common

set PERL5LIB=C:\Strawberry\lib\perl
perl -CSD %DLLDIR%\dbdExpand.pl  -I. -I.. -I../O.Common -I%TOPLEVEL%\dbd  -o O.Common\WithAsyn.dbd base.dbd motorSupport.dbd devAcsMotor.dbd devImsMotor.dbd devMclennanMotor.dbd devMicos.dbd devMicroMo.dbd devNewport.dbd LdevNewportTS.dbd devPIMotor.dbd devOms.dbd devSoftMotor.dbd motorSimSupport.dbd devSmartMotorMotor.dbd devKohzuMotor.dbd devAttocube.dbd devAerotech.dbd ACRMotorSupport.dbd drvAsynSerialPort.dbd drvAsynIPPort.dbd busySupport.dbd devSequencer.dbd
perl -CSD %DLLDIR%\registerRecordDeviceDriver.pl  -I. -I./O.Common -I%TOPLEVEL%\dbd -o  O.Common\WithAsyn_registerRecordDeviceDriver.cpp O.Common/WithAsyn.dbd WithAsyn_registerRecordDeviceDriver ..\..\..\
