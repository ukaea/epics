SET TOPLEVEL=%1
SET BUILDIR=%2
SET DLLDIR=%3

if not exist O.Common md O.Common
cd O.Common

%DLLDIR%\lemon.exe o=. ../snl.lem
%TOPLEVEL%\support\seq\re2c.exe -s -b -o lexer.c ../snl.re
cd %BUILDIR%\

echo BatchCompleted: %ERRORLEVEL%