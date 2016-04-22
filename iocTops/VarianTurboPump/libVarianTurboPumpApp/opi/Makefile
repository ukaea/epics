TOP=../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
VERSION_STRING=-r $(VERSION).$(REVISION)
VERSION_NUM=$(shell sh ../../version.sh $(VERSION_STRING) -n )
VERSION_DOT=$(shell sh ../../version.sh $(VERSION_STRING) -d )

#----------------------------------------
# List of edl files
#OPI_FILES += VarianTurboPump.edl
OPI_FILES += VarianTurboPumpMain.edl
OPI_FILES += VarianTurboPumpComm.edl
OPI_FILES += VarianTurboPumpConfig.edl
OPI_FILES += VarianTurboPumpCounters.edl
OPI_FILES += VarianTurboPumpGauge.edl
OPI_FILES += VarianTurboPumpIO.edl
OPI_FILES += VarianTurboPumpSetPoint.edl
OPI_FILES += VarianTurboPumpVent.edl
OPI_FILES += runVarianTurboPump.example

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

include $(EPICS_LOCAL)/drivers/makefileIncludes/cleanOpiMakefile.include
include $(EPICS_LOCAL)/drivers/makefileIncludes/opiMakefile.include
