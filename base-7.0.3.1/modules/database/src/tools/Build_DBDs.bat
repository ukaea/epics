SET TOPLEVEL=%1
SET BUILDIR=%2
SET DLLDIR=%3

if not exist %TOPLEVEL%\lib md %TOPLEVEL%\lib
if not exist %TOPLEVEL%\lib\perl md %TOPLEVEL%\lib\perl
if not exist %TOPLEVEL%\lib\perl\DBD md %TOPLEVEL%\lib\perl\DBD
if not exist %TOPLEVEL%\lib\perl\EPICS md %TOPLEVEL%\lib\perl\EPICS

xcopy /Y /Q /D /R %BUILDIR%*.pl %DLLDIR% > NUL
xcopy /Y /Q /D /R %BUILDIR%*.pm %TOPLEVEL%lib\perl > NUL
xcopy /Y /Q /D /R %BUILDIR%DBD\*.pm %TOPLEVEL%lib\perl\DBD > NUL
xcopy /Y /Q /D /R %BUILDIR%EPICS\*.pm %TOPLEVEL%lib\perl\EPICS > NUL

echo BatchCompleted: %ERRORLEVEL%