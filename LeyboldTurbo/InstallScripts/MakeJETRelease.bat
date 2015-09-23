md \\catsserver.jetcats.jet.efda.org\CopyArea\LeyboldTurbo\Release
md \\catsserver.jetcats.jet.efda.org\CopyArea\LeyboldTurbo\Release\dbd
md \\catsserver.jetcats.jet.efda.org\CopyArea\LeyboldTurbo\Release\LeyboldSimApp
md \\catsserver.jetcats.jet.efda.org\CopyArea\LeyboldTurbo\Release\LeyboldSimApp\Db
md \\catsserver.jetcats.jet.efda.org\CopyArea\LeyboldTurbo\Release\LeyboldSimApp\TestScripts
md \\catsserver.jetcats.jet.efda.org\CopyArea\LeyboldTurbo\Release\LeyboldTurboApp
md \\catsserver.jetcats.jet.efda.org\CopyArea\LeyboldTurbo\Release\LeyboldTurboApp\Db
md \\catsserver.jetcats.jet.efda.org\CopyArea\LeyboldTurbo\Release\LeyboldTurboApp\Scripts
md \\catsserver.jetcats.jet.efda.org\CopyArea\LeyboldTurbo\Release\iocBoot
md \\catsserver.jetcats.jet.efda.org\CopyArea\LeyboldTurbo\Release\iocBoot\iocLeyboldTurbo
md \\catsserver.jetcats.jet.efda.org\CopyArea\LeyboldTurbo\Release\iocBoot\iocLeyboldSim
md \\catsserver.jetcats.jet.efda.org\CopyArea\LeyboldTurbo\Release\Release_LIB


copy %~dp0..\dbd\LeyboldSim.dbd \\catsserver.jetcats.jet.efda.org\CopyArea\LeyboldTurbo\Release\dbd
copy %~dp0..\dbd\LeyboldTurbo.dbd \\catsserver.jetcats.jet.efda.org\CopyArea\LeyboldTurbo\Release\dbd

copy %~dp0..\LeyboldSimApp\Db\*.db \\catsserver.jetcats.jet.efda.org\CopyArea\LeyboldTurbo\Release\LeyboldSimApp\Db

copy %~dp0..\LeyboldSimApp\TestScripts\*.py \\catsserver.jetcats.jet.efda.org\CopyArea\LeyboldTurbo\Release\LeyboldSimApp\TestScripts
copy %~dp0..\LeyboldSimApp\TestScripts\*.bat \\catsserver.jetcats.jet.efda.org\CopyArea\LeyboldTurbo\Release\LeyboldSimApp\TestScripts

copy %~dp0..\LeyboldTurboApp\Db\*.db \\catsserver.jetcats.jet.efda.org\CopyArea\LeyboldTurbo\Release\LeyboldTurboApp\Db
copy %~dp0..\LeyboldTurboApp\Scripts\*.py \\catsserver.jetcats.jet.efda.org\CopyArea\LeyboldTurbo\Release\LeyboldTurboApp\Scripts
copy %~dp0..\LeyboldTurboApp\Scripts\*.bat \\catsserver.jetcats.jet.efda.org\CopyArea\LeyboldTurbo\Release\LeyboldTurboApp\Scripts
copy %~dp0..\LeyboldTurboApp\version.txt \\catsserver.jetcats.jet.efda.org\CopyArea\LeyboldTurbo\Release\LeyboldTurboApp

copy %~dp0..\iocBoot\iocLeyboldSim\*.cmd \\catsserver.jetcats.jet.efda.org\CopyArea\LeyboldTurbo\Release\iocBoot\iocLeyboldSim
copy %~dp0..\iocBoot\iocLeyboldTurbo\*.cmd \\catsserver.jetcats.jet.efda.org\CopyArea\LeyboldTurbo\Release\iocBoot\iocLeyboldTurbo

copy %~dp0..\Release_LIB\LeyboldSim.exe \\catsserver.jetcats.jet.efda.org\CopyArea\LeyboldTurbo\Release\Release_LIB
copy %~dp0..\Release_LIB\LeyboldTurboApp.exe \\catsserver.jetcats.jet.efda.org\CopyArea\LeyboldTurbo\Release\Release_LIB

