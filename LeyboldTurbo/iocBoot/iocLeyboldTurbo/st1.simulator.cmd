<<<<<<< HEAD
#!../../bin/linux-x86_64/LeyboldSim
=======
#!../../bin/win32-x86/LeyboldSim
>>>>>>> 9f0f9076504863821233a7e4d407c3f5844835a0

## You may have to change LeyboldSim to something else
## everywhere it appears in this file

## Register all support components
dbLoadDatabase ("../dbd/LeyboldSim.dbd")
LeyboldSim_registerRecordDeviceDriver pdbbase

epicsEnvSet ASYNPORT TURBOSIM
epicsEnvSet IOPORT PUMP

# Configure asyn communication port, first
LeyboldSimPortDriverConfigure($(ASYNPORT), 1)
drvAsynIPServerPortConfigure($(IOPORT):1, $(IPPORT1), 1, 0, 0, 1)
LeyboldSimAddIOPort($(IOPORT):1)

## Load record instances
<<<<<<< HEAD
dbLoadRecords("Db/LeyboldSim.db", "P=$(ASYNPORT):1:,PORT=$(ASYNPORT)")
=======
dbLoadRecords("Db/LeyboldSim.db", "P=$(ASYNPORT):1:,PORT=$(ASYNPORT),ADDR=0")
>>>>>>> 9f0f9076504863821233a7e4d407c3f5844835a0

iocInit

## Start any sequence programs
#seq sncxxx,"user=pheestHost"
