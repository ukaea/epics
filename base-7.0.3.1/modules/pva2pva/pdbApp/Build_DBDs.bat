SET TOPLEVEL=%1
SET BUILDIR=%2
SET DLLDIR=%3

if not exist %TOPLEVEL%DBD  md exist %TOPLEVEL%DBD
if not exist %TOPLEVEL%include  md %TOPLEVEL%include

SET PERL5LIB=C:\Strawberry\lib\perl
perl -CSD %DLLDIR%expandVars.pl -DEPICS_QSRV_MAJOR_VERSION="1" -DEPICS_QSRV_MINOR_VERSION="3" -DEPICS_QSRV_MAINTENANCE_VERSION="1" -DEPICS_QSRV_DEVELOPMENT_FLAG="0" -DEPICS_QSRV_ABI_MAJOR_VERSION="1" -DEPICS_QSRV_ABI_MINOR_VERSION="1" -t %DLLDIR% pv/qsrvVersionNum.h O.Common/pv/qsrvVersionNum.h
xcopy qsrv-new.dbd %TOPLEVEL%DBD/qsrv.dbd

xcopy /Y /Q /D /R *.h %TOPLEVEL%include > NUL
xcopy /Y /Q /D /R pv\*.h %TOPLEVEL%include > NUL

echo BatchCompleted: %ERRORLEVEL%