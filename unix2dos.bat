REM http://stackoverflow.com/questions/10989147/dos-batch-print-names-of-files-exclude-certain-extensions
FOR /D /R %%i IN (*) DO (
	if "%%i" neq ".git" if "%%i" neq "boostApp" if "%%i" neq "Debug_DLL" if "%%i" neq "Debug_LIB" if "%%i" neq "Release_DLL" if "%%i" neq "Release_LIB" (
		FOR %%j IN (*.bat *.vcxproj *.vcxproj.filters *.sln) DO unix2dos.exe "%%~j
	)
)


