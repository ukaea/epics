REM DBcore
SET TOPLEVEL=%1
SET BUILDIR=%2
SET DLLDIR=%3

if not exist  %TOPLEVEL%include  md %TOPLEVEL%include
if not exist  %TOPLEVEL%DBD  md %TOPLEVEL%DBD
if not exist  O.Common  md O.Common

SET PERL5LIB=C:\Strawberry\lib\perl
if not exist C:\strawberry\perl\lib\DBD.pm  mklink /h C:\strawberry\perl\lib\DBD.pm %TOPLEVEL%lib\perl\DBD.pm
if not exist C:\strawberry\perl\lib\DBD  mklink /d C:\strawberry\perl\lib\DBD %TOPLEVEL%lib\perl\DBD
if not exist C:\strawberry\perl\lib\EPICS  mklink /d C:\strawberry\perl\lib\EPICS %TOPLEVEL%lib\perl\EPICS

cd dbstatic
%DLLDIR%antelope.exe -bdbYacc.y dbYacc.y
move /Y dbYacc.y.tab.c ..\O.Common\dbYacc.c
%DLLDIR%e_flex.exe -S%TOPLEVEL%include/flex.skel.static -8 -I dbLex.l
move /Y lex.yy.c ..\O.Common\dbLex.c
xcopy /Y /Q dbLexRoutines.c ..\O.Common

cd ..\dbtemplate
%DLLDIR%antelope.exe -bdbLoadTemplate.y dbLoadTemplate.y
move /Y dbLoadTemplate.y.tab.c ..\O.Common\dbLoadTemplate.c
%DLLDIR%e_flex.exe -S%TOPLEVEL%include/flex.skel.static -8 -I dbLoadTemplate_lex.l
move /Y lex.yy.c ..\O.Common\dbLoadTemplate_lex.c

cd ..\misc
perl -CSD %DLLDIR%dbdToMenuH.pl -I../O.Common -I%TOPLEVEL%include -o ..\O.Common\dbCore.h dbCore.dbd
perl -CSD %DLLDIR%dbdToMenuH.pl -I../O.Common -I%TOPLEVEL%include -o ..\O.Common\dlLoad.h dlLoad.dbd
perl -CSD %DLLDIR%dbdToMenuH.pl -I../O.Common -I%TOPLEVEL%include -o ..\O.Common\system.h system.dbd

cd ..\db
perl -CSD %DLLDIR%dbdExpand.pl  -I../O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o menuAlarmSevr.dbd menuAlarmSevr.dbd.pod
perl -CSD %DLLDIR%dbdExpand.pl  -I../O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o menuSimm.dbd menuSimm.dbd.pod
perl -CSD %DLLDIR%dbdExpand.pl  -I../O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o menuAlarmStat.dbd menuAlarmStat.dbd.pod
perl -CSD %DLLDIR%dbdExpand.pl  -I../O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o menuFtype.dbd menuFtype.dbd.pod
perl -CSD %DLLDIR%dbdExpand.pl  -I../O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o menuIvoa.dbd menuIvoa.dbd.pod
perl -CSD %DLLDIR%dbdExpand.pl  -I../O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o menuOmsl.dbd menuOmsl.dbd.pod
perl -CSD %DLLDIR%dbdExpand.pl  -I../O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o menuPini.dbd menuPini.dbd.pod
perl -CSD %DLLDIR%dbdExpand.pl  -I../O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o menuPriority.dbd menuPriority.dbd.pod
perl -CSD %DLLDIR%dbdExpand.pl  -I../O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o menuScan.dbd menuScan.dbd.pod
perl -CSD %DLLDIR%dbdExpand.pl  -I../O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o menuPost.dbd menuPost.dbd.pod
perl -CSD %DLLDIR%dbdExpand.pl  -I../O.Common -I%TOPLEVEL%include -I%TOPLEVEL%DBD -o menuYesNo.dbd menuYesNo.dbd.pod
xcopy /Y /Q menu*.dbd ..\O.Common

perl -CSD %DLLDIR%dbdToRecordtypeH.pl -I%TOPLEVEL%include -o ..\O.Common\dbCommon.h dbCommon.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl -I%TOPLEVEL%include -o ..\O.Common\dbCommonRecord.h dbCommonRecord.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl -I%TOPLEVEL%include -o ..\O.Common\menuAlarmStat.h menuAlarmStat.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl -I%TOPLEVEL%include -o ..\O.Common\menuFtype.h menuFtype.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl -I%TOPLEVEL%include -o ..\O.Common\menuIvoa.h menuIvoa.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl -I%TOPLEVEL%include -o ..\O.Common\menuOmsl.h menuOmsl.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl -I%TOPLEVEL%include -o ..\O.Common\menuPini.h menuPini.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl -I%TOPLEVEL%include -o ..\O.Common\menuPriority.h menuPriority.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl -I%TOPLEVEL%include -o ..\O.Common\menuScan.h menuScan.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl -I%TOPLEVEL%include -o ..\O.Common\menuYesNo.h menuYesNo.dbd
perl -CSD %DLLDIR%dbdToRecordtypeH.pl -I%TOPLEVEL%include -o ..\O.Common\menuPost.h menuPost.dbd

perl -CSD %DLLDIR%dbdToMenuH.pl -I../O.Common -I%TOPLEVEL%include -o ..\O.Common\menuPost.h menuPost.dbd
perl -CSD %DLLDIR%dbdToMenuH.pl -I../O.Common -I%TOPLEVEL%include -o ..\O.Common\menuSimm.h menuSimm.dbd

cd ..

xcopy /Y /Q  as\*.h  %TOPLEVEL%include > NUL
xcopy /Y /Q  bpt\*.h  %TOPLEVEL%include > NUL
xcopy /Y /Q  db\*.h  %TOPLEVEL%include > NUL
xcopy /Y /Q  dbStatic\*.h  %TOPLEVEL%include > NUL
xcopy /Y /Q  dbtemplate\*.h  %TOPLEVEL%include > NUL
xcopy /Y /Q  misc\*.h  %TOPLEVEL%include > NUL
xcopy /Y /Q  registry\*.h  %TOPLEVEL%include > NUL
xcopy /Y /Q  rsrv\*.h  %TOPLEVEL%include > NUL
xcopy /Y /Q  db\*.dbd  %TOPLEVEL%dbd > NUL
xcopy /Y /Q  misc\*.dbd  %TOPLEVEL%dbd > NUL

echo BatchCompleted: %ERRORLEVEL%
