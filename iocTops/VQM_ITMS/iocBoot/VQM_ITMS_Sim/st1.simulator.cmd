#!../../bin/linux-x86_64/VQM_ITMS_Sim

## Register all support components
dbLoadDatabase ("../../dbd/VQM_ITMS_Sim.dbd")
VQM_ITMS_Sim_registerRecordDeviceDriver pdbbase

epicsEnvSet EPICS_CA_SERVER_PORT "5072"
epicsEnvSet ASYNSIMPORT $(ASYNSIMPORT=VQM_ITMS_SIMM)
epicsEnvSet ASYNSIMPORT1 $(ASYNSIMPORT1=$(ASYNSIMPORT):1)
epicsEnvSet IOPORT PUMP

# Configure asyn communication port, first
VQM_ITMS_SimPortDriverConfigure($(ASYNSIMPORT), 1, $(NOOFPZD="6"))
drvAsynIPServerPortConfigure($(IOPORT):1, $(IPPORT1="localhost:5066"), 1, 0, 0, 1)
VQM_ITMS_SimAddIOPort($(IOPORT):1)

## Load record instances
dbLoadRecords("../../VQM_ITMSApp/Db/SoftwareVersions.db", "P=$(ASYNSIMPORT):,PORT=$(ASYNSIMPORT)")

$(ASYN_VERSION_GE426=#) epicsEnvSet SIMDB VQM_ITMS_Sim.Asyn4-26
dbLoadRecords("../../VQM_ITMS_SimApp/Db/$(SIMDB=VQM_ITMS_Sim).db", "P=$(ASYNSIMPORT1):,PORT=$(ASYNSIMPORT),ADDR=0")
asynSetTraceMask($(IOPORT):1, 0, 0x21)

iocInit
