#!../../bin/_ARCH_/VarianTurboPump

#< envPaths

epicsEnvSet IOCSH_PS1 "Varian-Agilent Turbopump> "
epicsEnvSet STREAM_PROTOCOL_PATH ".:../../protocols"

## Register all support components
dbLoadDatabase("../../dbd/VarianTurboPump.dbd",0,0)
VarianTurboPump_registerRecordDeviceDriver(pdbbase)
#MoxaSerialSupport_registerRecordDeviceDriver(pdbbase)

#moxaSerialModeSet( "/dev/ttyS1", "RS485x4")
##moxaSerialModeGet( "/dev/ttyM1")

drvAsynSerialPortConfigure ("L1","/dev/ttyS1")
#drvAsynSerialPortConfigure ("L1","/dev/ttyM1")
asynSetOption ("L1", 0, "baud", "9600")
asynSetOption ("L1", 0, "bits", "8")
asynSetOption ("L1", 0, "parity", "none")
asynSetOption ("L1", 0, "stop", "1")
asynSetOption ("L1", 0, "clocal", "N")
asynSetOption ("L1", 0, "crtscts", "N")

## Load record instances
dbLoadRecords("../../db/VarianTurboPump.db")

iocInit()

## Start any sequence programs
#seq sncVarianTurboPump ""
