#!../../bin/windows-x64-debug/beers

## You may have to change bottles to something else
## everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "db/beers.dbd"
beers_registerRecordDeviceDriver pdbbase

## Load record instances
dbLoadTemplate "db/user.substitutions"
dbLoadRecords "db/beer.db", "user=ktn98257"

cd "${TOP}/iocBoot/${IOC}"
iocInit
