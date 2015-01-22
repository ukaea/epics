@ECHO OFF
IF NOT "%LEYBOLD_PATH_ADDED%"=="" GOTO already_added
PATH %PATH%;D:/EPICS/LeyboldTurbo/bin/win32-x86;D:/EPICS/asyn/bin/win32-x86;D:/EPICS/base/bin/win32-x86
set LEYBOLD_PATH_ADDED=1
:already_added
