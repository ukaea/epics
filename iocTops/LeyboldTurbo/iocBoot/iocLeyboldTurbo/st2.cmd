#!../bin/linux-x86_64/LeyboldTurbo

## Register all support components
dbLoadDatabase ("../../dbd/LeyboldTurbo.dbd")
LeyboldTurbo_registerRecordDeviceDriver pdbbase

epicsEnvSet ASYNPORT $(ASYNPORT=LEYBOLDTURBO)
epicsEnvSet ASYNPORT1 $(ASYNPORT1=$(ASYNPORT):1)
epicsEnvSet ASYNPORT2 $(ASYNPORT2=$(ASYNPORT):2)
epicsEnvSet IOPORT PUMP

# Configure asyn communication port, first
LeyboldTurboPortDriverConfigure($(ASYNPORT), 2, $(NOOFPZD="6"))
drvAsynSerialPortConfigure($(IOPORT):1, $(COMPORT1="/dev/ttyS0"), 0, 0, 0)
LeyboldTurboAddIOPort($(IOPORT):1)
drvAsynSerialPortConfigure($(IOPORT):2, $(COMPORT2="/dev/ttyS1"), 0, 0, 0)
LeyboldTurboAddIOPort($(IOPORT):2)

dbLoadRecords("../../LeyboldTurboApp/Db/SoftwareVersions.db", "P=$(ASYNPORT):,PORT=$(ASYNPORT)")

dbLoadRecords("../../LeyboldTurboApp/Db/$(DB=LeyboldTurbo).db", "P=$(ASYNPORT1):,PORT=$(ASYNPORT),SCAN=$(SCANINT=5 second),ADDR=0")
asynSetTraceMask($(IOPORT):1, 0, 0x21)

dbLoadRecords("../../LeyboldTurboApp/Db/$(DB=LeyboldTurbo).db", "P=$(ASYNPORT2):,PORT=$(ASYNPORT),SCAN=$(SCANINT=5 second),ADDR=1")
asynSetTraceMask($(IOPORT):2, 1, 0x21)

asynSetOption ($(IOPORT):1, 0, "baud", $(BAUD=19200))
asynSetOption ($(IOPORT):1, 0, "bits", "8")
asynSetOption ($(IOPORT):1, 0, "parity", "even")
asynSetOption ($(IOPORT):1, 0, "stop", "1")
asynSetOption ($(IOPORT):1, -1, "clocal", "Y")
asynSetOption ($(IOPORT):1, -1, "crtscts", "N")

asynSetOption ($(IOPORT):2, 0, "baud", $(BAUD=19200))
asynSetOption ($(IOPORT):2, 0, "bits", "8")
asynSetOption ($(IOPORT):2, 0, "parity", "even")
asynSetOption ($(IOPORT):2, 0, "stop", "1")
asynSetOption ($(IOPORT):2, -1, "clocal", "Y")
asynSetOption ($(IOPORT):2, -1, "crtscts", "N")

iocInit
