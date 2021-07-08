SET TOPLEVEL=%1

if not exist %TOPLEVEL%\DBD md exist %TOPLEVEL%\DBD

xcopy /Y /Q /D /R  *.dbd  %TOPLEVEL%\DBD > NUL

echo BatchCompleted: %ERRORLEVEL%