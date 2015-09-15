#!../bin/linux-x86_64/LeyboldSim

## Register all support components
dbLoadDatabase ("../dbd/LeyboldSim.dbd")
LeyboldSim_registerRecordDeviceDriver pdbbase

epicsEnvSet EPICS_CA_SERVER_PORT "5071"
epicsEnvSet ASYNSIMPORT $(ASYNSIMPORT=LEYBOLDTURBOSIM)
epicsEnvSet ASYNSIMPORT1 $(ASYNSIMPORT1=$(ASYNSIMPORT):1)
epicsEnvSet ASYNSIMPORT2 $(ASYNSIMPORT2=$(ASYNSIMPORT):2)
epicsEnvSet ASYNSIMPORT3 $(ASYNSIMPORT3=$(ASYNSIMPORT):3)
epicsEnvSet ASYNSIMPORT4 $(ASYNSIMPORT4=$(ASYNSIMPORT):4)
epicsEnvSet ASYNSIMPORT5 $(ASYNSIMPORT5=$(ASYNSIMPORT):5)
epicsEnvSet IOPORT PUMP

# Configure asyn communication port, first
LeyboldSimPortDriverConfigure($(ASYNSIMPORT), 5, $(NOOFPZD="6"))
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
dbLoadRecords("../LeyboldSimApp/Db/$(DB=LeyboldSim).db", "P=$(ASYNSIMPORT1):,PORT=$(ASYNSIMPORT),ADDR=0")

dbLoadRecords("../LeyboldSimApp/Db/$(DB=LeyboldSim).db", "P=$(ASYNSIMPORT2):,PORT=$(ASYNSIMPORT),ADDR=1")

dbLoadRecords("../LeyboldSimApp/Db/$(DB=LeyboldSim).db", "P=$(ASYNSIMPORT3):,PORT=$(ASYNSIMPORT),ADDR=2")

dbLoadRecords("../LeyboldSimApp/Db/$(DB=LeyboldSim).db", "P=$(ASYNSIMPORT4):,PORT=$(ASYNSIMPORT),ADDR=3")

dbLoadRecords("../LeyboldSimApp/Db/$(DB=LeyboldSim).db", "P=$(ASYNSIMPORT5):,PORT=$(ASYNSIMPORT),ADDR=4")

iocInit

## Start any sequence programs
#seq sncxxx,"user=pheestHost"
