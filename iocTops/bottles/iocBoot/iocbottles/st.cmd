#!../../bin/windows-x64/bottles

#- You may have to change bottles to something else
#- everywhere it appears in this file
## Macros
epicsEnvSet("U","${USERNAME}")
epicsEnvSet("F","beer")

## Not for WinEPICS just use TARGET dirs... to be moved to C:\ProgramFiles\epics
epicsEnvSet("IOC","iocbottles")
epicsEnvSet("TOP","C:\Users\ktn98257\source\repos\epics.dotnet\iocTops\bottles")
epicsEnvSet("BASE","C:\Users\ktn98257\source\repos\epics.dotnet\base")
epicsEnvSet("CALC","C:\Users\ktn98257\source\repos\epics.dotnet\support\calc")

## Register all support components
#dbLoadDatabase "..\..\..\..\DBD\bottles.dbd"
dbLoadDatabase "C:\Users\ktn98257\source\repos\epics.dotnet\DBD\bottles.dbd"

bottles_registerRecordDeviceDriver pdbbase

## Load record instances
dbLoadRecords("../../db/bottles.db","F=$(F), user=$(U)")

#- Run this to trace the stages of iocInit
#traceIocInit

iocInit
## Type dbl to list loaded variables
