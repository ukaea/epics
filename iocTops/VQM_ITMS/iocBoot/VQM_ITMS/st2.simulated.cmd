#!../../bin/linux-x86_64/VQM_ITMS

## Register all support components
dbLoadDatabase ("../../dbd/VQM_ITMS.dbd")
VQM_ITMS_registerRecordDeviceDriver pdbbase

epicsEnvSet ASYNPORT $(ASYNPORT=VQM_ITMS)
epicsEnvSet IOPORT PUMP

# Configure asyn communication port, first
VQM_ITMSPortDriverConfigure($(ASYNPORT), 2, $(NOOFPZD="6"))
drvAsynIPPortConfigure($(IOPORT):1, $(IPPORT1="localhost:5066"), 0, 0)
VQM_ITMSAddIOPort($(IOPORT):1)
drvAsynIPPortConfigure($(IOPORT):2, $(IPPORT2="localhost:5067"), 0, 0)
VQM_ITMSAddIOPort($(IOPORT):2)

## Load record instances

$(ASYN_VERSION_GE426=#) epicsEnvSet DB VQM_ITMS.Asyn4-26

dbLoadRecords("../../VQM_ITMSApp/Db/SoftwareVersions.db", "P=$(ASYNPORT):,PORT=$(ASYNPORT)")
asynSetTraceMask($(IOPORT):1, 0, 0x21)

dbLoadRecords("../../VQM_ITMSApp/Db/$(DB=VQM_ITMS).db", "P=$(ASYNPORT):1:,PORT=$(ASYNPORT),SCAN=$(SCANINT=.1 second),ADDR=0")
asynSetTraceMask($(IOPORT):2, 1, 0x21)

dbLoadRecords("../../VQM_ITMSApp/Db/$(DB=VQM_ITMS).db", "P=$(ASYNPORT):2:,PORT=$(ASYNPORT),SCAN=$(SCANINT=.1 second),ADDR=1")
asynSetTraceMask($(IOPORT):3, 2, 0x21)

iocInit
