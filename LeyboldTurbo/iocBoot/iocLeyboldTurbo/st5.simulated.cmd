#!../../bin/win32-x86/LeyboldTurbo

## You may have to change LeyboldTurbo to something else
## everywhere it appears in this file

## Register all support components
dbLoadDatabase ("../dbd/LeyboldTurbo.dbd")
LeyboldTurbo_registerRecordDeviceDriver pdbbase

epicsEnvSet ASYNPORT TURBO
epicsEnvSet IOPORT PUMP

# Configure asyn communication port, first
LeyboldTurboPortDriverConfigure($(ASYNPORT), 5)
drvAsynIPPortConfigure($(IOPORT):1, $(IPPORT1), 0, 0)
LeyboldTurboAddIOPort($(IOPORT):1)
drvAsynIPPortConfigure($(IOPORT):2, $(IPPORT2), 0, 0)
LeyboldTurboAddIOPort($(IOPORT):2)
drvAsynIPPortConfigure($(IOPORT):3, $(IPPORT3), 0, 0)
LeyboldTurboAddIOPort($(IOPORT):3)
drvAsynIPPortConfigure($(IOPORT):4, $(IPPORT4), 0, 0)
LeyboldTurboAddIOPort($(IOPORT):4)
drvAsynIPPortConfigure($(IOPORT):5, $(IPPORT5), 0, 0)
LeyboldTurboAddIOPort($(IOPORT):5)

## Load record instances
dbLoadRecords("Db/LeyboldTurbo.db", "P=$(ASYNPORT):1:,PORT=$(ASYNPORT),ADDR=0")

dbLoadRecords("Db/LeyboldTurbo.db", "P=$(ASYNPORT):2:,PORT=$(ASYNPORT),ADDR=1")

dbLoadRecords("Db/LeyboldTurbo.db", "P=$(ASYNPORT):3:,PORT=$(ASYNPORT),ADDR=2")

dbLoadRecords("Db/LeyboldTurbo.db", "P=$(ASYNPORT):4:,PORT=$(ASYNPORT),ADDR=3")

dbLoadRecords("Db/LeyboldTurbo.db", "P=$(ASYNPORT):5:,PORT=$(ASYNPORT),ADDR=4")

iocInit

## Start any sequence programs
#seq sncxxx,"user=pheestHost"
