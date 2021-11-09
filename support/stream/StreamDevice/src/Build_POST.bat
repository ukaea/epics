SET TOPLEVEL=%1

xcopy /Y /Q O.Common\*.dbd %TOPLEVEL%include > NUL

echo BatchCompleted: %ERRORLEVEL%