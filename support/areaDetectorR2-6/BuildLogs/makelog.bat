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

pushd %~dp0..
call make.bat clean uninstall > NUL
call make.bat > %~dp0areaDetectorR-2-6.%EPICS_HOST_ARCH%.log 2>&1
popd
