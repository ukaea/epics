< envPaths
errlogInit(20000)

dbLoadDatabase("$(AREA_DETECTOR)/dbd/PSLApp.dbd")
PSLApp_registerRecordDeviceDriver(pdbbase) 

epicsEnvSet("PREFIX", "PSL1:")
epicsEnvSet("PORT",   "PSL")
epicsEnvSet("QSIZE",  "20")
epicsEnvSet("XSIZE",  "4007")
epicsEnvSet("YSIZE",  "2670")
epicsEnvSet("NCHANS", "2048")

###
# Create the asyn port to talk to the PSL server on port 50000
# Set noAutoConnect and noProcessEos
drvAsynIPPortConfigure("PSLServer","10.0.4.190:50000", 0, 0, 0)
asynOctetSetOutputEos("PSLServer", 0, "\n")

asynSetTraceIOMask("PSLServer",0,2)
#asynSetTraceMask("PSLServer",0,9)
#asynSetTraceFile("PSLServer", 0, "PSLServer.out")


# Need to open connection to camera - edit the camera name as needed
# Note: subsequent running of the IOC will get an error that the camera is already open,
# but that is OK
asynOctetConnect PSLServer PSLServer 0 10
asynOctetWriteRead PSLServer "Open;VHR11"

# PSLConfig(const char *portName, const char *serverPort, 
#          int maxBuffers, size_t maxMemory,
#          int priority, int stackSize)
PSLConfig("$(PORT)", "PSLServer", 0, 0)
asynSetTraceIOMask("$(PORT)",0,2)
#asynSetTraceMask("$(PORT)",0,9)

dbLoadRecords("$(AREA_DETECTOR)/ADApp/Db/ADBase.template","P=$(PREFIX),R=cam1:,PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(AREA_DETECTOR)/ADApp/Db/NDFile.template","P=$(PREFIX),R=cam1:,PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(AREA_DETECTOR)/ADApp/Db/PSL.template",   "P=$(PREFIX),R=cam1:,PORT=$(PORT),ADDR=0,TIMEOUT=1")

# Create a standard arrays plugin
NDStdArraysConfigure("Image1", 5, 0, "$(PORT)", 0, 0)
dbLoadRecords("$(AREA_DETECTOR)/ADApp/Db/NDPluginBase.template","P=$(PREFIX),R=image1:,PORT=Image1,ADDR=0,TIMEOUT=1,NDARRAY_PORT=$(PORT),NDARRAY_ADDR=0")
# Make NELEMENTS in the following be a little bigger than 4007*2670
dbLoadRecords("$(AREA_DETECTOR)/ADApp/Db/NDStdArrays.template", "P=$(PREFIX),R=image1:,PORT=Image1,ADDR=0,TIMEOUT=1,TYPE=Int16,FTVL=SHORT,NELEMENTS=11000000")

# Load all other plugins using commonPlugins.cmd
< ../commonPlugins.cmd


iocInit()

# save things every thirty seconds
create_monitor_set("auto_settings.req", 30,"P=$(PREFIX),D=cam1:")
