#!../../bin/windows-x64/bottles

#- You may have to change bottles to something else
#- everywhere it appears in this file
## Macros
epicsEnvSet("U","${USERNAME}")
epicsEnvSet("F","beer")

## Not for WinEPICS just use TARGET dirs... to be moved to C:\ProgramFiles\epics
epicsEnvSet("IOC","iocbottles")
epicsEnvSet("TOP","C:\Users\ktn98257\source\repos\epics.dotnet\iocTops\bottles")
epicsEnvSet("SUPPORT","C:\Users\ktn98257\source\repos\epics.dotnet\support")
epicsEnvSet("CALC","C:\Users\ktn98257\source\repos\epics.dotnet\support\calc")
epicsEnvSet("EPICS_BASE","C:\Users\ktn98257\source\repos\epics.dotnet\base-3.15.9")


## Register all support components
#dbLoadDatabase "..\..\..\..\DBD\bottles.dbd"
dbLoadDatabase "C:\Users\ktn98257\source\repos\epics.dotnet\iocTops\bottles\bottles.dbd"

bottles_registerRecordDeviceDriver pdbbase

## Load record instances
dbLoadRecords("../../db/bottles.db","F=$(F), user=$(U)")

#- Run this to trace the stages of iocInit

#traceIocInit

iocInit
## Type dbl to list loaded variables
