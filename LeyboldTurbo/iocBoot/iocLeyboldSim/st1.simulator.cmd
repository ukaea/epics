#!../../bin/linux-x86_64/LeyboldSim

## You may have to change LeyboldSim to something else
## everywhere it appears in this file

## Register all support components
dbLoadDatabase ("../dbd/LeyboldSim.dbd")
LeyboldSim_registerRecordDeviceDriver pdbbase

epicsEnvSet ASYNPORT LEYBOLDTURBOSIM
epicsEnvSet IOPORT PUMP

# Configure asyn communication port, first
LeyboldSimPortDriverConfigure($(ASYNPORT), 1, $(NOOFPZD))
drvAsynIPServerPortConfigure($(IOPORT):1, $(IPPORT1), 1, 0, 0, 1)
LeyboldSimAddIOPort($(IOPORT):1)

## Load record instances
dbLoadRecords("Db/LeyboldSim.db", "P=$(ASYNPORT):1:,PORT=$(ASYNPORT),ADDR=0")

iocInit

## Start any sequence programs
#seq sncxxx,"user=pheestHost"
