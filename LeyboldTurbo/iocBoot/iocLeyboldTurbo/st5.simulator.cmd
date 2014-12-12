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
dbLoadRecords("Db/LeyboldSim.db", "P=$(ASYNPORT):1:,PORT=$(ASYNPORT)")
dbLoadRecords("../../asyn/asyn/asynRecord/asynRecord.db","P=$(ASYNPORT):1:,R=asyn,PORT=$(ASYNPORT),ADDR=0,OMAX=80,IMAX=80")

dbLoadRecords("Db/LeyboldSim.db", "P=$(ASYNPORT):2:,PORT=$(ASYNPORT)")
dbLoadRecords("../../asyn/asyn/asynRecord/asynRecord.db","P=$(ASYNPORT):2:,R=asyn,PORT=$(ASYNPORT),ADDR=0,OMAX=80,IMAX=80")

dbLoadRecords("Db/LeyboldSim.db", "P=$(ASYNPORT):3:,PORT=$(ASYNPORT)")
dbLoadRecords("../../asyn/asyn/asynRecord/asynRecord.db","P=$(ASYNPORT):3:,R=asyn,PORT=$(ASYNPORT),ADDR=0,OMAX=80,IMAX=80")

dbLoadRecords("Db/LeyboldSim.db", "P=$(ASYNPORT):4:,PORT=$(ASYNPORT)")
dbLoadRecords("../../asyn/asyn/asynRecord/asynRecord.db","P=$(ASYNPORT):4:,R=asyn,PORT=$(ASYNPORT),ADDR=0,OMAX=80,IMAX=80")

dbLoadRecords("Db/LeyboldSim.db", "P=$(ASYNPORT):5:,PORT=$(ASYNPORT)")
dbLoadRecords("../../asyn/asyn/asynRecord/asynRecord.db","P=$(ASYNPORT):5:,R=asyn,PORT=$(ASYNPORT),ADDR=0,OMAX=80,IMAX=80")

iocInit

## Start any sequence programs
#seq sncxxx,"user=pheestHost"
