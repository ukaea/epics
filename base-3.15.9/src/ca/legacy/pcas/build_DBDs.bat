SET TOPLEVEL=%1

if not exist  %TOPLEVEL%\include md  %TOPLEVEL%\include
xcopy /Y /Q /D /R generic\*.h  %TOPLEVEL%\include > NUL

echo BatchCompleted: %ERRORLEVEL%