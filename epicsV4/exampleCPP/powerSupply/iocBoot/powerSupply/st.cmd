< envPaths

cd ${TOP}

## Register all support components
dbLoadDatabase("dbd/powerSupply.dbd")
powerSupply_registerRecordDeviceDriver(pdbbase)

## Load record instances

dbLoadRecords("db/dbCounter.db","name=powerSupplyCounter");

cd ${TOP}/iocBoot/${IOC}
iocInit()
startPVAServer
powerSupplyCreateRecord powerSupply
