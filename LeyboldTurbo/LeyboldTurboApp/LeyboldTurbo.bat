echo off

set NumPumps=%1
if "%NumPumps%"=="" set NumPumps=1

set COMPORT1=COM1:
set COMPORT2=COM2:
set COMPORT3=COM3:
set COMPORT4=COM4:
set COMPORT5=COM5:
set IPPORT1=localhost:5066
set IPPORT2=localhost:5067
set IPPORT3=localhost:5068
set IPPORT4=localhost:5069
set IPPORT5=localhost:5070
LeyboldTurbo.exe ..\iocBoot\iocLeyboldTurbo\st%NumPumps%.simulated.cmd
