TOP=../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
VERSION_STRING=-r $(VERSION).$(REVISION)
VERSION_NUM=$(shell sh ../../version.sh $(VERSION_STRING) -n )
VERSION_DOT=$(shell sh ../../version.sh $(VERSION_STRING) -d )

#----------------------------------------------------
# List of Library Templates
TEMPLATES += Varian_TV301.proto
TEMPLATES += Varian_TV550.proto
TEMPLATES += Varian_TV551.proto
TEMPLATES += Varian_TPS.proto
TEMPLATES += Agilent_TVRP.proto
TEMPLATES += Agilent_TT304.proto
TEMPLATES += Varian_TV81.proto

TEMPLATES += Varian-Agilent_Turbopump.template
TEMPLATES += Varian-Agilent_ErrorA.template
TEMPLATES += Varian-Agilent_ErrorB.template


include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

#include $(EPICS_LOCAL)/drivers/makefileIncludes/templateMakefile.include

vpath %.protocol ..

%.proto: %.protocol Varian-Agilent_Common.protocol
	cat ../Varian-Agilent_Common.protocol $< > $@
