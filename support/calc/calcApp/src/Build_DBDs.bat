if not exist O.Common md O.Common
if not exist %TOPLEVEL%\include md %TOPLEVEL%\include
if not exist %TOPLEVEL%\dbd md exist %TOPLEVEL%\dbd

xcopy /Y /Q /D /R  *.dbd  %TOPLEVEL%\dbd > NUL