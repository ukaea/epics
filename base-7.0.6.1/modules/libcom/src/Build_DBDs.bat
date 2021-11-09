SET TOPLEVEL=%1
SET BUILDIR=%2
SET DLLDIR=%3

SET EPICS_BUILD_COMPILER_CLASS=cl
SET EPICS_BUILD_OS_CLASS=win32
SET EPICS_BUILD_TARGET_ARCH=win32-x86

if not exist %TOPLEVEL%include  md %TOPLEVEL%include
if not exist %TOPLEVEL%include\os  md %TOPLEVEL%include\os
if not exist %TOPLEVEL%include\os\WIN32  md %TOPLEVEL%include\os\WIN32
if not exist %TOPLEVEL%include\valgrind  md %TOPLEVEL%include\valgrind

if not exist O.Common  md O.Common

xcopy /Y /Q as\*.h  %TOPLEVEL%include > NUL
xcopy /Y /Q misc\*.h  %TOPLEVEL%include > NUL
xcopy /Y /Q osi\*.h  %TOPLEVEL%include > NUL
xcopy /Y /Q env\*.h   %TOPLEVEL%include > NUL
xcopy /Y /Q iocsh\*.h  %TOPLEVEL%include > NUL
xcopy /Y /Q cppStd\*.h  %TOPLEVEL%include > NUL
xcopy /Y /Q error\*.h  %TOPLEVEL%include > NUL
xcopy /Y /Q ellLib\*.h  %TOPLEVEL%include > NUL
xcopy /Y /Q cxxTemplates\*.h  %TOPLEVEL%include > NUL
xcopy /Y /Q timer\*.h  %TOPLEVEL%include > NUL
xcopy /Y /Q freeList\*.h  %TOPLEVEL%include > NUL
xcopy /Y /Q cvtFast\*.h  %TOPLEVEL%include > NUL
xcopy /Y /Q taskwd\*.h  %TOPLEVEL%include > NUL
xcopy /Y /Q fdmgr\*.h  %TOPLEVEL%include > NUL
xcopy /Y /Q gpHash\*.h  %TOPLEVEL%include > NUL
xcopy /Y /Q bucketLib\*.h  %TOPLEVEL%include > NUL
xcopy /Y /Q dbmf\*.h  %TOPLEVEL%include > NUL
xcopy /Y /Q macLib\*.h  %TOPLEVEL%include > NUL
xcopy /Y /Q macLib\EPICS\*.pm  %TOPLEVEL%lib\perl\EPICS > NUL
xcopy /Y /Q calc\*.h  %TOPLEVEL%include > NUL
xcopy /Y /Q ring\*.h  %TOPLEVEL%include > NUL
xcopy /Y /Q log\*.h  %TOPLEVEL%include > NUL
xcopy /Y /Q yajl\*.h  %TOPLEVEL%include > NUL
xcopy /Y /Q pool\*.h  %TOPLEVEL%include > NUL
xcopy /Y /Q osi\os\default\*.h %TOPLEVEL%include\os\WIN32 > NUL
xcopy /Y /Q valgrind\*.h %TOPLEVEL%include\valgrind > NUL

REM required by bldEnvData.pl
xcopy /Y /Q ..\..\database\src\ioc\*.h %TOPLEVEL%include > NUL
xcopy /Y /Q ..\..\database\src\ioc\dbStatic\*.h %TOPLEVEL%include > NUL

REM conflict with osi\os\WIN32 header file versions.
del /F %TOPLEVEL%include\os\WIN32\osdWireConfig.h
del /F %TOPLEVEL%include\os\WIN32\epicsGetopt.h

xcopy /Y /Q osi\os\WIN32\*.h  %TOPLEVEL%include\os\WIN32 > NUL
xcopy /Y /Q osi\compiler\msvc\*.h  %TOPLEVEL%include\compiler\msvc\ > NUL

REM Copy convertRelease.pl to the DLL build folder
xcopy /Y /Q %TOPLEVEL%base-7.0.6.1\src\tools\convertRelease.pl %DLLDIR%

SET PERL5LIB=C:\Strawberry\lib\perl
if not exist C:\strawberry\perl\lib\DBD.pm  mklink /h C:\strawberry\perl\lib\DBD.pm %TOPLEVEL%lib\perl\DBD.pm
if not exist C:\strawberry\perl\lib\DBD  mklink /d C:\strawberry\perl\lib\DBD %TOPLEVEL%lib\perl\DBD
if not exist C:\strawberry\perl\lib\EPICS  mklink /d C:\strawberry\perl\lib\EPICS %TOPLEVEL%lib\perl\EPICS

cd O.Common
perl ..\env\bldEnvData.pl %TOPLEVEL%base-7.0.6.1\configure
perl ..\error\makeStatTbl.pl %TOPLEVEL%include\devLib.h %TOPLEVEL%include\asLib.h %TOPLEVEL%include\dbAccessDefs.h %TOPLEVEL%include\devSup.h
%DLLDIR%antelope.exe ..\as\aslib.y
move /Y %BUILDIR%O.Common\y.tab.c asLib.c
%DLLDIR%e_flex.exe -S%TOPLEVEL%include\flex.skel.static -8 -I ..\as\asLib_lex.l
move /Y %BUILDIR%O.Common\lex.yy.c asLib_lex.c
perl ..\misc\makeEpicsVersion.pl -o %TOPLEVEL%include\epicsVersion.h %TOPLEVEL%base-7.0.6.1\configure\CONFIG_BASE_VERSION

cd %BUILDIR%
REM perl -CSD %DLLDIR%convertRelease.pl -T %TOPLEVEL% %DLLDIR%\libComModuleDirs.pm


echo BatchCompleted: %ERRORLEVEL%