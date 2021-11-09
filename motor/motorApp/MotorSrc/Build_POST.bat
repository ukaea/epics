SET TOPLEVEL=%1

xcopy /Y /Q O.Common\*.h  %TOPLEVEL%include > NUL

echo BatchCompleted: %ERRORLEVEL%