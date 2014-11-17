#!../../bin/win32-x86/LeyboldSim

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
dbLoadRecords("db/LeyboldSim.db", "P=$(ASYNPORT):1:,PORT=$(ASYNPORT)")
dbLoadRecords("../../asyn/asyn/asynRecord/asynRecord.db","P=$(ASYNPORT):1:,R=asyn,PORT=$(ASYNPORT),ADDR=0,OMAX=80,IMAX=80")

iocInit

## Start any sequence programs
#seq sncxxx,"user=pheestHost"