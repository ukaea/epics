SET TOPLEVEL=%1

xcopy /Y /Q O.Common\*.DBD %TOPLEVEL%DBD > NUL

echo BatchCompleted: %ERRORLEVEL%