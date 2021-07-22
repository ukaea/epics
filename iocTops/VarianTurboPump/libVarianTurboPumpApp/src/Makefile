TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================
VERSION_STRING=-r $(VERSION).$(REVISION)
VERSION_NUM=$(shell sh ../../version.sh $(VERSION_STRING) -n )
VERSION_DOT=$(shell sh ../../version.sh $(VERSION_STRING) -d )

#=============================
SHRLIB_LDFLAGS += -Wl,-soname=lib$(LIBRARY_DEFAULT).so.$(VERSION)

# Build devVarianTurboPump as a library for an IOC:
LIBRARY_DEFAULT = VarianTurboPump
SHRLIB_VERSION = $(VERSION_DOT)


# Driver library Source files
#VarianTurboPump_SRCS += devVarianTurboPump.c


# Driver library Header files
# These will be installed into $(TOP)/include
# NOTE: Only do this if the header file is required when compiling this library 
#       into another application.  This is usually not needed
#INC += VarianTurboPump.h
 


# VarianTurboPump.dbd will be created and installed
DBD += VarianTurboSupport.dbd

# VarianTurboPump.dbd will be made up from these files:
#VarianTurboPump_DBD += asyn.dbd



# Add all the support libraries needed by this Driver
VarianTurboPump_LIBS += asyn
VarianTurboPump_LIBS += stream
VarianTurboPump_LIBS += calc

# Finally link to the EPICS Base libraries
VarianTurboPump_LIBS += $(EPICS_BASE_IOC_LIBS)


#NOTES:
#      1)It is not possible to build sncExample both as a component of VarianTurboPump
#        and standalone. You must choose only one.
#      2)To build sncExample SNCSEQ must be defined in <top>/configure/RELEASE

#The following builds sncExample as a component of VarianTurboPump
#VarianTurboPump_SRCS += sncExample.stt
#VarianTurboPump_LIBS += seq pv


#The following builds sncExample as a standalone application
#PROD_HOST += sncExample
#sncExample_SNCFLAGS += +m
#sncExample_SRCS += sncExample.stt
#sncExample_LIBS += seq pv
#sncExample_LIBS += $(EPICS_BASE_HOST_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE


# include $(EPICS_LOCAL)/drivers/makefileIncludes/dbdMakefile.include
# include $(EPICS_LOCAL)/drivers/makefileIncludes/libMakefile.include
# include $(EPICS_LOCAL)/drivers/makefileIncludes/includeMakefile.include
