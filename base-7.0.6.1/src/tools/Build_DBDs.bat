SET TOPLEVEL=%1
SET BUILDIR=%2
SET DLLDIR=%3

if not exist %TOPLEVEL%configure  mklink /D %TOPLEVEL%configure \epics\base-7.0.3.1\configure
if not exist %TOPLEVEL%lib\perl\EPICS  md %TOPLEVEL%lib\perl\EPICS
if not exist %TOPLEVEL%lib  md %TOPLEVEL%lib
if not exist %TOPLEVEL%lib\perl  md %TOPLEVEL%lib\perl

xcopy /Y /Q *.pl %DLLDIR% > NUL
xcopy /Y /Q EPICS\*.pm %TOPLEVEL%lib\perl\EPICS > NUL

echo BatchCompleted: %ERRORLEVEL%