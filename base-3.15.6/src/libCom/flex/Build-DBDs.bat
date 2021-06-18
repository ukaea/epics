if not exist %TOPLEVEL%\include md %TOPLEVEL%\include
if not exist O.Common md O.Common
xcopy /Y /Q /D /R flex.skel.static %TOPLEVEL%\include > NUL
