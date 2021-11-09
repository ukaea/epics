SET TOPLEVEL=%1
SET BUILDIR=%2
SET DLLDIR=%3

if not exist %TOPLEVEL%include  md %TOPLEVEL%include
if not exist O.Common md O.Common
xcopy /Y /Q flex.skel.static %TOPLEVEL%include > NUL

echo BatchCompleted: %ERRORLEVEL%