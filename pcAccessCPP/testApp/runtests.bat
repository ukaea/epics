if "%EPICS_HOST_ARCH%"=="" goto setarch
goto continue

:setarch
set EPICS_HOST_ARCH=win32-x86

:continue

REM client
..\bin\%EPICS_HOST_ARCH%\testCreateRequest.exe
..\bin\%EPICS_HOST_ARCH%\testStartStop.exe

REM remote
..\bin\%EPICS_HOST_ARCH%\testGetPerformance.exe
start ..\bin\%EPICS_HOST_ARCH%\testBlockingTCPSrv.exe
..\bin\%EPICS_HOST_ARCH%\testBlockingTCPClnt.exe
start ..\bin\%EPICS_HOST_ARCH%\testBlockingUDPSrv.exe
..\bin\%EPICS_HOST_ARCH%\testBlockingUDPClnt.exe
start ..\bin\%EPICS_HOST_ARCH%\testServer.exe
..\bin\%EPICS_HOST_ARCH%\testChannelConnect.exe
..\bin\%EPICS_HOST_ARCH%\testRemoteClientImpl.exe
..\bin\%EPICS_HOST_ARCH%\testServerContext.exe

REM utils
..\bin\%EPICS_HOST_ARCH%\testAtomicBoolean.exe
..\bin\%EPICS_HOST_ARCH%\testHexDump.exe
..\bin\%EPICS_HOST_ARCH%\testInetAddressUtils.exe
