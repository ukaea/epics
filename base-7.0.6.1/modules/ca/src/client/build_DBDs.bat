SET TOPLEVEL=%1
SET BUILDIR=%2
SET DLLDIR=%3

if not exist  %TOPLEVEL%\include md  %TOPLEVEL%\include
xcopy /Y /Q  /D /R *.h %TOPLEVEL%\include > NUL
echo BatchCompleted: %ERRORLEVEL%