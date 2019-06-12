#!../../bin/linux-x86_64/LeyboldSim

## Register all support components
dbLoadDatabase ("../../dbd/LeyboldSim.dbd")
LeyboldSim_registerRecordDeviceDriver pdbbase

epicsEnvSet EPICS_CA_SERVER_PORT "5072"
epicsEnvSet ASYNSIMPORT $(ASYNSIMPORT=LEYBOLDTURBOSIM)
epicsEnvSet ASYNSIMPORT1 $(ASYNSIMPORT1=$(ASYNSIMPORT):1)
epicsEnvSet ASYNSIMPORT2 $(ASYNSIMPORT2=$(ASYNSIMPORT):2)
epicsEnvSet IOPORT PUMP

# Configure asyn communication port, first
LeyboldSimPortDriverConfigure($(ASYNSIMPORT), 2, $(NOOFPZD="6"))
drvAsynIPServerPortConfigure($(IOPORT):1, $(IPPORT1="localhost:5066"), 1, 0, 0, 1)
LeyboldSimAddIOPort($(IOPORT):1)
drvAsynIPServerPortConfigure($(IOPORT):2, $(IPPORT2="localhost:5067"), 1, 0, 0, 1)
LeyboldSimAddIOPort($(IOPORT):2)

## Load record instances
dbLoadRecords("../../LeyboldTurboApp/Db/SoftwareVersions.db", "P=$(ASYNSIMPORT):,PORT=$(ASYNSIMPORT)")

dbLoadRecords("../../LeyboldSimApp/Db/$(SIMDB=LeyboldSim).db", "P=$(ASYNSIMPORT1):,PORT=$(ASYNSIMPORT),ADDR=0")
asynSetTraceMask($(IOPORT):1, 0, 0x21)

dbLoadRecords("../../LeyboldSimApp/Db/$(SIMDB=LeyboldSim).db", "P=$(ASYNSIMPORT2):,PORT=$(ASYNSIMPORT),ADDR=1")
asynSetTraceMask($(IOPORT):2, 1, 0x21)

iocInit
