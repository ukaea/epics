if "%EPICS_HOST_ARCH%"=="" (
	call %~dp0\..\..\..\base\startup\win32.bat x86-DLL
)

pushd %~dp0..\..\..\base
call make.bat  > %~dp0base.%EPICS_HOST_ARCH%.log 2>&1
popd

pushd %~dp0..\..\ipac
call make.bat > %~dp0ipac.%EPICS_HOST_ARCH%.log 2>&1
popd

pushd %~dp0..\..\seq
call make.bat > %~dp0seq.%EPICS_HOST_ARCH%.log 2>&1
popd

pushd %~dp0..\..\asyn
call make.bat > %~dp0asyn.%EPICS_HOST_ARCH%.log 2>&1
popd

pushd %~dp0..\..\sscan
call make.bat > %~dp0sscan.%EPICS_HOST_ARCH%.log 2>&1
popd

pushd %~dp0..\..\calc
call make.bat > %~dp0calc.%EPICS_HOST_ARCH%.log 2>&1
popd

pushd %~dp0..\..\busy
call make.bat > %~dp0busy.%EPICS_HOST_ARCH%.log 2>&1
popd

pushd %~dp0..\..\autosave
call make.bat  > %~dp0autosave.%EPICS_HOST_ARCH%.log 2>&1
popd

pushd %~dp0..
call make.bat clean uninstall > NUL
call make.bat > %~dp0areaDetectorR-3-2.%EPICS_HOST_ARCH%.log 2>&1
popd
