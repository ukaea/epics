< envPaths

cd ${TOP}

## Register all support components
dbLoadDatabase("dbd/exampleDatabase.dbd")
exampleDatabase_registerRecordDeviceDriver(pdbbase)

## Load record instances
dbLoadRecords("db/dbDouble.db","name=double00")
dbLoadRecords("db/dbDouble.db","name=double01")
dbLoadRecords("db/dbDouble.db","name=double02")
dbLoadRecords("db/dbDouble.db","name=double03")
dbLoadRecords("db/dbDouble.db","name=double04")
dbLoadRecords("db/dbDouble.db","name=double05")
dbLoadRecords("db/dbStringArray.db","name=stringArray01")
dbLoadRecords("db/dbEnum.db","name=enum01")
dbLoadRecords("db/dbCounter.db","name=counter01");
dbLoadRecords("db/dbArray.db","name=doubleArray,type=DOUBLE");


cd ${TOP}/iocBoot/${IOC}
iocInit()
dbl
epicsThreadSleep(2.0)
exampleDatabase
exampleMonitorPlugin
startPVAServer
pvdbl
