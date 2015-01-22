call %~dp0..\..\..\iocBoot\iocLeyboldTurbo\dllPath.bat

start cmd /C camonitor5.bat

call setrunning1 1 %1
timeout 10
call setrunning1 2 %1
timeout 10
call setrunning1 3 %1
timeout 10
call setrunning1 4 %1
timeout 10
call setrunning1 5 %1

