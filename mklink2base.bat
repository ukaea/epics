rmdir base

if "%1"=="" goto blank
mklink /D base %1
goto end
:blank
mklink /D base base-3.15.2
:end
