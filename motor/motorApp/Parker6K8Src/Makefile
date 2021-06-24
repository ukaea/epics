TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

#=============================
# Build the IOC support library

USR_CXXFLAGS += -Wunused-but-set-variable

LIBRARY_IOC += parker6kSupport

DBD += parker6kSupport.dbd

# Compile and add the code to the support library
parker6kSupport_SRCS += parker6kController.cpp
parker6kSupport_SRCS += parker6kAxis.cpp

parker6kSupport_LIBS += $(EPICS_BASE_IOC_LIBS)

#=============================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

