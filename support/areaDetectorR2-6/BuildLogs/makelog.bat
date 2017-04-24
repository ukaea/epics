pushd %~dp0..
call make.bat clean uninstall > NUL
call make.bat > %~dp0areaDetectorR-2-6.%EPICS_HOST_ARCH%.log 2>&1
popd
