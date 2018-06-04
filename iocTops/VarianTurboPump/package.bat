@echo off

REM Copies VTP IOC deliverables to user-specified folder

set VTPPKGDIR=%1

if "%VTPPKGDIR%" == "" (
echo Usage: deliverables.bat PKG_DIR
echo Please specify home directory for VTP IOC package
echo Exiting
exit /B 1
)

if not exist %VTPPKGDIR%\ (
echo Cannot find folder %VTPPKGDIR%
echo Exiting
exit /B 1
)

set /p INP="Copy VTP IOC deliverables to folder %VTPPKGDIR% (Y/N)?"

set "TRUE="
if "%INP%" == "y" set TRUE=1
if "%INP%" == "Y" set TRUE=1
if defined TRUE (
echo Copying VTP IOC deliverables...
xcopy /D /S /I bin %VTPPKGDIR%\bin
xcopy /D /S /I db %VTPPKGDIR%\db
xcopy /D /S /I dbd %VTPPKGDIR%\dbd
xcopy /D /S /I iocBoot\iocVarianTurboPump %VTPPKGDIR%\iocBoot\iocVarianTurboPump
xcopy /D /S /I VTP %VTPPKGDIR%\VTP
xcopy /D ..\..\base\bin\win32-x86-static\caRepeater.exe %VTPPKGDIR%
)