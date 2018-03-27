
## You may have to change VarianTurboPump to something else
## everywhere it appears in this file

epicsEnvSet IOCSH_PS1 "Varian-Agilent Turbopump NT10> "
epicsEnvSet STREAM_PROTOCOL_PATH ".:../../db"

## Register all support components
dbLoadDatabase("../../dbd/VarianTurboPump.dbd",0,0)
VarianTurboPump_registerRecordDeviceDriver(pdbbase) 

drvAsynSerialPortConfigure ("PORT:NT10","COM10")
asynSetOption ("PORT:NT10", 0, "baud", "9600")
asynSetOption ("PORT:NT10", 0, "bits", "8")
asynSetOption ("PORT:NT10", 0, "parity", "none")
asynSetOption ("PORT:NT10", 0, "stop", "1")
asynSetOption ("PORT:NT10", 0, "clocal", "N")
asynSetOption ("PORT:NT10", 0, "crtscts", "N")

## Load record instances
dbLoadRecords("../../db/VarianTurboPump.db","DVC=NT10,PORT=PORT:NT10")

## Apply security
#Replace X with name of CA client controlling this pump
asSetFilename("/VarianTurbo/db/CCFE_VTP.acf")
asSetSubstitutions("client=vc")

## Initialise IOC instance
iocInit()

