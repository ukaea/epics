#!../bin/linux-x86_64/LeyboldTurbo

## Register all support components
dbLoadDatabase ("../dbd/LeyboldTurbo.dbd")
LeyboldTurbo_registerRecordDeviceDriver pdbbase

epicsEnvSet ASYNPORT LEYBOLDTURBO
epicsEnvSet IOPORT PUMP

# Configure asyn communication port, first
LeyboldTurboPortDriverConfigure($(ASYNPORT), 5, $(NOOFPZD="6"))
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

## Load record instances
dbLoadRecords("../LeyboldTurboApp/Db/$(DB=LeyboldTurbo).db", "P=$(ASYNPORT):1:,PORT=$(ASYNPORT),ADDR=0")

dbLoadRecords("../LeyboldTurboApp/Db/$(DB=LeyboldTurbo).db", "P=$(ASYNPORT):2:,PORT=$(ASYNPORT),ADDR=1")

dbLoadRecords("../LeyboldTurboApp/Db/$(DB=LeyboldTurbo).db", "P=$(ASYNPORT):3:,PORT=$(ASYNPORT),ADDR=2")

dbLoadRecords("../LeyboldTurboApp/Db/$(DB=LeyboldTurbo).db", "P=$(ASYNPORT):4:,PORT=$(ASYNPORT),ADDR=3")

dbLoadRecords("../LeyboldTurboApp/Db/$(DB=LeyboldTurbo).db", "P=$(ASYNPORT):5:,PORT=$(ASYNPORT),ADDR=4")

iocInit

## Start any sequence programs
#seq sncxxx,"user=pheestHost"
