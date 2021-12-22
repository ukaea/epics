#!../../bin/linux-x86_64/parker6k8

## You may have to change parker6k8 to something else
## everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/parker6k8.dbd"
parker6k8_registerRecordDeviceDriver pdbbase

####################################################
# P6K Controller - Configure communications
drvAsynSerialPortConfigure("6K", "COM1", 0, 0, 0)
asynSetOption("DEVICEPORT", 0, "baud", "9600")
asynSetOption("DEVICEPORT", 0, "bits", "8")
asynSetOption("DEVICEPORT", 0, "parity", "none")
asynSetOption("DEVICEPORT", 0, "stop", "1")
asynSetOption("DEVICEPORT", 0, "clocal", "Y")
asynSetOption("DEVICEPORT", 0, "crtscts", "N")

## Load record instances
dbLoadRecords("db/parker6k8.db")

# ####################################################
# Instantiate controller
# p6kCreateController(const char *portName, const char *lowLevelPortName, 
#			       int lowLevelPortAddress, int numAxes, 
#			       int movingPollPeriod, int idlePollPeriod)
p6kCreateController("P6K","6K",0,8,100,2000)
p6kUpload("P6K", "${TOP}/iocBoot/iocparker6k8/P6K_CONFIG")
p6kUpload("P6K", "${TOP}/iocBoot/iocparker6k8/P6K_AXIS_1_CONFIG.cmd")
p6kUpload("P6K", "${TOP}/iocBoot/iocparker6k8/P6K_AXIS_2_CONFIG.cmd")
p6kUpload("P6K", "${TOP}/iocBoot/iocparker6k8/P6K_AXIS_3_CONFIG.cmd")
p6kUpload("P6K", "${TOP}/iocBoot/iocparker6k8/P6K_AXIS_4_CONFIG.cmd")
p6kUpload("P6K", "${TOP}/iocBoot/iocparker6k8/P6K_AXIS_5_CONFIG.cmd")
p6kUpload("P6K", "${TOP}/iocBoot/iocparker6k8/P6K_AXIS_6_CONFIG.cmd")
p6kUpload("P6K", "${TOP}/iocBoot/iocparker6k8/P6K_AXIS_7_CONFIG.cmd")
p6kUpload("P6K", "${TOP}/iocBoot/iocparker6k8/P6K_AXIS_8_CONFIG.cmd")

####################################################
# DEBUG - This prints low level commands and Ethernet responses.
#asynSetTraceMask("6K",-1,0x9)
#asynSetTraceIOMask("6K",-1,0x2)
# asynSetTraceMask("6K",0,0xFF)
# asynSetTraceIOMask("6K",0,0xFF)

# ####################################################
# p6kCreateAxis(const char *p6kName, int axis)
# Axis particulars - 8 of
p6kCreateAxis("P6K",1)
p6kCreateAxis("P6K",2)
p6kCreateAxis("P6K",3)
p6kCreateAxis("P6K",4)
p6kCreateAxis("P6K",5)
p6kCreateAxis("P6K",6)
p6kCreateAxis("P6K",7)
p6kCreateAxis("P6K",8)

#################################################
# autosave5228b63us_prefix("TZ:Mot:P6K:")

set_requestfile_path("$(SAVE_DIR)")
set_savefile_path("$(SAVE_DIR)")

save_restoreSet_NumSeqFiles(3)
save_restoreSet_SeqPeriodInSeconds(600)
set_pass0_restoreFile("$(IOC).sav")
set_pass0_restoreFil5228b63###############################
# Engage IOC
cd ${TOP}/iocBoot/${IOC}
iocInit

# Create request file and commence periodic 'save' to log of positions
epicsThreadSleep(0.5)
makeAutosaveFileFromDbInfo("$(SAVE_DIR)/$(IOC).req", "autosaveFields")
makeAutosaveFileFromDbInfo("$(SAVE_DIR)/$(IOC)_pass0.req", "autosaveFields_pass0")
create_monitor_set("$(IOC).req", 5)
create_monitor_set("$(IOC)_pass0.req", 30)
