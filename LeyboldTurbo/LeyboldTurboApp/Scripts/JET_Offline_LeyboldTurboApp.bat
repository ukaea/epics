set NUMPUMPS=1
set BAUD=9600
set NoOfPZD=2

set COMPORT1=COM2

start ..\..\Release_LIB\LeyboldTurboApp ..\..\iocBoot\iocLeyboldTurbo\st1.cmd
start ..\..\Release_LIB\LeyboldTurboGUI.exe

md d:\LocalData\
camonitor.py 1 1 D:\LocalData\
