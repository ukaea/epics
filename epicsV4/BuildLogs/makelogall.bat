call ..\..\base\startup\win32.bat x86
call makelog.bat
timeout 30

call ..\..\base\startup\win32.bat x86-DLL
call makelog.bat
timeout 30

call ..\..\base\startup\win32.bat x86-debug-DLL
call makelog.bat
timeout 30

call ..\..\base\startup\win32.bat x64
call makelog.bat
timeout 30

call ..\..\base\startup\win32.bat x64-DLL
call makelog.bat
timeout 30

call ..\..\base\startup\win32.bat x64-debug-DLL
call makelog.bat
