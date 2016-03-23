#!../../bin/_TARGETARCH_/VQM_ITMS

## Register all support components
dbLoadDatabase ("../../dbd/VQM_ITMS.dbd")
VQM_ITMS__registerRecordDeviceDriver pdbbase

epicsEnvSet ASYNPORT $(ASYNPORT=VQM_ITMS)
epicsEnvSet ASYNPORT1 $(ASYNPORT1=$(ASYNPORT):1)

# Configure asyn communication port, first
VQM_ITMSPortDriverConfigure($(ASYNPORT), 1)
VQM_ITMSAddIOPort($(COMPORT1="COM5:"))

## Load record instances
dbLoadRecords("../../VQM_ITMS_App/Db/SoftwareVersions.db", "P=$(ASYNPORT):,PORT=$(ASYNPORT)")

$(ASYN_VERSION_GE426=#) epicsEnvSet DB VQM_ITMS.Asyn4-26

dbLoadRecords("../../VQM_ITMS_App/Db/$(DB=VQM_ITMS).db", "P=$(ASYNPORT1):,PORT=$(ASYNPORT),SCAN=$(SCANINT=5 second),ADDR=0")

iocInit
