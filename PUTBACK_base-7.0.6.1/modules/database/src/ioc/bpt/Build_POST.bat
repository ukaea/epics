SET TOPLEVEL=%1
SET BUILDIR=%2
SET DLLDIR=%3

%DLLDIR%makeBpt.exe bptTypeJdegC.data  O.Common\bptTypeJdegC.dbd
%DLLDIR%makeBpt.exe bptTypeJdegF.data  O.Common\bptTypeJdegF.dbd
%DLLDIR%makeBpt.exe bptTypeKdegC.data  O.Common\bptTypeKdegC.dbd
%DLLDIR%makeBpt.exe bptTypeKdegF.data  O.Common\bptTypeKdegF.dbd
xcopy /Y /Q O.Common\menuConvert.h %TOPLEVEL%include > NUL
xcopy /Y /Q O.Common\*.dbd %TOPLEVEL%dbd > NUL

echo BatchCompleted: %ERRORLEVEL%