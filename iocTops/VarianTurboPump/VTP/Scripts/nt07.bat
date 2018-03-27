@echo OFF

tasklist /fi "imagename eq cmd.exe" /v > nt07_tmp.txt
find "VTPIOC:NT07" nt07_tmp.txt > nul

if %errorlevel% == 0 (
title VTPIOC:ERROR
echo At least one other instance of VTPIOC NT07 is already running. Please close by typing exit in its command console, then try running this IOC again.
echo Press any key to exit this error message...
pause > nul
exit
) else (
title VTPIOC:NT07
..\..\bin\win32-x86-static\VarianTurboPump.exe ..\..\iocBoot\iocVarianTurboPump\nt07.cmd
title VTPIOC
)