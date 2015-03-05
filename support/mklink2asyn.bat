rmdir asyn

if "%1"=="" goto blank
mklink /D asyn %1
goto end
:blank
mklink /D asyn asyn4-25
:end
