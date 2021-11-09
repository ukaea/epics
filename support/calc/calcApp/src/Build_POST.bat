SET TOPLEVEL=%1

xcopy /Y /Q O.Common\*.h %TOPLEVEL%include > NUL
xcopy /Y /Q O.Common\*.dbd %TOPLEVEL%dbd > NUL

echo BatchCompleted: %ERRORLEVEL%