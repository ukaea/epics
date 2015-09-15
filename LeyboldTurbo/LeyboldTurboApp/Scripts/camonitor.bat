set NUMPUMPS=1 
if not "%1"=="" set NUMPUMPS=%1
echo "Number of pumps " %NUMPUMPS%

md D:\LocalData\
camonitor.py %NUMPUMPS% D:\LocalData\camonitor.app.log
