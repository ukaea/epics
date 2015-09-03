#!../../bin/_TARGETARCH_/LeyboldTurbo

## Register all support components
dbLoadDatabase ("../dbd/LeyboldTurbo.dbd")
LeyboldTurbo_registerRecordDeviceDriver pdbbase

epicsEnvSet ASYNPORT LEYBOLDTURBO
epicsEnvSet IOPORT PUMP

# Configure asyn communication port, first
LeyboldTurboPortDriverConfigure($(ASYNPORT), 1, $(NOOFPZD))
drvAsynSerialPortConfigure($(IOPORT):1, $(COMPORT1), 0, 0, 0)
LeyboldTurboAddIOPort($(IOPORT):1)

## Load record instances
dbLoadRecords("Db/LeyboldTurbo.db", "P=$(ASYNPORT):1:,PORT=$(ASYNPORT),ADDR=0")

asynSetOption ($(IOPORT):1, 0, "baud", $(BAUD))
asynSetOption ($(IOPORT):1, 0, "bits", "8")
asynSetOption ($(IOPORT):1, 0, "parity", "even")
asynSetOption ($(IOPORT):1, 0, "stop", "1")
asynSetOption ($(IOPORT):1, -1, "clocal", "Y")
asynSetOption ($(IOPORT):1, -1, "crtscts", "N")

iocInit

## Start any sequence programs
#seq sncxxx,"user=pheestHost"
