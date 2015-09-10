#!../bin/linux-x86_64/LeyboldSim

## Register all support components
dbLoadDatabase ("../dbd/LeyboldSim.dbd")
LeyboldSim_registerRecordDeviceDriver pdbbase

epicsEnvSet EPICS_CA_SERVER_PORT "5071"
epicsEnvSet ASYNPORT LEYBOLDTURBOSIM
epicsEnvSet IOPORT PUMP

# Configure asyn communication port, first
LeyboldSimPortDriverConfigure($(ASYNPORT), 5, $(NOOFPZD="6"))
drvAsynIPServerPortConfigure($(IOPORT):1, $(IPPORT1="localhost:5066"), 1, 0, 0, 1)
LeyboldSimAddIOPort($(IOPORT):1)
drvAsynIPServerPortConfigure($(IOPORT):2, $(IPPORT2="localhost:5067"), 1, 0, 0, 1)
LeyboldSimAddIOPort($(IOPORT):2)
drvAsynIPServerPortConfigure($(IOPORT):3, $(IPPORT3="localhost:5068"), 1, 0, 0, 1)
LeyboldSimAddIOPort($(IOPORT):3)
drvAsynIPServerPortConfigure($(IOPORT):4, $(IPPORT4="localhost:5069"), 1, 0, 0, 1)
LeyboldSimAddIOPort($(IOPORT):4)
drvAsynIPServerPortConfigure($(IOPORT):5, $(IPPORT5="localhost:5070"), 1, 0, 0, 1)
LeyboldSimAddIOPort($(IOPORT):5)

## Load record instances
dbLoadRecords("../LeyboldSimApp/Db/$(DB=LeyboldSim).db", "P=$(ASYNPORT):1:,PORT=$(ASYNPORT),ADDR=0")

dbLoadRecords("../LeyboldSimApp/Db/$(DB=LeyboldSim).db", "P=$(ASYNPORT):2:,PORT=$(ASYNPORT),ADDR=1")

dbLoadRecords("../LeyboldSimApp/Db/$(DB=LeyboldSim).db", "P=$(ASYNPORT):3:,PORT=$(ASYNPORT),ADDR=2")

dbLoadRecords("../LeyboldSimApp/Db/$(DB=LeyboldSim).db", "P=$(ASYNPORT):4:,PORT=$(ASYNPORT),ADDR=3")

dbLoadRecords("../LeyboldSimApp/Db/$(DB=LeyboldSim).db", "P=$(ASYNPORT):5:,PORT=$(ASYNPORT),ADDR=4")

iocInit

## Start any sequence programs
#seq sncxxx,"user=pheestHost"
