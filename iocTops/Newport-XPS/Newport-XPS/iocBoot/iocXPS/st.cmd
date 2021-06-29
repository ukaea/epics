#!../../bin/linux-x86_64/XPS

## You may have to change XPS to something else
## everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/XPS.dbd"
epicsEnvSet("DEVICE", "IOC")
epicsEnvSet("XPSIP",  "192.168.254.254")

XPS_registerRecordDeviceDriver pdbbase

## Load record instances
#dbLoadRecords("db/xxx.db","user=swdevHost")
dbLoadRecords("db/Positions.db", "DEVICE=$(DEVICE)")
dbLoadRecords("db/heartbeat.db", "DEVICE=$(DEVICE)")

cd "${TOP}/iocBoot/${IOC}"
dbLoadTemplate("motor.substitutions.xps5")

XPSCreateController("XPS1", "$(XPSIP)", 5001, 1, 10, 500, 0, 500)
#XPSAuxConfig("XPS_AUX1", "$(XPSIP)", 5001, 50)

# XPS asyn port,  axis, groupName.positionerName, stepSize
XPSCreateAxis("XPS1",0,"Group1.Pos",   "1000")
#XPSCreateAxis("XPS1",1,"Group2.Pos",   "1000")

XPSCreateProfile("XPS1", 2000, "Administrator", "Administrator")

#asynSetTraceMask("XPS1",0,0xFF)
#asynReport 10 XPS1

#set_savefile_path("$(TOP)/autoSaveRestore")
#set_requestfile_path("$(TOP)/autoSaveRestore")
#set_pass1_restoreFile("Newport.sav")   

cd "${TOP}/iocBoot/${IOC}"
iocInit

#create_monitor_set("Newport.req", 5, "DEVICE=$(DEVICE)")

## Start any sequence programs
#seq sncxxx,"user=swdevHost"
