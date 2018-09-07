set TOP=/epics/support/areaDetector/ADURL/iocs/urlIOC
set EPICS_HOST_ARCH=win32-x86-static
pushd ..
call start_epics.bat
popd
timeout 30

set EPICS_HOST_ARCH=win32-x86
pushd ..
call start_epics.bat
popd
timeout 30

set EPICS_HOST_ARCH=win32-x86-debug
pushd ..
call start_epics.bat
popd
timeout 30

set EPICS_HOST_ARCH=win32-x86-debug-static
pushd ..
call start_epics.bat
popd
timeout 30

set EPICS_HOST_ARCH=windows-x64
pushd ..
call start_epics.bat
popd
timeout 30

set EPICS_HOST_ARCH=windows-x64-debug
pushd ..
call start_epics.bat
popd

