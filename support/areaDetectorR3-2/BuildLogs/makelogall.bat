pushd ..\..\..\base\startup
call win32.bat x86
popd
call makelog.bat
timeout 30

pushd ..\..\..\base\startup
call win32.bat x86-DLL
popd
call makelog.bat
timeout 30

pushd ..\..\..\base\startup
call win32.bat x86-debug-DLL
popd
call makelog.bat
timeout 30

pushd ..\..\..\base\startup
call win32.bat x64
popd
call makelog.bat
timeout 30

pushd ..\..\..\base\startup
call win32.bat x64-DLL
popd
call makelog.bat
timeout 30

pushd ..\..\..\base\startup
call win32.bat x64-debug-DLL
popd
call makelog.bat
