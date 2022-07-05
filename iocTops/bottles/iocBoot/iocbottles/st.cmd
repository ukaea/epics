#!../../bin/windows-x64/bottles

#- You may have to change bottles to something else
#- everywhere it appears in this file
## Macros
epicsEnvSet("U","${USERNAME}")
epicsEnvSet("F","beer")

## Not for WinEPICS just use TARGET dirs... to be moved to C:\ProgramFiles\epics
epicsEnvSet("TOP","C:\Users\${USERNAME}\Documents\0.repos\epics.dotnet\iocTops\bottles")
cd "${TOP}/iocBoot/${IOC}"

epicsEnvSet("IOC","bottles")
## Register all support components
dbLoadDatabase "..\..\DBD\bottles.dbd"

bottles_registerRecordDeviceDriver pdbbase

## Load record instances
dbLoadRecords("../../db/bottles.db","F=$(F), user=$(U)")

#- Run this to trace the stages of iocInit

#traceIocInit

iocInit
## Type dbl to list loaded variables
