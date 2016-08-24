< envPaths
errlogInit(20000)

dbLoadDatabase("$(AREA_DETECTOR)/dbd/marCCDApp.dbd")
marCCDApp_registerRecordDeviceDriver(pdbbase) 

epicsEnvSet("PREFIX", "13MARCCD1:")
epicsEnvSet("PORT",   "MAR")
epicsEnvSet("QSIZE",  "20")
epicsEnvSet("XSIZE",  "2048")
epicsEnvSet("YSIZE",  "2048")
epicsEnvSet("NCHANS", "2048")

###
# Create the asyn port to talk to the MAR on port 2222
drvAsynIPPortConfigure("marServer","gse-marccd1.cars.aps.anl.gov:2222")
# Set the input and output terminators.
asynOctetSetInputEos("marServer", 0, "\n")
asynOctetSetOutputEos("marServer", 0, "\n")
#asynSetTraceMask("marServer",0,255)
asynSetTraceIOMask("marServer",0,2)

marCCDConfig("$(PORT)", "marServer", 20, 200000000)
dbLoadRecords("$(AREA_DETECTOR)/ADApp/Db/ADBase.template","P=$(PREFIX),R=cam1:,PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(AREA_DETECTOR)/ADApp/Db/NDFile.template","P=$(PREFIX),R=cam1:,PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(AREA_DETECTOR)/ADApp/Db/marCCD.template","P=$(PREFIX),R=cam1:,PORT=$(PORT),ADDR=0,TIMEOUT=1,MARSERVER_PORT=marServer")

# Create a standard arrays plugin
NDStdArraysConfigure("Image1", 5, 0, "$(PORT)", 0, 0)
dbLoadRecords("$(AREA_DETECTOR)/ADApp/Db/NDPluginBase.template","P=$(PREFIX),R=image1:,PORT=Image1,ADDR=0,TIMEOUT=1,NDARRAY_PORT=$(PORT),NDARRAY_ADDR=0")
# Make NELEMENTS in the following be a little bigger than 2048*2048
dbLoadRecords("$(AREA_DETECTOR)/ADApp/Db/NDStdArrays.template", "P=$(PREFIX),R=image1:,PORT=Image1,ADDR=0,TIMEOUT=1,TYPE=Int16,FTVL=SHORT,NELEMENTS=4200000")

# Load all other plugins using commonPlugins.cmd
< ../commonPlugins.cmd

#asynSetTraceMask("$(PORT)",0,3)
#asynSetTraceIOMask("$(PORT)",0,4)

iocInit()

# save things every thirty seconds
create_monitor_set("auto_settings.req", 30,"P=$(PREFIX),D=cam1:")
