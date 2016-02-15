#!../../bin/_TARGETARCH_/VQM_ITMS

## Register all support components
dbLoadDatabase ("../../dbd/VQM_ITMS.dbd")
VQM_ITMS_registerRecordDeviceDriver pdbbase

epicsEnvSet ASYNPORT $(ASYNPORT=VQM_ITMS)
epicsEnvSet ASYNPORT1 $(ASYNPORT1=$(ASYNPORT):1)
epicsEnvSet IOPORT PUMP

# Configure asyn communication port, first
VQM_ITMSPortDriverConfigure($(ASYNPORT), 1, $(NOOFPZD=6))
drvAsynSerialPortConfigure($(IOPORT):1, $(COMPORT1="/dev/ttyS0"), 0, 0, 0)
VQM_ITMSAddIOPort($(IOPORT):1)

## Load record instances
dbLoadRecords("../../VQM_ITMSApp/Db/SoftwareVersions.db", "P=$(ASYNPORT):,PORT=$(ASYNPORT)")

$(ASYN_VERSION_GE426=#) epicsEnvSet DB VQM_ITMS.Asyn4-26

dbLoadRecords("../../VQM_ITMSApp/Db/$(DB=VQM_ITMS).db", "P=$(ASYNPORT1):,PORT=$(ASYNPORT),SCAN=$(SCANINT=5 second),ADDR=0")
asynSetTraceMask($(IOPORT):1, 0, 0x21)

asynSetOption ($(IOPORT):1, 0, "baud", $(BAUD=19200))
asynSetOption ($(IOPORT):1, 0, "bits", "8")
asynSetOption ($(IOPORT):1, 0, "parity", "even")
asynSetOption ($(IOPORT):1, 0, "stop", "1")
asynSetOption ($(IOPORT):1, -1, "clocal", "Y")
asynSetOption ($(IOPORT):1, -1, "crtscts", "N")

iocInit
