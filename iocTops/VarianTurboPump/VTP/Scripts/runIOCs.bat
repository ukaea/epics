@echo OFF

REM Batch file to run multiple VTP IOCs
REM NB: Relies on the individual batch files <vtp-ioc-instance>.bat setting the titles of their respective consoles accordingly.

echo Starting IOCs...

tasklist /fi "imagename eq cmd.exe" /v > runIOCS_tmp.txt

find "VTPIOC:TT01" runIOCs_tmp.txt > nul
if not %errorlevel% == 0 (
start tt01.bat
timeout 2 > nul
)

find "VTPIOC:TT02" runIOCs_tmp.txt > nul
if not %errorlevel% == 0 (
start tt02.bat
timeout 2 > nul
)

find "VTPIOC:TT03" runIOCs_tmp.txt > nul
if not %errorlevel% == 0 (
start tt03.bat
timeout 2 > nul
)

find "VTPIOC:TT04" runIOCs_tmp.txt > nul
if not %errorlevel% == 0 (
start tt04.bat
timeout 2 > nul
)

find "VTPIOC:NT07" runIOCs_tmp.txt > nul
if not %errorlevel% == 0 (
start nt07.bat
timeout 2 > nul
)

find "VTPIOC:NT10" runIOCs_tmp.txt > nul
if not %errorlevel% == 0 (
start nt10.bat
timeout 2 > nul
)

exit
