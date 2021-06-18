perl -CSD %DLLDIR%\makeIncludeDbd.pl menuAlarmSevr.dbd menuAlarmStat.dbd menuFtype.dbd menuIvoa.dbd menuOmsl.dbd menuPini.dbd menuPost.dbd menuPriority.dbd menuYesNo.dbd menuSimm.dbd O.Common\menuGlobal.dbd
xcopy /Y /Q /D /R  db\O.Common\*.h  %TOPLEVEL%\include > NUL

SET PERL5LIB= C:\Strawberry\lib\perl
xcopy /Y /Q /D /R O.Common\*.h  %TOPLEVEL%\include > NUL
xcopy /Y /Q /D /R O.Common\*.dbd %TOPLEVEL%\DBD > NUL
