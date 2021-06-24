SET TOPLEVEL=%1

xcopy /Y /Q /D /R O.Common\*.h  %TOPLEVEL%\include &gt; NUL

echo BatchCompleted: %ERRORLEVEL%