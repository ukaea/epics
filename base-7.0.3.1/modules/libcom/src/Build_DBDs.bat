SET TOPLEVEL=%1
SET BUILDIR=%2
SET DLLDIR=%3

if not exist %TOPLEVEL%\include  md %TOPLEVEL%\include
if not exist %TOPLEVEL%\include\os  md %TOPLEVEL%\include\os
if not exist %TOPLEVEL%\include\os\WIN32  md %TOPLEVEL%\include\os\WIN32
if not exist %TOPLEVEL%\include\valgrind  md %TOPLEVEL%\include\valgrind

if not exist O.Common  md O.Common

xcopy /Y /Q /D /R as\*.h  %TOPLEVEL%\include > NUL
xcopy /Y /Q /D /R misc\*.h  %TOPLEVEL%\include > NUL
xcopy /Y /Q /D /R osi\*.h  %TOPLEVEL%\include > NUL
xcopy /Y /Q /D /R env\*.h   %TOPLEVEL%\include > NUL
xcopy /Y /Q /D /R iocsh\*.h  %TOPLEVEL%\include > NUL
xcopy /Y /Q /D /R cppStd\*.h  %TOPLEVEL%\include > NUL
xcopy /Y /Q /D /R error\*.h  %TOPLEVEL%\include > NUL
xcopy /Y /Q /D /R ellLib\*.h  %TOPLEVEL%\include > NUL
xcopy /Y /Q /D /R cxxTemplates\*.h  %TOPLEVEL%\include > NUL
xcopy /Y /Q /D /R timer\*.h  %TOPLEVEL%\include > NUL
xcopy /Y /Q /D /R freeList\*.h  %TOPLEVEL%\include > NUL
xcopy /Y /Q /D /R cvtFast\*.h  %TOPLEVEL%\include > NUL
xcopy /Y /Q /D /R taskwd\*.h  %TOPLEVEL%\include > NUL
xcopy /Y /Q /D /R fdmgr\*.h  %TOPLEVEL%\include > NUL
xcopy /Y /Q /D /R gpHash\*.h  %TOPLEVEL%\include > NUL
xcopy /Y /Q /D /R bucketLib\*.h  %TOPLEVEL%\include > NUL
xcopy /Y /Q /D /R dbmf\*.h  %TOPLEVEL%\include > NUL
xcopy /Y /Q /D /R macLib\*.h  %TOPLEVEL%\include > NUL
xcopy /Y /Q /D /R macLib\EPICS\*.pm  %TOPLEVEL%\lib\perl\EPICS > NUL
xcopy /Y /Q /D /R calc\*.h  %TOPLEVEL%\include > NUL
xcopy /Y /Q /D /R ring\*.h  %TOPLEVEL%\include > NUL
xcopy /Y /Q /D /R log\*.h  %TOPLEVEL%\include > NUL
xcopy /Y /Q /D /R yajl\*.h  %TOPLEVEL%\include > NUL
xcopy /Y /Q /D /R pool\*.h  %TOPLEVEL%\include > NUL
xcopy /Y /Q /D /R osi\os\default\*.h %TOPLEVEL%\include\os\WIN32 > NUL
xcopy /Y /Q /D /R valgrind\*.h %TOPLEVEL%\include\valgrind > NUL

REM required by bldEnvData.pl
xcopy /Y /Q /D /R ..\..\database\src\ioc\*.h %TOPLEVEL%\include > NUL
xcopy /Y /Q /D /R ..\..\database\src\ioc\dbStatic\*.h %TOPLEVEL%\include > NUL

REM conflict with osi\os\WIN32 header file versions.
del %TOPLEVEL%\include\os\WIN32\osdWireConfig.h
del %TOPLEVEL%\include\os\WIN32\epicsGetopt.h

xcopy /Y /Q /D /R osi\os\WIN32\*.h  %TOPLEVEL%\include\os\WIN32 > NUL
xcopy /Y /Q /D /R osi\compiler\msvc\*.h  %TOPLEVEL%\include\compiler\msvc\ > NUL

REM Copy convertRelease.pl to the DLL build folder
xcopy /Y /Q /D /R %TARGETDIR%\base-7.0.3.1\src\tools\convertRelease.pl %DLLDIR%

SET PERL5LIB=C:\Strawberry\lib\perl
if not exist C:\strawberry\perl\lib\DBD.pm  mklink /h C:\strawberry\perl\lib\DBD.pm %TOPLEVEL%lib\perl\DBD.pm
if not exist C:\strawberry\perl\lib\DBD  mklink /d C:\strawberry\perl\lib\DBD %TOPLEVEL%lib\perl\DBD
if not exist C:\strawberry\perl\lib\EPICS  mklink /d C:\strawberry\perl\lib\EPICS %TOPLEVEL%lib\perl\EPICS

cd %TOPLEVEL%
perl -CSD %DLLDIR%\convertRelease.pl -T %TOPLEVEL% %DLLDIR%\libComModuleDirs.pm
cd %BUILDIR%

echo BatchCompleted: %ERRORLEVEL%