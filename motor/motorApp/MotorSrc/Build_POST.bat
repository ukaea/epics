SET TOPLEVEL=%1

xcopy /Y /Q /D /R O.Common\*.h  %TOPLEVEL%\include > NUL

echo BatchCompleted: %ERRORLEVEL%