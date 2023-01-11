#!../../bin/windows-x64/iocAcsMotion

epicsEnvSet("TOP","C:\Users\ktn98257\source\repos\epics.dotnet\iocTops\acsMotionControl")
epicsEnvSet("IOC","iocAcsMotion")
~~~~~~~~~
epicsEnvSet("SUPPORT","C:\Users\ktn98257\source\repos\epics.dotnet\support")
epicsEnvSet("EPICS_BASE","C:\Users\ktn98257\source\repos\epics.dotnet\base-3.15.9")
epicsEnvSet("ASYN","C:\Users\ktn98257\source\repos\epics.dotnet\support\asyn")
epicsEnvSet("SNCSEQ","C:\Users\ktn98257\source\repos\epics.dotnet\support\seq")
epicsEnvSet("AUTOSAVE","C:\Users\ktn98257\source\repos\epics.dotnet\support\autosave")
epicsEnvSet("MOTOR","C:\Users\ktn98257\source\repos\epics.dotnet\motor")

< envVars

## Register all support components
#dbLoadDatabase "..\..\..\..\DBD\acsMotionIOC.dbd"
dbLoadDatabase "C:\Users\ktn98257\source\repos\epics.dotnet\DBD\acsMotionIOC.dbd"
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
