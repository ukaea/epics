SET TOPLEVEL=%1

xcopy /Y /Q /D /R O.Common\*.DBD %TOPLEVEL%\DBD > NUL

echo BatchCompleted: %ERRORLEVEL%