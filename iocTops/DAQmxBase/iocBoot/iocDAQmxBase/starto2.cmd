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
dbLoadRecords("db/DAQmxBaseO.vdb","DAQMX=test,NELM=1000")

DAQmxBaseConfig("myport1", "Dev1/ao0", 0, "AO","N=1000 F=10000 ")
DAQmxBaseConfig("myport1", "Dev1/ao1", 1, "AO","")
asynSetTraceMask("myport1", 0,0x11)

iocInit()

#examples:
#DAQmxGen("myport1",0,"clear block A=3 B=1 C=3.5 D=-0.5")
#DAQmxGen("myport1",0,"clear Random A=2 B=-2 C=1000")
#DAQmxGen("myport1",0,"clear Triangle A=4 B=1 C=0.1 D=0  E=12 F=0")
#DAQmxGen("myport1",0,"clear sin A=0.1 B=3 C=0 D=0")
#DAQmxGen("myport1",0,"clear smoothrandom A=3 B=-3 C=0.14")
#DAQmxGen("myport1",0,"clear buldge A=50 B=60 C=5")


#asynSetTraceMask("myport1", 0,0x01)
