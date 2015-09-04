REM http://stackoverflow.com/questions/10989147/dos-batch-print-names-of-files-exclude-certain-extensions

echo off
set ThisDirectory=%1
if "%ThisDirectory%"=="" set ThisDirectory=%cd%

FOR %%j IN (%ThisDirectory%\*) DO (
	if %%~xj neq .bat if %%~xj neq .sln if %%~xj neq .vcxproj if %%~xj neq .vcxproj.filters if %%~xj neq .sdf (
		dos2unix.exe %%j
	) else (
		echo skipping %%j
	)
)
