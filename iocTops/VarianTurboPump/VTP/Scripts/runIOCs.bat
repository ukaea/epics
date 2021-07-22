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

REM Removed running of VTP IOCs for pumps NT07 and NT10
REM because these Agilent turbopumps now replaced by
REM Edwards turbopumps. (MHC 02 Jul 2018).

REM find "VTPIOC:NT07" runIOCs_tmp.txt > nul
REM if not %errorlevel% == 0 (
REM start nt07.bat
REM timeout 2 > nul
REM )

REM find "VTPIOC:NT10" runIOCs_tmp.txt > nul
REM if not %errorlevel% == 0 (
REM start nt10.bat
REM timeout 2 > nul
REM )

exit
