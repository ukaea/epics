#!../../bin/linux-x86_64/LeyboldTurbo

## Register all support components
dbLoadDatabase ("../../dbd/LeyboldTurbo.dbd")
LeyboldTurbo_registerRecordDeviceDriver pdbbase

epicsEnvSet ASYNPORT $(ASYNPORT=LEYBOLDTURBO)
epicsEnvSet ASYNPORT1 $(ASYNPORT1=$(ASYNPORT):1)
epicsEnvSet ASYNPORT2 $(ASYNPORT2=$(ASYNPORT):2)
epicsEnvSet ASYNPORT3 $(ASYNPORT3=$(ASYNPORT):3)
epicsEnvSet ASYNPORT4 $(ASYNPORT4=$(ASYNPORT):4)
epicsEnvSet ASYNPORT5 $(ASYNPORT5=$(ASYNPORT):5)
epicsEnvSet ASYNPORT6 $(ASYNPORT6=$(ASYNPORT):6)
epicsEnvSet IOPORT PUMP

# Configure asyn communication port, first
LeyboldTurboPortDriverConfigure($(ASYNPORT), 6, $(NOOFPZD="6"))
drvAsynSerialPortConfigure($(IOPORT):1, $(COMPORT1="/dev/ttyS0"), 0, 0, 0)
LeyboldTurboAddIOPort($(IOPORT):1)
drvAsynSerialPortConfigure($(IOPORT):2, $(COMPORT2="/dev/ttyS1"), 0, 0, 0)
LeyboldTurboAddIOPort($(IOPORT):2)
drvAsynSerialPortConfigure($(IOPORT):3, $(COMPORT3="/dev/ttyS2"), 0, 0, 0)
LeyboldTurboAddIOPort($(IOPORT):3)
drvAsynSerialPortConfigure($(IOPORT):4, $(COMPORT4="/dev/ttyS3"), 0, 0, 0)
LeyboldTurboAddIOPort($(IOPORT):4)
drvAsynSerialPortConfigure($(IOPORT):5, $(COMPORT5="/dev/ttyS4"), 0, 0, 0)
LeyboldTurboAddIOPort($(IOPORT):5)
drvAsynSerialPortConfigure($(IOPORT):6, $(COMPORT6="/dev/ttyS5"), 0, 0, 0)
LeyboldTurboAddIOPort($(IOPORT):6)

## Load record instances

dbLoadRecords("../../LeyboldTurboApp/Db/SoftwareVersions.db", "P=$(ASYNPORT):,PORT=$(ASYNPORT)")

dbLoadRecords("../../LeyboldTurboApp/Db/$(DB=LeyboldTurbo).db", "P=$(ASYNPORT1):,PORT=$(ASYNPORT),SCAN=$(SCANINT=10 second),ADDR=0")
asynSetTraceMask($(IOPORT):1, 0, 0x21)

dbLoadRecords("../../LeyboldTurboApp/Db/$(DB=LeyboldTurbo).db", "P=$(ASYNPORT2):,PORT=$(ASYNPORT),SCAN=$(SCANINT=10 second),ADDR=1")
asynSetTraceMask($(IOPORT):2, 1, 0x21)

dbLoadRecords("../../LeyboldTurboApp/Db/$(DB=LeyboldTurbo).db", "P=$(ASYNPORT3):,PORT=$(ASYNPORT),SCAN=$(SCANINT=10 second),ADDR=2")
asynSetTraceMask($(IOPORT):3, 2, 0x21)

dbLoadRecords("../../LeyboldTurboApp/Db/$(DB=LeyboldTurbo).db", "P=$(ASYNPORT4):,PORT=$(ASYNPORT),SCAN=$(SCANINT=10 second),ADDR=3")
asynSetTraceMask($(IOPORT):4, 3, 0x21)

dbLoadRecords("../../LeyboldTurboApp/Db/$(DB=LeyboldTurbo).db", "P=$(ASYNPORT5):,PORT=$(ASYNPORT),SCAN=$(SCANINT=10 second),ADDR=4")
asynSetTraceMask($(IOPORT):5, 4, 0x21)

dbLoadRecords("../../LeyboldTurboApp/Db/$(DB=LeyboldTurbo).db", "P=$(ASYNPORT6):,PORT=$(ASYNPORT),SCAN=$(SCANINT=10 second),ADDR=5")
asynSetTraceMask($(IOPORT):6, 5, 0x21)

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

asynSetOption ($(IOPORT):3, 0, "baud", $(BAUD=19200))
asynSetOption ($(IOPORT):3, 0, "bits", "8")
asynSetOption ($(IOPORT):3, 0, "parity", "even")
asynSetOption ($(IOPORT):3, 0, "stop", "1")
asynSetOption ($(IOPORT):3, -1, "clocal", "Y")
asynSetOption ($(IOPORT):3, -1, "crtscts", "N")

asynSetOption ($(IOPORT):4, 0, "baud", $(BAUD=19200))
asynSetOption ($(IOPORT):4, 0, "bits", "8")
asynSetOption ($(IOPORT):4, 0, "parity", "even")
asynSetOption ($(IOPORT):4, 0, "stop", "1")
asynSetOption ($(IOPORT):4, -1, "clocal", "Y")
asynSetOption ($(IOPORT):4, -1, "crtscts", "N")

asynSetOption ($(IOPORT):5, 0, "baud", $(BAUD=19200))
asynSetOption ($(IOPORT):5, 0, "bits", "8")
asynSetOption ($(IOPORT):5, 0, "parity", "even")
asynSetOption ($(IOPORT):5, 0, "stop", "1")
asynSetOption ($(IOPORT):5, -1, "clocal", "Y")
asynSetOption ($(IOPORT):5, -1, "crtscts", "N")

asynSetOption ($(IOPORT):6, 0, "baud", $(BAUD=19200))
asynSetOption ($(IOPORT):6, 0, "bits", "8")
asynSetOption ($(IOPORT):6, 0, "parity", "even")
asynSetOption ($(IOPORT):6, 0, "stop", "1")
asynSetOption ($(IOPORT):6, -1, "clocal", "Y")
asynSetOption ($(IOPORT):6, -1, "crtscts", "N")

iocInit
