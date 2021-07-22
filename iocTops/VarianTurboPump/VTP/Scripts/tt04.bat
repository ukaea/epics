@echo OFF

tasklist /fi "imagename eq cmd.exe" /v > tt04_tmp.txt
find "VTPIOC:TT04" tt04_tmp.txt > nul

if %errorlevel% == 0 (
title VTPIOC:ERROR
echo At least one other instance of VTPIOC TT04 is already running. Please close by typing exit in its command console, then try running this IOC again.
echo Press any key to exit this error message...
pause > nul
exit
) else (
title VTPIOC:TT04
..\..\bin\win32-x86-static\VarianTurboPump.exe ..\..\iocBoot\iocVarianTurboPump\tt04.cmd
title VTPIOC
)
