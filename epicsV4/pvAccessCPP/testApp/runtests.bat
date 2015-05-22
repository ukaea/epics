if "%EPICS_HOST_ARCH%"=="" goto setarch
goto continue

:setarch
set EPICS_HOST_ARCH=win32-x86

:continue

REM client
%~dp0..\bin\%EPICS_HOST_ARCH%\testCreateRequest.exe
%~dp0..\bin\%EPICS_HOST_ARCH%\testStartStop.exe

REM remote
%~dp0..\bin\%EPICS_HOST_ARCH%\testGetPerformance.exe
start %~dp0..\bin\%EPICS_HOST_ARCH%\testBlockingTCPSrv.exe
%~dp0..\bin\%EPICS_HOST_ARCH%\testBlockingTCPClnt.exe
start %~dp0..\bin\%EPICS_HOST_ARCH%\testBlockingUDPSrv.exe
%~dp0..\bin\%EPICS_HOST_ARCH%\testBlockingUDPClnt.exe
start %~dp0..\bin\%EPICS_HOST_ARCH%\testServer.exe
%~dp0..\bin\%EPICS_HOST_ARCH%\testChannelConnect.exe
%~dp0..\bin\%EPICS_HOST_ARCH%\testRemoteClientImpl.exe
%~dp0..\bin\%EPICS_HOST_ARCH%\testServerContext.exe

REM utils
%~dp0..\bin\%EPICS_HOST_ARCH%\testAtomicBoolean.exe
%~dp0..\bin\%EPICS_HOST_ARCH%\testHexDump.exe
%~dp0..\bin\%EPICS_HOST_ARCH%\testInetAddressUtils.exe
