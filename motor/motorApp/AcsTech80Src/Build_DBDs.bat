SET TOPLEVEL=%1
SET BUILDIR=%2
SET DLLDIR=%3

if not exist %TOPLEVEL%DBD  md %TOPLEVEL%DBD
if not exist %BUILDIR%O.Common  md %BUILDIR%O.Common

xcopy /Y /Q *.dbd %TOPLEVEL%DBD > NUL

echo BatchCompleted: %ERRORLEVEL%