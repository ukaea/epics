#!../../bin/linux-x86_64/VQM_ITMS

## Register all support components
dbLoadDatabase ("../../dbd/VQM_ITMS.dbd")
VQM_ITMS_registerRecordDeviceDriver pdbbase

epicsEnvSet ASYNPORT $(ASYNPORT=VQM_ITMS)
epicsEnvSet ASYNPORT1 $(ASYNPORT1=$(ASYNPORT):1)
epicsEnvSet ASYNPORT2 $(ASYNPORT2=$(ASYNPORT):2)
epicsEnvSet ASYNPORT3 $(ASYNPORT3=$(ASYNPORT):3)
epicsEnvSet ASYNPORT4 $(ASYNPORT4=$(ASYNPORT):4)
epicsEnvSet ASYNPORT5 $(ASYNPORT5=$(ASYNPORT):5)
epicsEnvSet ASYNPORT6 $(ASYNPORT6=$(ASYNPORT):6)

epicsEnvSet IOPORT PUMP

# Configure asyn communication port, first
VQM_ITMSPortDriverConfigure($(ASYNPORT), 6, $(NOOFPZD="6"))
drvAsynIPPortConfigure($(IOPORT):1, $(IPPORT1="localhost:5066"), 0, 0)
VQM_ITMSAddIOPort($(IOPORT):1)
drvAsynIPPortConfigure($(IOPORT):2, $(IPPORT2="localhost:5067"), 0, 0)
VQM_ITMSAddIOPort($(IOPORT):2)
drvAsynIPPortConfigure($(IOPORT):3, $(IPPORT3="localhost:5068"), 0, 0)
VQM_ITMSAddIOPort($(IOPORT):3)
drvAsynIPPortConfigure($(IOPORT):4, $(IPPORT4="localhost:5069"), 0, 0)
VQM_ITMSAddIOPort($(IOPORT):4)
drvAsynIPPortConfigure($(IOPORT):5, $(IPPORT5="localhost:5070"), 0, 0)
VQM_ITMSAddIOPort($(IOPORT):5)
drvAsynIPPortConfigure($(IOPORT):6, $(IPPORT6="localhost:5071"), 0, 0)
VQM_ITMSAddIOPort($(IOPORT):6)

## Load record instances
#Required before loadinhg the database because it can initialise ASYN_VERSION_GE426

dbLoadRecords("../../VQM_ITMSApp/Db/SoftwareVersions.db", "P=$(ASYNPORT):,PORT=$(ASYNPORT)")

$(ASYN_VERSION_GE426=#) epicsEnvSet DB VQM_ITMS.Asyn4-26
dbLoadRecords("../../VQM_ITMSApp/Db/$(DB=VQM_ITMS).db", "P=$(ASYNPORT1):,PORT=$(ASYNPORT),SCAN=$(SCANINT=.1 second),ADDR=0")
asynSetTraceMask($(IOPORT):1, 0, 0x21)

dbLoadRecords("../../VQM_ITMSApp/Db/$(DB=VQM_ITMS).db", "P=$(ASYNPORT2):,PORT=$(ASYNPORT),SCAN=$(SCANINT=.1 second),ADDR=1")
asynSetTraceMask($(IOPORT):2, 1, 0x21)

dbLoadRecords("../../VQM_ITMSApp/Db/$(DB=VQM_ITMS).db", "P=$(ASYNPORT3):,PORT=$(ASYNPORT),SCAN=$(SCANINT=.1 second),ADDR=2")
asynSetTraceMask($(IOPORT):3, 2, 0x21)

dbLoadRecords("../../VQM_ITMSApp/Db/$(DB=VQM_ITMS).db", "P=$(ASYNPORT4):,PORT=$(ASYNPORT),SCAN=$(SCANINT=.1 second),ADDR=3")
asynSetTraceMask($(IOPORT):4, 3, 0x21)

dbLoadRecords("../../VQM_ITMSApp/Db/$(DB=VQM_ITMS).db", "P=$(ASYNPORT5):,PORT=$(ASYNPORT),SCAN=$(SCANINT=.1 second),ADDR=4")
asynSetTraceMask($(IOPORT):5, 4, 0x21)

dbLoadRecords("../../VQM_ITMSApp/Db/$(DB=VQM_ITMS).db", "P=$(ASYNPORT6):,PORT=$(ASYNPORT),SCAN=$(SCANINT=.1 second),ADDR=5")
asynSetTraceMask($(IOPORT):6, 5, 0x21)

iocInit
