#!../../bin/windows-x64/iocAcsMotion

epicsEnvSet("TOP","C:\Users\ktn98257\source\repos\epics.dotnet\iocTops\acsMotionControl")
epicsEnvSet("IOC","iocAcsMotion")
cd "${TOP}\iocBoot\${IOC}"

< envPaths
< envVars

## Register all support components
dbLoadDatabase "..\..\..\..\DBD\acsMotionIOC.dbd"
acsMotion_registerRecordDeviceDriver pdbbase

cd "${TOP}/iocBoot/${IOC}"

## motorUtil (allstop & alldone)
dbLoadRecords("$(MOTOR)/db/motorUtil.db", "P=$(PREFIX)")

## 
< AcsMotion.cmd
##
#< AcsMotionAuxIO.cmd

iocInit

epicsThreadSleep(2)

##Autosave particulars


## motorUtil (allstop & alldone)
motorUtilInit("$(PREFIX)")

# Boot complete
