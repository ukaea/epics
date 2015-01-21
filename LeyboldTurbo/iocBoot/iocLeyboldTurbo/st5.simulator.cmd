#!../../bin/win32-x86/LeyboldSim

## You may have to change LeyboldSim to something else
## everywhere it appears in this file

## Register all support components
dbLoadDatabase ("../dbd/LeyboldSim.dbd")
LeyboldSim_registerRecordDeviceDriver pdbbase

epicsEnvSet ASYNPORT TURBOSIM
epicsEnvSet IOPORT PUMP

# Configure asyn communication port, first
LeyboldSimPortDriverConfigure($(ASYNPORT), 5)
drvAsynIPServerPortConfigure($(IOPORT):1, $(IPPORT1), 1, 0, 0, 1)
LeyboldSimAddIOPort($(IOPORT):1)
drvAsynIPServerPortConfigure($(IOPORT):2, $(IPPORT2), 1, 0, 0, 1)
LeyboldSimAddIOPort($(IOPORT):2)
drvAsynIPServerPortConfigure($(IOPORT):3, $(IPPORT3), 1, 0, 0, 1)
LeyboldSimAddIOPort($(IOPORT):3)
drvAsynIPServerPortConfigure($(IOPORT):4, $(IPPORT4), 1, 0, 0, 1)
LeyboldSimAddIOPort($(IOPORT):4)
drvAsynIPServerPortConfigure($(IOPORT):5, $(IPPORT5), 1, 0, 0, 1)
LeyboldSimAddIOPort($(IOPORT):5)

## Load record instances
<<<<<<< HEAD
dbLoadRecords("Db/LeyboldSim.db", "P=$(ASYNPORT):1:,PORT=$(ASYNPORT)")

dbLoadRecords("Db/LeyboldSim.db", "P=$(ASYNPORT):2:,PORT=$(ASYNPORT)")

dbLoadRecords("Db/LeyboldSim.db", "P=$(ASYNPORT):3:,PORT=$(ASYNPORT)")

dbLoadRecords("Db/LeyboldSim.db", "P=$(ASYNPORT):4:,PORT=$(ASYNPORT)")

dbLoadRecords("Db/LeyboldSim.db", "P=$(ASYNPORT):5:,PORT=$(ASYNPORT)")
=======
dbLoadRecords("Db/LeyboldSim.db", "P=$(ASYNPORT):1:,PORT=$(ASYNPORT),ADDR=0")

dbLoadRecords("Db/LeyboldSim.db", "P=$(ASYNPORT):2:,PORT=$(ASYNPORT),ADDR=1")

dbLoadRecords("Db/LeyboldSim.db", "P=$(ASYNPORT):3:,PORT=$(ASYNPORT),ADDR=2")

dbLoadRecords("Db/LeyboldSim.db", "P=$(ASYNPORT):4:,PORT=$(ASYNPORT),ADDR=3")

dbLoadRecords("Db/LeyboldSim.db", "P=$(ASYNPORT):5:,PORT=$(ASYNPORT),ADDR=4")
>>>>>>> 9f0f9076504863821233a7e4d407c3f5844835a0

iocInit

## Start any sequence programs
#seq sncxxx,"user=pheestHost"
