< envPaths

cd ${TOP}

## Register all support components
dbLoadDatabase("dbd/exampleServer.dbd")
exampleServer_registerRecordDeviceDriver(pdbbase)

## Load record instances
dbLoadRecords("db/dbScalar.db","name=pvdouble,type=ao")
dbLoadRecords("db/dbArray.db","name=pvdoubleArray,type=DOUBLE")
dbLoadRecords("db/dbStringArray.db","name=pvstringArray")
dbLoadRecords("db/dbEnum.db","name=pvenum")
dbLoadRecords("db/dbCounter.db","name=pvcounter");

cd ${TOP}/iocBoot/${IOC}
iocInit()
dbl
epicsThreadSleep(1.0)
exampleServerCreateRecord exampleServer
startPVAServer
pvdbl

