REM http://stackoverflow.com/questions/10989147/dos-batch-print-names-of-files-exclude-certain-extensions


FOR /D /R %%i IN (*) DO (
	if "%%i" neq ".git" if "%%i" neq "boostApp" if "%%i" neq "Debug_DLL" if "%%i" neq "Debug_LIB" if "%%i" neq "Release_DLL" if "%%i" neq "Release_LIB" (
		if "%%~i" neq ".bat" if "%%~i" neq ".sln" if "%%~i" neq ".vcxproj" if "%%~i" neq ".vcxproj.filters" if "%%~i" neq ".obj" dos2unix.exe %%i
	)
)
