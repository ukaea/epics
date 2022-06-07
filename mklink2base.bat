rmdir base

if "%1"=="" goto blank
mklink /D base %1
goto end
:blank
mklink /D base base-7.0.6.1
REM mklink /D configure base-7.0.6\configure
:end
