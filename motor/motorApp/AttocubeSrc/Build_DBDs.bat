SET TOPLEVEL=%1

if not exist %TOPLEVEL%\DBD md exist %TOPLEVEL%\DBD

xcopy /Y /Q /D /R  *.dbd  %TOPLEVEL%\dbd > NUL

echo BatchCompleted: %ERRORLEVEL%