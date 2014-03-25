#!../../bin/win32-x86/withAsyn

# Gives (relative) path to the motor project root.
epicsEnvSet MOTOR ../../motorApp

# Register all support components
dbLoadDatabase("../../../dbd/WithAsyn.dbd")
WithAsyn_registerRecordDeviceDriver pdbbase

# Motors substitutions, customize this for your motor
dbLoadTemplate (motor.substitutions.$(ENCODERENABLED).ims)
dbLoadRecords("$(MOTOR)/db/motorUtil.db", "P=IOC:")

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
