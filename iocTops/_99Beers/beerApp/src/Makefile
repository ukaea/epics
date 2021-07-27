TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS BELOW HERE

# Reference calcSupport.dbd into <top>/dbd

DBD += calcSupport.dbd

# Build an IOC support library
LIBRARY_IOC += beerSupport

# Compile and add the code to the support library

# Link locally-provided code into the support library,
# rather than directly into the IOC application.
# This is required for Windows DLL builds.
beerSupport_SRCS += beerHello.c

beerSupport_LIBS += $(EPICS_BASE_IOC_LIBS)
beerSupport_LIBS += calc

# Build the IOC application
PROD_IOC = beer

# beer.dbd will be created and installed
DBD += beer.dbd

# beer.dbd will include these files:
beer_DBD += base.dbd

# beer_registerRecordDeviceDriver.cpp derives from beer.dbd
beer_SRCS += beer_registerRecordDeviceDriver.cpp

# Build the main IOC entry point where needed
beer_SRCS_DEFAULT += beerMain.cpp
beer_SRCS_vxWorks += -nil-

# Link in the code from our support library
beer_LIBS += beerSupport

# Finally link IOC to the EPICS Base libraries
beer_LIBS += $(EPICS_BASE_IOC_LIBS)

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD EXTRA GNUMAKE RULES BELOW HERE

