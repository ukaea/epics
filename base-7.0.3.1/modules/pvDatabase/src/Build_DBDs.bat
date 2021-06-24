SET TOPLEVEL=%1
SET BUILDIR=%2
SET DLLDIR=%3

if not exist %TOPLEVEL%\include md %TOPLEVEL%\include
if not exist %TOPLEVEL%\include\pv md %TOPLEVEL%\include\pv
xcopy /Y /Q /D /R pv\*.h %TOPLEVEL%\include\pv > NUL

echo BatchCompleted: %ERRORLEVEL%