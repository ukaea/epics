#!../../bin/linux-x86/DAQmxBase

###!$(INSTALL)/bin/$(ARCH)/DAQmxBase

## You may have to change DAQmxBase to something else
## everywhere it appears in this file

#cd "$(INSTALL)"
cd ../../

# Load binaries on architectures that need to do so.
# VXWORKS_ONLY, LINUX_ONLY and RTEMS_ONLY are macros that resolve
# to a comment symbol on architectures that are not the current
# build architecture, so they can be used liberally to do architecture
# specific things. Alternatively, you can include an architecture
# specific file.
#$(VXWORKS_ONLY)ld < bin/$(ARCH)/DAQmxBase.munch
ld < bin/linux-x86/DAQmxBase.munch

## Register all support components
dbLoadDatabase("dbd/DAQmxBase.dbd")
DAQmxBase_registerRecordDeviceDriver(pdbbase)

## Load record instances
dbLoadRecords("db/DAQmxBaseDigIn.vdb","DAQMX=test")

DAQmxBaseConfig("myport1", "Dev1/port0", 0, "BI","OneShot N=1 F=0")
asynSetTraceMask("myport1", 0,0x11)


iocInit()

#asynSetTraceMask("myport1", 0,0x01)
