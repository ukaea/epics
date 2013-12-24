if "%EPICS_HOST_ARCH%"=="" goto setarch
goto continue

:setarch
set EPICS_HOST_ARCH=win32-x86

:continue

bin\%EPICS_HOST_ARCH%\mb_test.exe
