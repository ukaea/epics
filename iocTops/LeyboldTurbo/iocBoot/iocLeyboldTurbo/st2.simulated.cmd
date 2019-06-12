#!../../bin/linux-x86_64/LeyboldTurbo

## Register all support components
dbLoadDatabase ("../../dbd/LeyboldTurbo.dbd")
LeyboldTurbo_registerRecordDeviceDriver pdbbase

epicsEnvSet ASYNPORT $(ASYNPORT=LEYBOLDTURBO)
epicsEnvSet IOPORT PUMP

# Configure asyn communication port, first
LeyboldTurboPortDriverConfigure($(ASYNPORT), 2, $(NOOFPZD="6"))
drvAsynIPPortConfigure($(IOPORT):1, $(IPPORT1="localhost:5066"), 0, 0)
LeyboldTurboAddIOPort($(IOPORT):1)
drvAsynIPPortConfigure($(IOPORT):2, $(IPPORT2="localhost:5067"), 0, 0)
LeyboldTurboAddIOPort($(IOPORT):2)

## Load record instances

dbLoadRecords("../../LeyboldTurboApp/Db/SoftwareVersions.db", "P=$(ASYNPORT):,PORT=$(ASYNPORT)")
asynSetTraceMask($(IOPORT):1, 0, 0x21)

dbLoadRecords("../../LeyboldTurboApp/Db/$(DB=LeyboldTurbo).db", "P=$(ASYNPORT):1:,PORT=$(ASYNPORT),SCAN=$(SCANINT=.1 second),ADDR=0")
asynSetTraceMask($(IOPORT):2, 1, 0x21)

dbLoadRecords("../../LeyboldTurboApp/Db/$(DB=LeyboldTurbo).db", "P=$(ASYNPORT):2:,PORT=$(ASYNPORT),SCAN=$(SCANINT=.1 second),ADDR=1")
asynSetTraceMask($(IOPORT):3, 2, 0x21)

iocInit
