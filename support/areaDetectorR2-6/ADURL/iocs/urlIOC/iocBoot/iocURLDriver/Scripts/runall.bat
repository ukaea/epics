call ..\..\..\..\..\..\..\..\base\startup\win32.bat x86
pushd ..
call start_epics.bat
popd
timeout 30

call ..\..\..\..\..\..\..\..\base\startup\win32.bat x86-DLL
pushd ..
call start_epics.bat
popd
timeout 30

call ..\..\..\..\..\..\..\..\base\startup\win32.bat x86-debug-DLL
pushd ..
call start_epics.bat
popd
timeout 30

call ..\..\..\..\..\..\..\..\base\startup\win32.bat x64
pushd ..
call start_epics.bat
popd
timeout 30

call ..\..\..\..\..\..\..\..\base\startup\win32.bat x64-DLL
pushd ..
call start_epics.bat
popd
timeout 30

call ..\..\..\..\..\..\..\..\base\startup\win32.bat x64-debug-DLL
pushd ..
call start_epics.bat
popd
