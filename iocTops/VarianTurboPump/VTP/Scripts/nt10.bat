@echo OFF

tasklist /fi "imagename eq cmd.exe" /v > nt10_tmp.txt
find "VTPIOC:NT10" nt10_tmp.txt > nul

if %errorlevel% == 0 (
title VTPIOC:ERROR
echo At least one other instance of VTPIOC NT10 is already running. Please close by typing exit in its command console, then try running this IOC again.
echo Press any key to exit this error message...
pause > nul
exit
) else (
title VTPIOC:NT10
..\..\bin\win32-x86-static\VarianTurboPump.exe ..\..\iocBoot\iocVarianTurboPump\nt10.cmd
title VTPIOC
)