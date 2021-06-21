if not exist  $(SolutionDir)include md  $(SolutionDir)include
xcopy /Y /Q  /D /R *.h $(SolutionDir)include > NUL