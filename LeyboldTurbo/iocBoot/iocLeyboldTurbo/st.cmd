#!../../bin/win32-x86/LeyboldTurbo

## You may have to change LeyboldTurbo to something else
## everywhere it appears in this file

## Register all support components
dbLoadDatabase ("../dbd/LeyboldTurbo.dbd")
LeyboldTurbo_registerRecordDeviceDriver pdbbase

epicsEnvSet ASYNPORT TURBO:$(INDEX)
epicsEnvSet IOPORT PUMP:$(INDEX)

# Configure asyn communication port, first
drvAsynSerialPortConfigure($(IOPORT), $(COMPORT), 0, 0, 0)
LeyboldTurboPortDriverConfigure($(ASYNPORT), $(IOPORT))

## Load record instances
dbLoadRecords("db/LeyboldTurbo.db", "P=$(ASYNPORT):,PORT=$(ASYNPORT)")
dbLoadRecords("../../asyn/asyn/asynRecord/asynRecord.db","P=$(ASYNPORT):,R=asyn,PORT=$(ASYNPORT),ADDR=0,OMAX=80,IMAX=80")

asynSetOption ($(IOPORT), 0, "baud", "19200")
asynSetOption ($(IOPORT), 0, "bits", "8")
asynSetOption ($(IOPORT), 0, "parity", "even")
asynSetOption ($(IOPORT), 0, "stop", "1")
asynSetOption ($(IOPORT), -1, "clocal", "Y")
asynSetOption ($(IOPORT), -1, "crtscts", "N")

iocInit

## Start any sequence programs
#seq sncxxx,"user=pheestHost"
