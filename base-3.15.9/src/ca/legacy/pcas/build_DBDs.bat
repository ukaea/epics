SET TOPLEVEL=%1

if not exist  %TOPLEVEL%include  md %TOPLEVEL%include
xcopy /Y /Q generic\*.h %TOPLEVEL%include > NUL

echo BatchCompleted: %ERRORLEVEL%