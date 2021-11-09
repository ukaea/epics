SET TOPLEVEL=%1

if not exist %TOPLEVEL%DBD  md %TOPLEVEL%DBD

xcopy /Y /Q O.Common\*.DBD %TOPLEVEL%DBD > NUL

echo BatchCompleted: %ERRORLEVEL%