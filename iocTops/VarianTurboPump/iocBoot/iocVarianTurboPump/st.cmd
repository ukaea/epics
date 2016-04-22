#!../../bin/linux-armv5teb/VarianTurboPump

## You may have to change VarianTurboPump to something else
## everywhere it appears in this file

#< envPaths

epicsEnvSet IOCSH_PS1 "Varian-Agilent Turbopump> "
epicsEnvSet STREAM_PROTOCOL_PATH ".:../../db"

## Register all support components
dbLoadDatabase("../../dbd/VarianTurboPump.dbd",0,0)
VarianTurboPump_registerRecordDeviceDriver(pdbbase) 
MoxaSerialSupport_registerRecordDeviceDriver(pdbbase) 

moxaSerialModeSet( "/dev/ttyM1", "rs485")
moxaSerialModeSet( "/dev/ttyM2", "rs485")
moxaSerialModeSet( "/dev/ttyM3", "rs485")
moxaSerialModeSet( "/dev/ttyM4", "rs485")
moxaSerialModeSet( "/dev/ttyM5", "rs485")
moxaSerialModeSet( "/dev/ttyM6", "rs485")
moxaSerialModeSet( "/dev/ttyM7", "rs485")
#moxaSerialModeGet( "/dev/ttyM1")
#moxaSerialModeGet( "/dev/ttyM2")
#moxaSerialModeGet( "/dev/ttyM3")
#moxaSerialModeGet( "/dev/ttyM4")
#moxaSerialModeGet( "/dev/ttyM5")
#moxaSerialModeGet( "/dev/ttyM6")
#moxaSerialModeGet( "/dev/ttyM7")

drvAsynSerialPortConfigure ("L1","/dev/ttyM1")
asynSetOption ("L1", 0, "baud", "9600")
asynSetOption ("L1", 0, "bits", "8")
asynSetOption ("L1", 0, "parity", "none")
asynSetOption ("L1", 0, "stop", "1")
asynSetOption ("L1", 0, "clocal", "N")
asynSetOption ("L1", 0, "crtscts", "N")

drvAsynSerialPortConfigure ("L2","/dev/ttyM2")
asynSetOption ("L2", 0, "baud", "9600")
asynSetOption ("L2", 0, "bits", "8")
asynSetOption ("L2", 0, "parity", "none")
asynSetOption ("L2", 0, "stop", "1")
asynSetOption ("L2", 0, "clocal", "N")
asynSetOption ("L2", 0, "crtscts", "N")

drvAsynSerialPortConfigure ("L3","/dev/ttyM3")
asynSetOption ("L3", 0, "baud", "9600")
asynSetOption ("L3", 0, "bits", "8")
asynSetOption ("L3", 0, "parity", "none")
asynSetOption ("L3", 0, "stop", "1")
asynSetOption ("L3", 0, "clocal", "N")
asynSetOption ("L3", 0, "crtscts", "N")

drvAsynSerialPortConfigure ("L4","/dev/ttyM4")
asynSetOption ("L4", 0, "baud", "9600")
asynSetOption ("L4", 0, "bits", "8")
asynSetOption ("L4", 0, "parity", "none")
asynSetOption ("L4", 0, "stop", "1")
asynSetOption ("L4", 0, "clocal", "N")
asynSetOption ("L4", 0, "crtscts", "N")

drvAsynSerialPortConfigure ("L5","/dev/ttyM5")
asynSetOption ("L5", 0, "baud", "9600")
asynSetOption ("L5", 0, "bits", "8")
asynSetOption ("L5", 0, "parity", "none")
asynSetOption ("L5", 0, "stop", "1")
asynSetOption ("L5", 0, "clocal", "N")
asynSetOption ("L5", 0, "crtscts", "N")

drvAsynSerialPortConfigure ("L6","/dev/ttyM6")
asynSetOption ("L6", 0, "baud", "9600")
asynSetOption ("L6", 0, "bits", "8")
asynSetOption ("L6", 0, "parity", "none")
asynSetOption ("L6", 0, "stop", "1")
asynSetOption ("L6", 0, "clocal", "N")
asynSetOption ("L6", 0, "crtscts", "N")

drvAsynSerialPortConfigure ("L7","/dev/ttyM7")
asynSetOption ("L7", 0, "baud", "9600")
asynSetOption ("L7", 0, "bits", "8")
asynSetOption ("L7", 0, "parity", "none")
asynSetOption ("L7", 0, "stop", "1")
asynSetOption ("L7", 0, "clocal", "N")
asynSetOption ("L7", 0, "crtscts", "N")

## Load record instances
dbLoadRecords("../../db/VarianTurboPump.db")

iocInit()

## Start any sequence programs
#seq sncVarianTurboPump
