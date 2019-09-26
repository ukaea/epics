rmdir base

if "%1"=="" goto blank
mklink /D base %1
goto end
:blank
mklink /D base base-7.0.3
mklink /D configure base-7.0.3\configure
:end
