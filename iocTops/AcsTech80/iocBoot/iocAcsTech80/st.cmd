#!../../bin/linux-x86_64/acsTech80

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/acsTech80.dbd"
acsTech80_registerRecordDeviceDriver pdbbase

cd "${TOP}/iocBoot/${IOC}"

## motorUtil (allstop & alldone)
dbLoadRecords("$(MOTOR)/db/motorUtil.db", "P=acsTech80:")

## 
< SPiiPlus.cmd

iocInit

## motorUtil (allstop & alldone)
motorUtilInit("acsTech80:")

# Boot complete
