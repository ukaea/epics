SET TOPLEVEL=%1
SET BUILDIR=%2
SET DLLDIR=%3

if not exist %TOPLEVEL%\include  md %TOPLEVEL%\include
if not exist %TOPLEVEL%\DBD  md %TOPLEVEL%\DBD

xcopy /Y /Q /D /R as\*.h  %TOPLEVEL%include > NUL
xcopy /Y /Q /D /R bpt\*.h  %TOPLEVEL%include > NUL
xcopy /Y /Q /D /R db\*.h  %TOPLEVEL%include > NUL
xcopy /Y /Q /D /R dbStatic\*.h  %TOPLEVEL%include > NUL
xcopy /Y /Q /D /R dbtemplate\*.h  %TOPLEVEL%include > NUL
xcopy /Y /Q /D /R misc\*.h  %TOPLEVEL%include > NUL
xcopy /Y /Q /D /R registry\*.h  %TOPLEVEL%include > NUL
xcopy /Y /Q /D /R rsrv\*.h  %TOPLEVEL%include > NUL

xcopy /Y /Q /D /R db\*.dbd  %TOPLEVEL%\dbd > NUL
xcopy /Y /Q /D /R misc\*.dbd  %TOPLEVEL%\dbd > NUL
xcopy /Y /Q /D /R rsrv\*.dbd  %TOPLEVEL%\dbd > NUL

SET PERL5LIB=C:\Strawberry\lib\perl
if not exist C:\strawberry\perl\lib\DBD.pm  mklink /h C:\strawberry\perl\lib\DBD.pm %TOPLEVEL%lib\perl\DBD.pm
if not exist C:\strawberry\perl\lib\DBD  mklink /d C:\strawberry\perl\lib\DBD %TOPLEVEL%lib\perl\DBD
if not exist C:\strawberry\perl\lib\EPICS  mklink /d C:\strawberry\perl\lib\EPICS %TOPLEVEL%lib\perl\EPICS

cd %BUILDIR%
if not exist O.Common  md O.Common
perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -o .\O.Common\dbCommon.h .\O.Common\dbCommonRecord.dbd

perl -CSD %DLLDIR%dbdToMenuH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o .\O.Common\menuPini.h menuPini.dbd
perl -CSD %DLLDIR%dbdToMenuH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o .\O.Common\menuPriority.h menuPriority.dbd
perl -CSD %DLLDIR%dbdToMenuH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o .\O.Common\menuYesNo.h menuYesNo.dbd
perl -CSD %DLLDIR%dbdToMenuH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o .\O.Common\menuPost.h menuPost.dbd
perl -CSD %DLLDIR%dbdToMenuH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o .\O.Common\menuSimm.h menuSimm.dbd
perl -CSD %DLLDIR%dbdToMenuH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o .\O.Common\menuAlarmSevr.h menuAlarmSevr.dbd
perl -CSD %DLLDIR%dbdToMenuH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o .\O.Common\menuConvert.h menuConvert.dbd
perl -CSD %DLLDIR%dbdToMenuH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o .\O.Common\menuScan.h menuScan.dbd
perl -CSD %DLLDIR%dbdToMenuH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o .\O.Common\menuAlarmStat.h menuAlarmStat.dbd
perl -CSD %DLLDIR%dbdToMenuH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o .\O.Common\menuFtype.h menuFtype.dbd
perl -CSD %DLLDIR%dbdToMenuH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o .\O.Common\menuGlobal.h menuGlobal.dbd
perl -CSD %DLLDIR%dbdToMenuH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o .\O.Common\menuIvoa.h menuIvoa.dbd
perl -CSD %DLLDIR%dbdToMenuH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o .\O.Common\menuOmsl.h menuOmsl.dbd

cd %BUILDIR%misc
perl -CSD %DLLDIR%dbdToMenuH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o ..\O.Common\dbCore.h dbCore.dbd
perl -CSD %DLLDIR%dbdToMenuH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o ..\O.Common\dlload.h dlload.dbd
perl -CSD %DLLDIR%dbdToMenuH.pl  -I..\O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o ..\O.Common\system.h system.dbd

cd %BUILDIR%dbtemplate
%DLLDIR%antelope.exe -bdbLoadTemplate dbLoadTemplate.y
move /Y dbLoadTemplate.tab.c ..\O.Common\dbLoadTemplate.c
%DLLDIR%e_flex.exe -S%TOPLEVEL%include\flex.skel.static -8 -I dbLoadTemplate_lex.l
move /Y lex.yy.c ..\O.Common\dbLoadTemplate_lex.c

cd %BUILDIR%dbstatic
%DLLDIR%antelope.exe  -bdbYacc dbYacc.y
move /Y dbYacc.tab.c ..\O.Common\dbYacc.c
%DLLDIR%e_flex.exe -S%TOPLEVEL%include\flex.skel.static -8 -I dbLex.l
move /Y lex.yy.c ..\O.Common\dbLex.c

cd %BUILDIR%db
perl -CSD %DLLDIR%podRemove.pl  -o ..\O.Common\menuAlarmSevr.dbd menuAlarmSevr.dbd.pod
perl -CSD %DLLDIR%podRemove.pl  -o ..\O.Common\menuSimm.dbd menuSimm.dbd.pod
perl -CSD %DLLDIR%podRemove.pl  -o ..\O.Common\menuScan.dbd menuScan.dbd.pod
perl -CSD %DLLDIR%podRemove.pl  -o ..\O.Common\menuAlarmStat.dbd menuAlarmStat.dbd.pod
perl -CSD %DLLDIR%podRemove.pl  -o ..\O.Common\menuFtype.dbd menuFtype.dbd.pod
perl -CSD %DLLDIR%podRemove.pl  -o ..\O.Common\menuIvoa.dbd menuIvoa.dbd.pod
perl -CSD %DLLDIR%podRemove.pl  -o ..\O.Common\menuOmsl.dbd menuOmsl.dbd.pod
perl -CSD %DLLDIR%podRemove.pl  -o ..\O.Common\menuYesNo.dbd menuYesNo.dbd.pod

cd %BUILDIR%

REM cd %TOPLEVEL%
REM perl -CSD %DLLDIR%convertRelease.pl -T %TOPLEVEL% %DLLDIR%libComModuleDirs.pm

echo BatchCompleted: %ERRORLEVEL%