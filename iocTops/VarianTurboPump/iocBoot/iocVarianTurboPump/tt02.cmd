
## You may have to change VarianTurboPump to something else
## everywhere it appears in this file

epicsEnvSet IOCSH_PS1 "Varian-Agilent Turbopump TT02> "
epicsEnvSet STREAM_PROTOCOL_PATH ".:../../db"

## Register all support components
dbLoadDatabase("../../dbd/VarianTurboPump.dbd",0,0)
VarianTurboPump_registerRecordDeviceDriver(pdbbase) 

drvAsynSerialPortConfigure ("PORT:TT02","COM2")
asynSetOption ("PORT:TT02", 0, "baud", "9600")
asynSetOption ("PORT:TT02", 0, "bits", "8")
asynSetOption ("PORT:TT02", 0, "parity", "none")
asynSetOption ("PORT:TT02", 0, "stop", "1")
asynSetOption ("PORT:TT02", 0, "clocal", "N")
asynSetOption ("PORT:TT02", 0, "crtscts", "N")

## Load record instances
dbLoadRecords("../../db/VarianTurboPump.db","DVC=TT02,PORT=PORT:TT02")

## Apply security
#Replace X with name of CA client controlling this pump
asSetFilename("/VarianTurbo/db/CCFE_VTP.acf")
asSetSubstitutions("client=vc")

## Initialise IOC instance
iocInit()

