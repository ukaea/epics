#!../../bin/linux-x86_64/VarianTurboPump

## You may have to change VarianTurboPump to something else
## everywhere it appears in this file

< envPaths

epicsEnvSet IOCSH_PS1 "Varian-Agilent Turbopump> "
epicsEnvSet STREAM_PROTOCOL_PATH ".:../../db"

## Register all support components
dbLoadDatabase "../../dbd/VarianTurboPump.dbd"
VarianTurboPump_registerRecordDeviceDriver pdbbase

drvAsynIPPortConfigure ("L1", "127.0.0.1:9001 tcp")
drvAsynIPPortConfigure ("L2", "127.0.0.1:9002 tcp")
drvAsynIPPortConfigure ("L3", "127.0.0.1:9003 tcp")

## Load record instances
dbLoadRecords("../../templates/Varian-Agilent_Turbopump.template","P=TMP2025-01-01,PORT=L1,PROTO=Varian_TV81,EVFAST=10,EVSLOW=11,EVSINGLE=12")
dbLoadRecords("../../templates/Varian-Agilent_ErrorA.template","P=TMP2025-01-01")

dbLoadRecords("../../templates/Varian-Agilent_Turbopump.template","P=TMP2025-01-02,PORT=L2,PROTO=Varian_TV81,EVFAST=13,EVSLOW=14,EVSINGLE=15")
dbLoadRecords("../../templates/Varian-Agilent_ErrorA.template","P=TMP2025-01-02")

dbLoadRecords("../../templates/Varian-Agilent_Turbopump.template","P=TMP2025-01-03,PORT=L3,PROTO=Varian_TT304,EVFAST=16,EVSLOW=17,EVSINGLE=18")
dbLoadRecords("../../templates/Varian-Agilent_ErrorA.template","P=TMP2025-01-03")

iocInit

## Start any sequence programs
#seq sncxxx,"user=igarasrHost"
