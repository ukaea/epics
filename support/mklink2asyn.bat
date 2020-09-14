rmdir asyn

SET A_V=%1%
if "%A_V%"=="" SET A_V=-R4-40-1
mklink /D asyn asyn%A_V%
