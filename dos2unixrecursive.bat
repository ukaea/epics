REM http://stackoverflow.com/questions/10989147/dos-batch-print-names-of-files-exclude-certain-extensions

set ThisDirectoryRecursive=%1
if "%ThisDirectoryRecursive%"=="" set ThisDirectoryRecursive=%cd%

call dos2unix %ThisDirectoryRecursive%

FOR /D %%I IN (%ThisDirectoryRecursive%\*) DO (
	if not %%~nxI==Bin if not %%~nxI==dbd if not %%~nxI==.git if not %%~nxI==.klocwork if not %%~nxI==boostApp if not %%~nxI==O.win32-x86 if not %%~nxI==O.win32-x86-debug if not %%~nxI==O.Common if not %%~nxI==Debug_DLL if not %%~nxI==Debug_LIB if not %%~nxI==Release_DLL if not %%~nxI==Release_LIB (
		call dos2unixrecursive.bat %%I
	) else (
		echo skipping %%I as %%~nxI
	)	
)
