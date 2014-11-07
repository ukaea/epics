#!../../bin/win32-x86/LeyboldTurbo

## You may have to change LeyboldTurbo to something else
## everywhere it appears in this file

## Register all support components
dbLoadDatabase ("../dbd/LeyboldTurbo.dbd")
LeyboldTurbo_registerRecordDeviceDriver pdbbase

epicsEnvSet ASYNPORT TURBO
epicsEnvSet IOPORT PUMP

# Configure asyn communication port, first
LeyboldTurboPortDriverConfigure($(ASYNPORT), 1)
drvAsynIPPortConfigure($(IOPORT):1, $(IPPORT1), 0, 0)
LeyboldTurboAddIOPort($(IOPORT):1)

## Load record instances
dbLoadRecords("db/LeyboldTurbo.db", "P=$(ASYNPORT):1:,PORT=$(ASYNPORT)")
dbLoadRecords("../../asyn/asyn/asynRecord/asynRecord.db","P=$(ASYNPORT):1:,R=asyn,PORT=$(ASYNPORT),ADDR=0,OMAX=80,IMAX=80")

iocInit

## Start any sequence programs
#seq sncxxx,"user=pheestHost"
