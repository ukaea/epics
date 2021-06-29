SET TOPLEVEL=%1
SET BUILDIR=%2
SET DLLDIR=%3

if not exist O.Common md O.Common
if not exist %TOPLEVEL%\include md %TOPLEVEL%\include
if not exist %TOPLEVEL%\dbd md exist %TOPLEVEL%\dbd

xcopy /Y /Q /D /R  *.h  %TOPLEVEL%\include > NUL
xcopy /Y /Q /D /R  *.dbd  %TOPLEVEL%\dbd > NUL

echo BatchCompleted: %ERRORLEVEL%