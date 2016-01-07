REM http://stackoverflow.com/questions/10989147/dos-batch-print-names-of-files-exclude-certain-extensions

echo off
set ThisDirectory=%1
if "%ThisDirectory%"=="" set ThisDirectory=%cd%

FOR %%j IN (%ThisDirectory%\*) DO (
	if %%~xj neq .bat if %%~xj neq .sln if %%~xj neq .vcxproj if %%~xj neq .vcxproj.filters if %%~xj neq .sdf if %%~xj neq .lvproj if %%~xj neq .lvlps if %%~xj neq .aliases (
		dos2unix.exe %%j
	) else (
		echo skipping %%j
	)
)
