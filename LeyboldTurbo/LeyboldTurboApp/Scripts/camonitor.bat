set FIRSTPUMP=1 
if not "%1"=="" set FIRSTPUMP=%1

set LASTPUMP=%FIRSTPUMP% 
if not "%2"=="" set LASTPUMP=%2

echo "Monitoring pumps %FISRPUMP% to %LASTPUMP%"

md D:\LocalData\
camonitor.py %FISRPUMP% %LASTPUMP% D:\LocalData\camonitor.app.log
