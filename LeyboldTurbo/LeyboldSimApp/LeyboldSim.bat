echo on
call ..\iocBoot\iocLeyboldSim\dllPath.bat
REM NumPumps =1 NoOfPZD =6
LeyboldSim.py %1 %2
