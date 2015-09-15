#!../bin/linux-x86_64/LeyboldSim

## Register all support components
dbLoadDatabase ("../dbd/LeyboldSim.dbd")
LeyboldSim_registerRecordDeviceDriver pdbbase

epicsEnvSet EPICS_CA_SERVER_PORT "5071"
epicsEnvSet ASYNSIMPORT $(ASYNSIMPORT=LEYBOLDTURBOSIM)
epicsEnvSet IOPORT PUMP

# Configure asyn communication port, first
LeyboldSimPortDriverConfigure($(ASYNSIMPORT), 2, $(NOOFPZD="6"))
drvAsynIPServerPortConfigure($(IOPORT):1, $(IPPORT1="localhost:5066"), 1, 0, 0, 1)
LeyboldSimAddIOPort($(IOPORT):1)
drvAsynIPServerPortConfigure($(IOPORT):2, $(IPPORT2="localhost:5067"), 1, 0, 0, 1)
LeyboldSimAddIOPort($(IOPORT):2)

## Load record instances
dbLoadRecords("../LeyboldSimApp/Db/$(DB=LeyboldSim).db", "P=$(ASYNSIMPORT):1:,PORT=$(ASYNSIMPORT),ADDR=0")

dbLoadRecords("../LeyboldSimApp/Db/$(DB=LeyboldSim).db", "P=$(ASYNSIMPORT):2:,PORT=$(ASYNSIMPORT),ADDR=1")

iocInit

## Start any sequence programs
#seq sncxxx,"user=pheestHost"
