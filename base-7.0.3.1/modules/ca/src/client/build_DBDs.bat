if not exist  %TOPLEVEL%\include md  %TOPLEVEL%\include
xcopy /Y /Q  /D /R *.h %TOPLEVEL%\include > NUL
attrib -r %DLLDIR%\ca.lib
attrib -r %DLLDIR%\ca.exp
