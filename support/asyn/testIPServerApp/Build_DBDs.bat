SET TOPLEVEL=%1
SET BUILDIR=%2
SET DLLDIR=%3

if not exist O.Common md O.Common

cd O.Common
cl /C /E /nologo /D__STDC__=0 /D_CRT_SECURE_NO_DEPRECATE /D_CRT_NONSTDC_NO_DEPRECATE -I. -I..\O.Common -I. -I.. -I..\..\..\include\os\WIN32 -I..\..\..\include -I\EPICS\support\ipac\include -I\EPICS\support\seq\include -I\EPICS\base\include\os\WIN32 -I\EPICS\base\Include ..\src\ipSNCServer.st > ipSNCServer.i
%DLLDIR%snc.exe ipSNCServer.i -o ipSNCServer.c.tmp
cd %BUILDIR%

echo BatchCompleted: %ERRORLEVEL%