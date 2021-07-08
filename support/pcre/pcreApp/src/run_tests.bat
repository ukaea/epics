setlocal
set "srcdir=%~dp0..\..\pcre-src"
set "pcretest=%~dp0..\..\bin\%EPICS_HOST_ARCH%\pcretest.exe"
call %srcdir%\RunTest.bat
%~dp0..\..\bin\%EPICS_HOST_ARCH%\pcre_jit_test.exe
%~dp0..\..\bin\%EPICS_HOST_ARCH%\pcrecpp_unittest.exe
%~dp0..\..\bin\%EPICS_HOST_ARCH%\pcre_scanner_unittest.exe
%~dp0..\..\bin\%EPICS_HOST_ARCH%\pcre_stringpiece_unittest.exe
@echo done
