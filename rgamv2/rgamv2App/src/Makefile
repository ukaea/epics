TOP=../..

include $(TOP)/configure/CONFIG
# The following gets rid of the -fno-implicit-templates flag on vxWorks, 
# so we get automatic template instantiation.
# This is what we want for the source files that define template functions.
ifeq (vxWorks,$(findstring vxWorks, $(T_A)))
CODE_CXXFLAGS=
endif

USR_CXXFLAGS_DEFAULT += -fno-operator-names
#USR_CXXFLAGS_DEFAULT += -pedantic
#USR_CXXFLAGS_DEFAULT += -I /dls_sw/targetOS/vxWorks/Tornado-2.2_RHEL5/target/h
#USR_CXXFLAGS_DEFAULT += /dls_sw/targetOS/vxWorks/Tornado-2.2_RHEL5/host/src/gnu.ppc/gcc/config/
#USR_CXXFLAGS_DEFAULT += -save-temps
#USR_CXXFLAGS_DEFAULT += -fimplicit-templates

# -------------------------------
# Build an Diamond Support Module
# -------------------------------
LIBRARY_IOC_Linux += rgamv2

# xxxRecord.dbd will be installed into <top>/dbd
DBD += rgamv2Include.dbd
DBD += rgaInclude.dbd

# The following are compiled and added to the support library
rgamv2_SRCS_Linux   +=rgamv2.cpp

# We need to link against the EPICS Base libraries
rgamv2_LIBS += $(EPICS_BASE_IOC_LIBS)

include $(TOP)/configure/RULES
