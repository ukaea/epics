if "%EPICS_HOST_ARCH%"=="" goto setarch
goto continue

:setarch
set EPICS_HOST_ARCH=win32-x86

:continue

start ..\bin\%EPICS_HOST_ARCH%\exampleServiceRPC.exe
..\bin\%EPICS_HOST_ARCH%\exampleClient.exe
