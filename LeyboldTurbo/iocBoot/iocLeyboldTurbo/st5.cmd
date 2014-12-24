#!../../bin/win32-x86/LeyboldTurbo

## You may have to change LeyboldTurbo to something else
## everywhere it appears in this file

## Register all support components
dbLoadDatabase ("../dbd/LeyboldTurbo.dbd")
LeyboldTurbo_registerRecordDeviceDriver pdbbase

epicsEnvSet ASYNPORT TURBO
epicsEnvSet IOPORT PUMP

# Configure asyn communication port, first
LeyboldTurboPortDriverConfigure($(ASYNPORT), 5)
drvAsynSerialPortConfigure($(IOPORT):1, $(COMPORT1), 0, 0, 0)
LeyboldTurboAddIOPort($(IOPORT):1)
drvAsynSerialPortConfigure($(IOPORT):2, $(COMPORT2), 0, 0, 0)
LeyboldTurboAddIOPort($(IOPORT):2)
drvAsynSerialPortConfigure($(IOPORT):3, $(COMPORT3), 0, 0, 0)
LeyboldTurboAddIOPort($(IOPORT):3)
drvAsynSerialPortConfigure($(IOPORT):4, $(COMPORT4), 0, 0, 0)
LeyboldTurboAddIOPort($(IOPORT):4)
drvAsynSerialPortConfigure($(IOPORT):5, $(COMPORT5), 0, 0, 0)
LeyboldTurboAddIOPort($(IOPORT):5)

## Load record instances
dbLoadRecords("Db/LeyboldTurbo.db", "P=$(ASYNPORT):1:,PORT=$(ASYNPORT),ADDR=0")

dbLoadRecords("Db/LeyboldTurbo.db", "P=$(ASYNPORT):2:,PORT=$(ASYNPORT),ADDR=1")

dbLoadRecords("Db/LeyboldTurbo.db", "P=$(ASYNPORT):3:,PORT=$(ASYNPORT),ADDR=2")

dbLoadRecords("Db/LeyboldTurbo.db", "P=$(ASYNPORT):4:,PORT=$(ASYNPORT),ADDR=3")

dbLoadRecords("Db/LeyboldTurbo.db", "P=$(ASYNPORT):5:,PORT=$(ASYNPORT),ADDR=4")

asynSetOption ($(IOPORT):1, 0, "baud", "19200")
asynSetOption ($(IOPORT):1, 0, "bits", "8")
asynSetOption ($(IOPORT):1, 0, "parity", "even")
asynSetOption ($(IOPORT):1, 0, "stop", "1")
asynSetOption ($(IOPORT):1, -1, "clocal", "Y")
asynSetOption ($(IOPORT):1, -1, "crtscts", "N")

asynSetOption ($(IOPORT):2, 0, "baud", "19200")
asynSetOption ($(IOPORT):2, 0, "bits", "8")
asynSetOption ($(IOPORT):2, 0, "parity", "even")
asynSetOption ($(IOPORT):2, 0, "stop", "1")
asynSetOption ($(IOPORT):2, -1, "clocal", "Y")
asynSetOption ($(IOPORT):2, -1, "crtscts", "N")

asynSetOption ($(IOPORT):3, 0, "baud", "19200")
asynSetOption ($(IOPORT):3, 0, "bits", "8")
asynSetOption ($(IOPORT):3, 0, "parity", "even")
asynSetOption ($(IOPORT):3, 0, "stop", "1")
asynSetOption ($(IOPORT):3, -1, "clocal", "Y")
asynSetOption ($(IOPORT):3, -1, "crtscts", "N")

asynSetOption ($(IOPORT):4, 0, "baud", "19200")
asynSetOption ($(IOPORT):4, 0, "bits", "8")
asynSetOption ($(IOPORT):4, 0, "parity", "even")
asynSetOption ($(IOPORT):4, 0, "stop", "1")
asynSetOption ($(IOPORT):4, -1, "clocal", "Y")
asynSetOption ($(IOPORT):4, -1, "crtscts", "N")

asynSetOption ($(IOPORT):5, 0, "baud", "19200")
asynSetOption ($(IOPORT):5, 0, "bits", "8")
asynSetOption ($(IOPORT):5, 0, "parity", "even")
asynSetOption ($(IOPORT):5, 0, "stop", "1")
asynSetOption ($(IOPORT):5, -1, "clocal", "Y")
asynSetOption ($(IOPORT):5, -1, "crtscts", "N")

iocInit

## Start any sequence programs
#seq sncxxx,"user=pheestHost"
