SET TOPLEVEL=%1
SET BUILDIR=%2
SET DLLDIR=%3

if not exist O.Common md O.Common
if not exist %TOPLEVEL%\dbd md %TOPLEVEL%\dbd
xcopy /Y /Q /D /R *.dbd %TOPLEVEL%\dbd > NUL
xcopy /Y /Q /D /R dev\*.dbd %TOPLEVEL%\dbd > NUL
xcopy /Y /Q /D /R rec\*.dbd %TOPLEVEL%\dbd > NUL

echo BatchCompleted: %ERRORLEVEL%