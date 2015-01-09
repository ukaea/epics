REM Windows tools (e.g. VS) tend to turn files into Windows format.
REM The general presumption should be Unix format.
REM Run this script before checking in code.

dos2unix.exe *.sh
dos2unix.exe LICENCE
dos2unix.exe MakeFile
dos2unix.exe .\configure\*
dos2unix.exe .\iocBoot\*
dos2unix.exe .\iocBoot\*
dos2unix.exe .\iocBoot\iocLeyboldTurbo\*.cmd
dos2unix.exe .\iocBoot\iocLeyboldTurbo\envPaths
dos2unix.exe .\iocBoot\iocLeyboldTurbo\MakeFile
dos2unix.exe .\LeyboldSimApp\MakeFile
dos2unix.exe .\LeyboldSimApp\*.sh
dos2unix.exe .\LeyboldSimApp\Db\*
dos2unix.exe .\LeyboldSimApp\src\*
dos2unix.exe .\LeyboldSimApp\TestScripts\Linux\*
dos2unix.exe .\LeyboldTurboApp\MakeFile
dos2unix.exe .\LeyboldTurboApp\*.sh
dos2unix.exe .\LeyboldTurboApp\Db\*
dos2unix.exe .\LeyboldTurboApp\src\*
dos2unix.exe .\LeyboldTurboApp\Scripts\Linux\*





