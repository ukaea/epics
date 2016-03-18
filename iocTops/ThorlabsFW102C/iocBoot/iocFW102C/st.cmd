#FW102C Start-Up Script

## Set environment variables
< envPaths
epicsEnvSet "STREAM_PROTOCOL_PATH" "$(TOP)/db"

## Change to IOC application's <top> directory
cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/FW102C.dbd"
FW102C_registerRecordDeviceDriver pdbbase

##Configure communications
drvAsynSerialPortConfigure("DEVICEPORT","COM3")
asynSetOption("DEVICEPORT", 0, "baud", "115200")
asynSetOption("DEVICEPORT", 0, "bits", "8")
asynSetOption("DEVICEPORT", 0, "parity", "none")
asynSetOption("DEVICEPORT", 0, "stop", "1")
asynSetOption("DEVICEPORT", 0, "clocal", "Y")
asynSetOption("DEVICEPORT", 0, "crtscts", "N")

## Load record instances
dbLoadRecords("db/FW102C.db")


## Initialise IOC instance
cd "${TOP}/iocBoot/${IOC}"
iocInit
