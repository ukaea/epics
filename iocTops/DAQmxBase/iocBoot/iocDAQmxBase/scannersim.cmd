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
#only read first 1200 scans
dbLoadRecords("db/DAQmxBaseScan.vdb","DAQMX=test,NELM=1200")

DAQmxBaseConfig("scan1", "Dev1/ao0", 0, "AO","N=1000 F=10000 ")
DAQmxBaseConfig("scan1", "Dev1/ao1", 1, "AO","")
asynSetTraceMask("scan1", 0,0x11)

#we are scanning a full 2000 scans!!
DAQmxBaseConfig("myport1","Dev1/ai0", 0 ,"AI","TerminalNRSE N=2000 F=10000")
DAQmxBaseConfig("myport1","Dev1/ai1", 1 ,"AI","TerminalNRSE")
DAQmxPortOptions("myport1",0,"S=1 s=0 T=3.0 p=0")
asynSetTraceMask("myport1" 0,0x11)

iocInit()


#generate a scanner output: (well sortoff)

DAQmxGen("scan1",0,"clear Pulse A=0 B=5")
DAQmxGen("scan1",0,"Pulse A=250 B=2.5")
DAQmxGen("scan1",0,"Pulse A=750 B=2.7")

DAQmxGen("scan1",1,"clear buldge A=250 B=45 C=2.1")
DAQmxGen("scan1",1,"buldge A=750 B=60 C=1.6")


#asynSetTraceMask("myport1", 0,0x01)
