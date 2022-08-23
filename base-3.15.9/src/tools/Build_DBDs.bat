SET TOPLEVEL=%1
SET BUILDIR=%2
SET DLLDIR=%3

if not exist %TOPLEVEL%lib  md %TOPLEVEL%lib
if not exist %TOPLEVEL%lib\perl  md %TOPLEVEL%lib\perl
if not exist %TOPLEVEL%lib\perl\DBD  md %TOPLEVEL%lib\perl\DBD
if not exist %TOPLEVEL%lib\perl\EPICS  md %TOPLEVEL%lib\perl\EPICS

xcopy /Y /Q *.pl %DLLDIR% > NUL
xcopy /Y /Q *.pl C:\Strawberry\perl\site\bin > NUL
xcopy /Y /Q *.pm %TOPLEVEL%lib\perl > NUL
xcopy /Y /Q DBD\*.pm %TOPLEVEL%lib\perl\DBD > NUL
xcopy /Y /Q EPICS\*.pm %TOPLEVEL%lib\perl\EPICS > NUL

echo BatchCompleted: %ERRORLEVEL%