SET TOPLEVEL=%1
SET BUILDIR=%2
SET DLLDIR=%3

SET EPICS_BUILD_COMPILER_CLASS=cl
SET EPICS_BUILD_OS_CLASS=win32
SET EPICS_BUILD_TARGET_ARCH=win32-x86

if not exist  %TOPLEVEL%include md  %TOPLEVEL%include
if not exist  %TOPLEVEL%include\os md  %TOPLEVEL%include\os
if not exist  %TOPLEVEL%include\os\WIN32 md  %TOPLEVEL%include\os\WIN32
if not exist O.Common md O.Common

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
xcopy /Y /Q calc\*.h  %TOPLEVEL%include > NUL
xcopy /Y /Q ring\*.h  %TOPLEVEL%include > NUL
xcopy /Y /Q log\*.h  %TOPLEVEL%include > NUL
xcopy /Y /Q yajl\*.h  %TOPLEVEL%include > NUL
xcopy /Y /Q pool\*.h  %TOPLEVEL%include > NUL
xcopy /Y /Q osi\os\default\*.h %TOPLEVEL%include\os\WIN32 > NUL

REM required by bldEnvData.pl
xcopy /Y /Q ..\ca\legacy\gdd\*.h %TOPLEVEL%include > NUL
xcopy /Y /Q ..\ca\legacy\pcas\generic\*.h %TOPLEVEL%include > NUL
xcopy /Y /Q ..\ioc\db\*.h %TOPLEVEL%include > NUL
xcopy /Y /Q ..\ioc\dbStatic\*.h %TOPLEVEL%include > NUL

REM conflict with osi\os\WIN32 header file versions.
del  %TOPLEVEL%include\os\WIN32\osdWireConfig.h
del  %TOPLEVEL%include\os\WIN32\epicsGetopt.h

xcopy /Y /Q osi\os\WIN32\*.h  %TOPLEVEL%include\os\WIN32 > NUL
xcopy /Y /Q osi\compiler\msvc\*.h  %TOPLEVEL%include\compiler\msvc\ > NUL

echo BatchCompleted: %ERRORLEVEL%