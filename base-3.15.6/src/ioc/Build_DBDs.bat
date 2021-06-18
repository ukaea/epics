if not exist  %TOPLEVEL%\include md  %TOPLEVEL%\include
if not exist  %TOPLEVEL%\DBD md  %TOPLEVEL%\DBD

xcopy /Y /Q /D /R as\*.h  %TOPLEVEL%\include > NUL
xcopy /Y /Q /D /R bpt\*.h  %TOPLEVEL%\include > NUL
xcopy /Y /Q /D /R db\*.h  %TOPLEVEL%\include > NUL
xcopy /Y /Q /D /R dbStatic\*.h  %TOPLEVEL%\include > NUL
xcopy /Y /Q /D /R dbtemplate\*.h  %TOPLEVEL%\include > NUL
xcopy /Y /Q /D /R misc\*.h  %TOPLEVEL%\include > NUL
xcopy /Y /Q /D /R registry\*.h  %TOPLEVEL%\include > NUL
xcopy /Y /Q /D /R rsrv\*.h  %TOPLEVEL%\include > NUL

xcopy /Y /Q /D /R db\*.dbd  %TOPLEVEL%\dbd > NUL
xcopy /Y /Q /D /R misc\*.dbd  %TOPLEVEL%\dbd > NUL
