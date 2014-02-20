..\bin\win32-x86\motorIMS %~nx0

# Gives (relative) path to the motor project root.
epicsEnvSet MOTOR ../

# Register all support components
dbLoadDatabase("../dbd/motorIMS.dbd")
motorIMS_registerRecordDeviceDriver pdbbase

# Motors substitutions, customize this for your motor
dbLoadTemplate (./db/motor.substitutions.tss.ims)
dbLoadRecords("$(MOTOR)db/motorUtil.db", "P=IOC:")

MDriveSetup(1, 10)
MDriveConfig(0, "M06")

# Configure asyn communication port, first
drvAsynSerialPortConfigure("M06", "COM3", 0, 0, 0)

asynSetOption("M06", -1, "baud", "9600")
asynSetOption("M06", -1, "bits", "8")
asynSetOption("M06", -1, "parity", "none")
asynSetOption("M06", -1, "stop", "1")
asynSetOption("M06", -1, "clocal", "Y")
asynSetOption("M06", -1, "crtscts", "N")

# Configure one controller per motor, each controller just has 1 axis
ImsMDrivePlusCreateController("IMS1", "M06", "", 200, 5000)

iocInit

motorUtilInit("IOC:")
# motorUtil (allstop & alldone)
