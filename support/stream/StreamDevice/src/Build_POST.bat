SET TOPLEVEL=%1

xcopy /Y /Q /D /R  O.Common\*.dbd  %TOPLEVEL%\include > NUL

echo BatchCompleted: %ERRORLEVEL%