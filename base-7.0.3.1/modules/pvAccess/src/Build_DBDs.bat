SET TOPLEVEL=%1
SET BUILDIR=%2
SET DLLDIR=%3

if not exist %TOPLEVEL%\include md %TOPLEVEL%\include
if not exist %TOPLEVEL%\include\pv md %TOPLEVEL%\include\pv
if not exist %TOPLEVEL%\include\pva md %TOPLEVEL%\include\pva
if not exist %TOPLEVEL%\DBD md %TOPLEVEL%\DBD
xcopy /Y /Q /D /R ca\pv\*.h %TOPLEVEL%\include\pv > NUL
xcopy /Y /Q /D /R client\pv\*.h %TOPLEVEL%\include\pv > NUL
xcopy /Y /Q /D /R client\pva\*.h %TOPLEVEL%\include\pva > NUL
xcopy /Y /Q /D /R ioc\pv\*.h %TOPLEVEL%\include\pv > NUL
xcopy /Y /Q /D /R mb\pv\*.h %TOPLEVEL%\include\pv > NUL
xcopy /Y /Q /D /R pipelineService\pv\*.h %TOPLEVEL%\include\pv > NUL
xcopy /Y /Q /D /R pva\pv\*.h %TOPLEVEL%\include\pv > NUL
xcopy /Y /Q /D /R remote\pv\*.h %TOPLEVEL%\include\pv > NUL
xcopy /Y /Q /D /R remoteClient\pv\*.h %TOPLEVEL%\include\pv > NUL
xcopy /Y /Q /D /R rpcClient\pv\*.h %TOPLEVEL%\include\pv > NUL
xcopy /Y /Q /D /R rpcService\pv\*.h %TOPLEVEL%\include\pv > NUL
xcopy /Y /Q /D /R server\pv\*.h %TOPLEVEL%\include\pv > NUL
xcopy /Y /Q /D /R utils\pv\*.h %TOPLEVEL%\include\pv > NUL
xcopy /Y /Q /D /R ioc\*.dbd %TOPLEVEL%\DBD > NUL

echo BatchCompleted: %ERRORLEVEL%