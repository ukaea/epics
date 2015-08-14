#!../../bin/linux-x86_64/LeyboldSim

## Register all support components
dbLoadDatabase ("../dbd/LeyboldSim.dbd")
LeyboldSim_registerRecordDeviceDriver pdbbase

epicsEnvSet ASYNPORT LEYBOLDTURBOSIM
epicsEnvSet IOPORT PUMP

# Configure asyn communication port, first
LeyboldSimPortDriverConfigure($(ASYNPORT), 2, $(NOOFPZD))
drvAsynIPServerPortConfigure($(IOPORT):1, $(IPPORT1), 1, 0, 0, 1)
LeyboldSimAddIOPort($(IOPORT):1)
drvAsynIPServerPortConfigure($(IOPORT):2, $(IPPORT2), 1, 0, 0, 1)
LeyboldSimAddIOPort($(IOPORT):2)

## Load record instances
dbLoadRecords("Db/LeyboldSim.db", "P=$(ASYNPORT):1:,PORT=$(ASYNPORT),ADDR=0")

dbLoadRecords("Db/LeyboldSim.db", "P=$(ASYNPORT):2:,PORT=$(ASYNPORT),ADDR=1")

iocInit

## Start any sequence programs
#seq sncxxx,"user=pheestHost"
