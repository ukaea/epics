REM http://stackoverflow.com/questions/10989147/dos-batch-print-names-of-files-exclude-certain-extensions
echo off
set ThisDirectory=%1
if "%ThisDirectory%"=="" set ThisDirectory=%cd%

FOR %%j IN (%ThisDirectory%\*.bat %ThisDirectory%\*.vcxproj %ThisDirectory%\*.vcxproj.filters %ThisDirectory%\*.sln) DO (
	unix2dos.exe %%j
)
