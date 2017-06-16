if "%EPICS_HOST_ARCH%"=="" (
	call %~dp0\..\..\base\startup\win32.bat x86-debug-DLL
)

pushd %~dp0..\..\base
make
popd

pushd %~dp0..
make.exe clean > NUL
make.exe  -j4 -O EPICS_BASE=\epics/base > BuildLogs\EPICSV4.4.6.%EPICS_HOST_ARCH%.log 2>&1
popd
