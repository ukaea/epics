SET TOPLEVEL=C:\Users\ubx69493\source\repos\epics.dotnet\
xcopy /Y /Q /D /R O.Common\*.h  %TOPLEVEL%include > NUL

echo BatchCompleted: %ERRORLEVEL%