@echo OFF

tasklist /fi "imagename eq cmd.exe" /v > tt01_tmp.txt
find "VTPIOC:TT01" tt01_tmp.txt > nul

if %errorlevel% == 0 (
title VTPIOC:ERROR
echo At least one other instance of VTPIOC TT01 is already running. Please close by typing exit in its command console, then try running this IOC again.
echo Press any key to exit this error message...
pause > nul
exit
) else (
title VTPIOC:TT01
..\..\bin\win32-x86-static\VarianTurboPump.exe ..\..\iocBoot\iocVarianTurboPump\tt01.cmd
title VTPIOC
)
