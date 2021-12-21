#!../../bin/windows-x64/acsTech80

## Not for WinEPICS just use TARGET dirs
epicsEnvSet("TOP","C:/Users/ktn98257/source/repos/epics.dotnet/iocTops/AcsTech80")
epicsEnvSet("IOC","iocAcsTech80")
epicsEnvSet("MOTOR","C:/Users/ktn98257/source/repos/epics.dotnet/motor/motorApp")

cd "${TOP}"

## Register all support components
dbLoadDatabase "C:/Users/ktn98257/source/repos/epics.dotnet/DBD/acsTechIOC.dbd"
acsTech80_registerRecordDeviceDriver pdbbase

cd "${TOP}/iocBoot/${IOC}"

## motorUtil (allstop & alldone)
dbLoadRecords("$(MOTOR)/db/motorUtil.db", "P=acsTech80:")

## 
< SPiiPlus.cmd

iocInit

## motorUtil (allstop & alldone)
motorUtilInit("acsTech80:")

# Boot complete
