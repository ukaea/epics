#!../../bin/windows-x64/bottles

#- You may have to change bottles to something else
#- everywhere it appears in this file

## Not for WinEPICS just use TARGET dirs
epicsEnvSet("TOP","C:\Users\ktn98257\source\repos\epics.dotnet\iocTops\bottles")
epicsEnvSet("IOC","bottles")

## Register all support components
dbLoadDatabase "C:\Users\ktn98257\source\repos\epics.dotnet\DBD\bottles.dbd"
bottles_registerRecordDeviceDriver pdbbase

## Macros
epicsEnvSet("U","${USERNAME}")
epicsEnvSet("F","beer")

## Load record instances
dbLoadRecords("C:/Users/ktn98257/source/repos/epics.dotnet/iocTops/bottles/db/bottles.db","F=$(F), user=$(U)")

#- Run this to trace the stages of iocInit
#traceIocInit

#- cd "${TOP}/iocBoot/${IOC}"

sleep(5)

iocInit

## Type dbl to list loaded variables
