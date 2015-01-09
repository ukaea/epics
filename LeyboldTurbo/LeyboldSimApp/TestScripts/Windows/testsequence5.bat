echo off
set EPICS_CA_SERVER_PORT=5071

start cmd /C camonitor5.bat

call testsequence1.bat 1
timeout 10

call testsequence1.bat 2
timeout 10

call testsequence1.bat 3
timeout 10

call testsequence1.bat 4
timeout 10

call testsequence1.bat 5

