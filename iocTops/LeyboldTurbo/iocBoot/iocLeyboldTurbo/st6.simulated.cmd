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
drvAsynIPPortConfigure($(IOPORT):1, $(IPPORT1="localhost:5066"), 0, 0)
LeyboldTurboAddIOPort($(IOPORT):1)
drvAsynIPPortConfigure($(IOPORT):2, $(IPPORT2="localhost:5067"), 0, 0)
LeyboldTurboAddIOPort($(IOPORT):2)
drvAsynIPPortConfigure($(IOPORT):3, $(IPPORT3="localhost:5068"), 0, 0)
LeyboldTurboAddIOPort($(IOPORT):3)
drvAsynIPPortConfigure($(IOPORT):4, $(IPPORT4="localhost:5069"), 0, 0)
LeyboldTurboAddIOPort($(IOPORT):4)
drvAsynIPPortConfigure($(IOPORT):5, $(IPPORT5="localhost:5070"), 0, 0)
LeyboldTurboAddIOPort($(IOPORT):5)
drvAsynIPPortConfigure($(IOPORT):6, $(IPPORT6="localhost:5071"), 0, 0)
LeyboldTurboAddIOPort($(IOPORT):6)

## Load record instances

dbLoadRecords("../../LeyboldTurboApp/Db/SoftwareVersions.db", "P=$(ASYNPORT):,PORT=$(ASYNPORT)")

dbLoadRecords("../../LeyboldTurboApp/Db/$(DB=LeyboldTurbo).db", "P=$(ASYNPORT1):,PORT=$(ASYNPORT),SCAN=$(SCANINT=.1 second),ADDR=0")
asynSetTraceMask($(IOPORT):1, 0, 0x21)

dbLoadRecords("../../LeyboldTurboApp/Db/$(DB=LeyboldTurbo).db", "P=$(ASYNPORT2):,PORT=$(ASYNPORT),SCAN=$(SCANINT=.1 second),ADDR=1")
asynSetTraceMask($(IOPORT):2, 1, 0x21)

dbLoadRecords("../../LeyboldTurboApp/Db/$(DB=LeyboldTurbo).db", "P=$(ASYNPORT3):,PORT=$(ASYNPORT),SCAN=$(SCANINT=.1 second),ADDR=2")
asynSetTraceMask($(IOPORT):3, 2, 0x21)

dbLoadRecords("../../LeyboldTurboApp/Db/$(DB=LeyboldTurbo).db", "P=$(ASYNPORT4):,PORT=$(ASYNPORT),SCAN=$(SCANINT=.1 second),ADDR=3")
asynSetTraceMask($(IOPORT):4, 3, 0x21)

dbLoadRecords("../../LeyboldTurboApp/Db/$(DB=LeyboldTurbo).db", "P=$(ASYNPORT5):,PORT=$(ASYNPORT),SCAN=$(SCANINT=.1 second),ADDR=4")
asynSetTraceMask($(IOPORT):5, 4, 0x21)

dbLoadRecords("../../LeyboldTurboApp/Db/$(DB=LeyboldTurbo).db", "P=$(ASYNPORT6):,PORT=$(ASYNPORT),SCAN=$(SCANINT=.1 second),ADDR=5")
asynSetTraceMask($(IOPORT):6, 5, 0x21)

iocInit
