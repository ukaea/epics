rmdir asyn

SET A_V=%1%
if "%A_V%"=="" SET A_V=4-26
mklink /D asyn asyn%A_V%
SETX ASYN_VER %A_V%
