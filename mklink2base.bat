rmdir base

if "%1"=="" goto blank
mklink /D base %1
goto end
:blank
mklink /D base base-3.16.1
:end
