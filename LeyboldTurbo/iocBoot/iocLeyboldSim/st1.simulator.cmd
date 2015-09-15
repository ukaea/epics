#!../bin/linux-x86_64/LeyboldSim

## Register all support components
dbLoadDatabase ("../dbd/LeyboldSim.dbd")
LeyboldSim_registerRecordDeviceDriver pdbbase

epicsEnvSet EPICS_CA_SERVER_PORT "5071"
epicsEnvSet ASYNSIMPORT $(ASYNSIMPORT=LEYBOLDTURBOSIM)
epicsEnvSet IOPORT PUMP

# Configure asyn communication port, first
LeyboldSimPortDriverConfigure($(ASYNPORT), 1, $(NOOFPZD="6"))
drvAsynIPServerPortConfigure($(IOPORT):1, $(IPPORT1="localhost:5066"), 1, 0, 0, 1)
LeyboldSimAddIOPort($(IOPORT):1)

## Load record instances
dbLoadRecords("../LeyboldSimApp/Db/$(DB=LeyboldSim).db", "P=$(ASYNSIMPORT):1:,PORT=$(ASYNSIMPORT),ADDR=0")

iocInit

## Start any sequence programs
#seq sncxxx,"user=pheestHost"
