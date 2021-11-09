SET TOPLEVEL=%1
SET BUILDIR=%2
SET DLLDIR=%3

if not exist %TOPLEVEL%include md %TOPLEVEL%include
if not exist %TOPLEVEL%dbd md %TOPLEVEL%dbd
xcopy /Y /Q *.h %TOPLEVEL%include > NUL
if not exist O.Common  md O.Common

echo BatchCompleted: %ERRORLEVEL%