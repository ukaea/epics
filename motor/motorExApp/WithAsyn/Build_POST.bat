SET TOPLEVEL=%1

xcopy /Y /Q  /D O.Common\*.dbd %TOPLEVEL%\dbd

echo BatchCompleted: %ERRORLEVEL%