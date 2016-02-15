#!../../bin/linux-x86_64/VQM_ITMS_Sim

## Register all support components
dbLoadDatabase ("../../dbd/VQM_ITMS_Sim.dbd")
VQM_ITMS_Sim_registerRecordDeviceDriver pdbbase

epicsEnvSet EPICS_CA_SERVER_PORT "5072"
epicsEnvSet ASYNSIMPORT $(ASYNSIMPORT=VQM_ITMS_SIMM)
epicsEnvSet ASYNSIMPORT1 $(ASYNSIMPORT1=$(ASYNSIMPORT):1)
epicsEnvSet ASYNSIMPORT2 $(ASYNSIMPORT2=$(ASYNSIMPORT):2)
epicsEnvSet ASYNSIMPORT3 $(ASYNSIMPORT3=$(ASYNSIMPORT):3)
epicsEnvSet ASYNSIMPORT4 $(ASYNSIMPORT4=$(ASYNSIMPORT):4)
epicsEnvSet ASYNSIMPORT5 $(ASYNSIMPORT5=$(ASYNSIMPORT):5)
epicsEnvSet ASYNSIMPORT6 $(ASYNSIMPORT6=$(ASYNSIMPORT):6)
epicsEnvSet IOPORT PUMP

# Configure asyn communication port, first
VQM_ITMS_SimPortDriverConfigure($(ASYNSIMPORT), 6, $(NOOFPZD="6"))
drvAsynIPServerPortConfigure($(IOPORT):1, $(IPPORT1="localhost:5066"), 1, 0, 0, 1)
VQM_ITMS_SimAddIOPort($(IOPORT):1)
drvAsynIPServerPortConfigure($(IOPORT):2, $(IPPORT2="localhost:5067"), 1, 0, 0, 1)
VQM_ITMS_SimAddIOPort($(IOPORT):2)
drvAsynIPServerPortConfigure($(IOPORT):3, $(IPPORT3="localhost:5068"), 1, 0, 0, 1)
VQM_ITMS_SimAddIOPort($(IOPORT):3)
drvAsynIPServerPortConfigure($(IOPORT):4, $(IPPORT4="localhost:5069"), 1, 0, 0, 1)
VQM_ITMS_SimAddIOPort($(IOPORT):4)
drvAsynIPServerPortConfigure($(IOPORT):5, $(IPPORT5="localhost:5070"), 1, 0, 0, 1)
VQM_ITMS_SimAddIOPort($(IOPORT):5)
drvAsynIPServerPortConfigure($(IOPORT):6, $(IPPORT6="localhost:5071"), 1, 0, 0, 1)
VQM_ITMS_SimAddIOPort($(IOPORT):6)

## Load record instances
dbLoadRecords("../../VQM_ITMSApp/Db/SoftwareVersions.db", "P=$(ASYNSIMPORT):,PORT=$(ASYNSIMPORT)")

$(ASYN_VERSION_GE426=#) epicsEnvSet SIMDB VQM_ITMS_Sim.Asyn4-26
dbLoadRecords("../../VQM_ITMS_SimApp/Db/$(SIMDB=VQM_ITMS_Sim).db", "P=$(ASYNSIMPORT1):,PORT=$(ASYNSIMPORT),ADDR=0")
asynSetTraceMask($(IOPORT):1, 0, 0x21)

dbLoadRecords("../../VQM_ITMS_SimApp/Db/$(SIMDB=VQM_ITMS_Sim).db", "P=$(ASYNSIMPORT2):,PORT=$(ASYNSIMPORT),ADDR=1")
asynSetTraceMask($(IOPORT):2, 1, 0x21)

dbLoadRecords("../../VQM_ITMS_SimApp/Db/$(SIMDB=VQM_ITMS_Sim).db", "P=$(ASYNSIMPORT3):,PORT=$(ASYNSIMPORT),ADDR=2")
asynSetTraceMask($(IOPORT):3, 2, 0x21)

dbLoadRecords("../../VQM_ITMS_SimApp/Db/$(SIMDB=VQM_ITMS_Sim).db", "P=$(ASYNSIMPORT4):,PORT=$(ASYNSIMPORT),ADDR=3")
asynSetTraceMask($(IOPORT):4, 3, 0x21)

dbLoadRecords("../../VQM_ITMS_SimApp/Db/$(SIMDB=VQM_ITMS_Sim).db", "P=$(ASYNSIMPORT5):,PORT=$(ASYNSIMPORT),ADDR=4")
asynSetTraceMask($(IOPORT):5, 4, 0x21)

dbLoadRecords("../../VQM_ITMS_SimApp/Db/$(SIMDB=VQM_ITMS_Sim).db", "P=$(ASYNSIMPORT6):,PORT=$(ASYNSIMPORT),ADDR=5")
asynSetTraceMask($(IOPORT):6, 5, 0x21)

iocInit
