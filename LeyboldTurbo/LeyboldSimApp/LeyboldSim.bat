echo on
call %~dp0..\iocBoot\iocLeyboldSim\dllPath.bat
REM NumPumps =1 NoOfPZD =6
%~dp0\LeyboldSim.py %1 %2
exit
