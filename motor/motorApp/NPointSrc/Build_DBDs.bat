SET TOPLEVEL=%1

if not exist %TOPLEVEL%DBD  md %TOPLEVEL%DBD

xcopy /Y /Q *.dbd %TOPLEVEL%dbd > NUL

echo BatchCompleted: %ERRORLEVEL%