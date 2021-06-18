if not exist DBD  md  %TOPLEVEL%\DBD
xcopy /Y /Q /D /R *.dbd  %TOPLEVEL%\dbd > NUL
if not exist %TOPLEVEL%include  md  %TOPLEVEL%\include
xcopy /Y /Q /D /R *.h  %TOPLEVEL\%include > NUL
