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
dbLoadRecords("$(MOTOR)/db/motorUtil.db", "P=acsTimBENCH:")

## st.cmd specifics for ACS motion control devices
< SPiiPlus.cmd

#DEBUG
asynReport 10
var drvSPiiPlusdebug 1

iocInit

## motorUtil (allstop & alldone)
motorUtilInit("acsTimBENCH:")

# Boot complete
epicsThreadSleep(5)

# AUTOSAVE > Create request file and start periodic stash of PV's
makeAutosaveFileFromDbInfo("$(SAVE_DIR)/$(IOCNAME).req", "autosaveFields")
makeAutosaveFileFromDbInfo("$(SAVE_DIR)/$(IOCNAME)_pass0.req", "autosaveFields_pass0")
create_monitor_set("$(IOCNAME).req", 5)
create_monitor_set("$(IOCNAME)_pass0.req", 30)