#!../../bin/linux-x86_64/LeyboldTurbo

## Register all support components
dbLoadDatabase ("../dbd/LeyboldTurbo.dbd")
LeyboldTurbo_registerRecordDeviceDriver pdbbase

epicsEnvSet ASYNPORT LEYBOLDTURBO
epicsEnvSet IOPORT PUMP

# Configure asyn communication port, first
LeyboldTurboPortDriverConfigure($(ASYNPORT), 2, $(NOOFPZD))
drvAsynIPPortConfigure($(IOPORT):1, $(IPPORT1), 0, 0)
LeyboldTurboAddIOPort($(IOPORT):1)
drvAsynIPPortConfigure($(IOPORT):2, $(IPPORT2), 0, 0)
LeyboldTurboAddIOPort($(IOPORT):2)

## Load record instances
dbLoadRecords("Db/LeyboldTurbo.db", "P=$(ASYNPORT):1:,PORT=$(ASYNPORT),ADDR=0")

dbLoadRecords("Db/LeyboldTurbo.db", "P=$(ASYNPORT):2:,PORT=$(ASYNPORT),ADDR=1")

iocInit

## Start any sequence programs
#seq sncxxx,"user=pheestHost"
