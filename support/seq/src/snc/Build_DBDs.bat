if not exist O.Common md O.Common
cd O.Common
%DLLDIR%\lemon.exe o=. ../snl.lem
C:\re2c\re2c.exe -s -b -o lexer.c ../snl.re
cd %BUILDIR%\


