if not exist  %TOPLEVEL%\include md  %TOPLEVEL%\include
if not exist  %TOPLEVEL%\dbd md  %TOPLEVEL%\dbd
if not exist O.Common md O.Common
xcopy /Y /Q /D /R .\asynDriver\*.h  %TOPLEVEL%\include > NUL
xcopy /Y /Q /D /R .\asynPortDriver\*.h  %TOPLEVEL%\include > NUL
xcopy /Y /Q /D /R .\asynGpib\*.h  %TOPLEVEL%\include > NUL
xcopy /Y /Q /D /R .\devEpics\*.h   %TOPLEVEL%\include > NUL
xcopy /Y /Q /D /R .\interfaces\*.h  %TOPLEVEL%\include > NUL
xcopy /Y /Q /D /R .\miscellaneous\*.h  %TOPLEVEL%\include > NUL
xcopy /Y /Q /D /R .\drvAsynSerial\*.h %TOPLEVEL%\include > NUL
xcopy /Y /Q /D /R .\devGpib\*.h %TOPLEVEL%\include > NUL
xcopy /Y /Q /D /R .\asynPortDriver\exceptions %TOPLEVEL%\include > NUL
xcopy /Y /Q /D /R .\drvAsynSerial\*.dbd %TOPLEVEL%\dbd > NUL
xcopy /Y /Q /D /R .\miscellaneous\*.dbd  %TOPLEVEL%\dbd > NUL
xcopy /Y /Q /D /R .\asynRecord\*.dbd  %TOPLEVEL%\dbd > NUL
xcopy /Y /Q /D /R .\devEpics\*.dbd   %TOPLEVEL%\dbd > NUL
