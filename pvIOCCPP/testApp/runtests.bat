if "%EPICS_HOST_ARCH%"=="" goto setarch
goto continue

:setarch
set EPICS_HOST_ARCH=win32-x86

:continue

start %~dp0..\bin\%EPICS_HOST_ARCH%\exampleServiceRPC.exe
%~dp0..\bin\%EPICS_HOST_ARCH%\exampleClient.exe
