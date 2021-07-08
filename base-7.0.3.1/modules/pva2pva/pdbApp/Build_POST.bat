SET TOPLEVEL=%1

if not exist %TOPLEVEL%DBD  md %TOPLEVEL%DBD
xcopy /Y /Q /D O.Common\*.dbd %TOPLEVEL%DBD > NUL

echo BatchCompleted: %ERRORLEVEL%