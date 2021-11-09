SET TOPLEVEL=%1
SET BUILDIR=%2
SET DLLDIR=%3

if not exist %TOPLEVEL%DBD  md %TOPLEVEL%DBD
if not exist %BUILDIR%O.Common  md %BUILDIR%O.Common

cl -C -E -nologo -D__STDC__=0 -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE -I.\O.Common -I%TOPLEVEL%include\compiler\msvc -I%TOPLEVEL%include\os\WIN32 -I%TOPLEVEL%include -I%TOPLEVEL%include\os\WIN32 XPS_trajectoryScan.st > O.Common\XPS_trajectoryScan.i
%DLLDIR%snc.exe O.Common\XPS_trajectoryScan.i -o O.Common\XPS_trajectoryScan.c

cl -C -E -nologo -D__STDC__=0 -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE -I.\O.Common -I%TOPLEVEL%include\compiler\msvc -I%TOPLEVEL%include\os\WIN32 -I%TOPLEVEL%include -I%TOPLEVEL%include\os\WIN32 MM4005_trajectoryScan.st > O.Common\MM4005_trajectoryScan.i
%DLLDIR%snc.exe O.Common\MM4005_trajectoryScan.i -o O.Common\MM4005_trajectoryScan.c

cl -C -E -nologo -D__STDC__=0 -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE -I.\O.Common -I%TOPLEVEL%include\compiler/msvc -I%TOPLEVEL%include\os\WIN32 -I%TOPLEVEL%include -I%TOPLEVEL%include\os\WIN32 xpsSlave.st > O.Common\xpsSlave.i
%DLLDIR%snc.exe O.Common\xpsSlave.i -o O.Common\xpsSlave.c

xcopy /Y /Q *.dbd  %TOPLEVEL%\DBD > NUL

echo BatchCompleted: %ERRORLEVEL%