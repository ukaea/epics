SET TOPLEVEL=%1
SET BUILDIR=%2
SET DLLDIR=%3

if not exist O.Common  md O.Common
if not exist %TOPLEVEL%\include  md %TOPLEVEL%\include
if not exist %TOPLEVEL%\dbd  md %TOPLEVEL%\dbd

SET PERL5LIB=C:\Strawberry\lib\perl
if not exist C:\strawberry\perl\lib\DBD.pm  mklink /h C:\strawberry\perl\lib\DBD.pm %TOPLEVEL%lib\perl\DBD.pm
if not exist C:\strawberry\perl\lib\DBD  mklink /d C:\strawberry\perl\lib\DBD %TOPLEVEL%lib\perl\DBD
if not exist C:\strawberry\perl\lib\EPICS  mklink /d C:\strawberry\perl\lib\EPICS %TOPLEVEL%lib\perl\EPICS

REM Timi - items for consideration!!!!
REM Any H stuff to topLevel DBD's
REM ==============================
REM perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I./O.Common -I%TOPLEVEL%include -I%TOPLEVEL%dbd -o ./O.Common/devStream.h devStream.dbd
REM perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I./O.Common -I%TOPLEVEL%include -I%TOPLEVEL%dbd -o ./O.Common/StreamBuffer.h
REM perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I./O.Common -I%TOPLEVEL%include -I%TOPLEVEL%dbd -o ./O.Common/StreamBusInterface.h
REM perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I./O.Common -I%TOPLEVEL%include -I%TOPLEVEL%dbd -o ./O.Common/StreamCore.h
REM perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I./O.Common -I%TOPLEVEL%include -I%TOPLEVEL%dbd -o ./O.Common/StreamFormat.h
REM perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I./O.Common -I%TOPLEVEL%include -I%TOPLEVEL%dbd -o ./O.Common/StreamFormatConverter.h
REM perl -CSD %DLLDIR%dbdToRecordtypeH.pl  -I./O.Common -I%TOPLEVEL%include -I%TOPLEVEL%dbd -o ./O.Common/StreamProtocol.h

REM perl -CSD %DLLDIR%dbdExpand.pl -I%TOPLEVEL%DBD -o O.Common/stream.dbd streamInclude.dbd
REM xcopy asynRegistrars.dbd & streamApp.dbd to %TOPLEVEL%/DBD

echo BatchCompleted: %ERRORLEVEL%