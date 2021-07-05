SET TOPLEVEL=%1

xcopy /Y /Q /D /R O.Common\*.h  %TOPLEVEL%include > NUL
xcopy /Y /Q /D /R O.Common\*.dbd  %TOPLEVEL%dbd > NUL

echo BatchCompleted: %ERRORLEVEL%