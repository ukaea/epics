
## You may have to change VarianTurboPump to something else
## everywhere it appears in this file

epicsEnvSet IOCSH_PS1 "Varian-Agilent Turbopump> "
epicsEnvSet STREAM_PROTOCOL_PATH ".:../../db"

## Register all support components
dbLoadDatabase("../../dbd/VarianTurboPump.dbd",0,0)
VarianTurboPump_registerRecordDeviceDriver(pdbbase) 

drvAsynSerialPortConfigure ("L8","COM2")
asynSetOption ("L8", 0, "baud", "9600")
asynSetOption ("L8", 0, "bits", "8")
asynSetOption ("L8", 0, "parity", "none")
asynSetOption ("L8", 0, "stop", "1")
asynSetOption ("L8", 0, "clocal", "N")
asynSetOption ("L8", 0, "crtscts", "N")

## Load record instances
dbLoadRecords("../../db/VarianTurboPump.db")

## Apply security
#Replace X with name of CA client controlling this pump
asSetFilename("/VarianTurboPump/db/CCFE_VTP.acf")
asSetSubstitutions("client=X")

## Initialise IOC instance
iocInit()

