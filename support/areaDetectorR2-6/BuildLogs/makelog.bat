if "%EPICS_HOST_ARCH%"=="" (
	call %~dp0\..\..\..\base\startup\win32.bat x86-debug-DLL
)

pushd %~dp0..\..\..\base
make
popd

pushd %~dp0..\..\ipac
call make.bat
popd

pushd %~dp0..\..\seq
call make.bat
popd

pushd %~dp0..\..\asyn
call make.bat
popd

pushd %~dp0..\..\sscan
call make.bat
popd

pushd %~dp0..\..\calc
call make.bat
popd

pushd %~dp0..\..\busy
call make.bat
popd

pushd %~dp0..\..\autosave
call make.bat
popd

pushd %~dp0..\..\..\epicsV4
make.exe  -j4 -O EPICS_BASE=\epics/base
popd

pushd %~dp0..
call make.bat clean uninstall > NUL
call make.bat > %~dp0areaDetectorR-2-6.%EPICS_HOST_ARCH%.log 2>&1
popd
