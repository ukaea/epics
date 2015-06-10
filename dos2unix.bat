REM http://stackoverflow.com/questions/10989147/dos-batch-print-names-of-files-exclude-certain-extensions

echo on

if "%1" neq "" pushd %1

FOR %%j IN (*) DO (
	echo %%j | findstr /i ".bat" > nul
	if "%errorlevel%"=="0" goto skipit
	echo %%j | findstr /i ".sln" > nul
	if "%errorlevel%"=="0" goto skipit
	echo %%j | findstr /i ".vcxproj" > nul
	if "%errorlevel%"=="0" goto skipit
	echo %%j | findstr /i "vcxproj.filters" > nul
	if "%errorlevel%"=="0" goto skipit
	dos2unix.exe %%j
	goto continue
:skipit
	echo skipped "%%j"
:continue
	REM continuing
)
if "%1" neq "" popd
