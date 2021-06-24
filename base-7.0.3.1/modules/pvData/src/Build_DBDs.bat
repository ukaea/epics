SET TOPLEVEL=%1

if not exist %TOPLEVEL%\include md %TOPLEVEL%\include
if not exist %TOPLEVEL%\include\pv md %TOPLEVEL%\include\pv
xcopy /Y /Q /D /R copy\pv\*.h %TOPLEVEL%\include\pv > NUL
xcopy /Y /Q /D /R json\pv\*.h %TOPLEVEL%\include\pv > NUL
xcopy /Y /Q /D /R factory\pv\*.h %TOPLEVEL%\include\pv > NUL
xcopy /Y /Q /D /R misc\pv\*.h %TOPLEVEL%\include\pv > NUL
xcopy /Y /Q /D /R property\pv\*.h %TOPLEVEL%\include\pv > NUL
xcopy /Y /Q /D /R pv\*.h %TOPLEVEL%\include\pv > NUL
xcopy /Y /Q /D /R pvMisc\pv\*.h %TOPLEVEL%\include\pv > NUL

echo BatchCompleted: %ERRORLEVEL%