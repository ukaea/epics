TOP=../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
VERSION_STRING=-r $(VERSION).$(REVISION)
VERSION_NUM=$(shell sh ../../version.sh $(VERSION_STRING) -n )
VERSION_DOT=$(shell sh ../../version.sh $(VERSION_STRING) -d )

#----------------------------------------------------
# List of Library Templates

#General Turbopump Solution
#TEMPLATES += Varian_TV301.proto
#TEMPLATES += Varian_TV550.proto
#TEMPLATES += Varian_TV551.proto
#TEMPLATES += Varian_TPS.proto
#TEMPLATES += Agilent_TT304.proto
#TEMPLATES += Varian_TV81.proto
#TEMPLATES += Agilent_TT2300.proto
#TEMPLATES += Varian-Agilent_Turbopump.template
#TEMPLATES += Varian-Agilent_ErrorA.template
#TEMPLATES += Varian-Agilent_ErrorB.template

#Specific Turbopump Solution
TEMPLATES += Agilent_TT2300.proto
TEMPLATES += CCFE_VTP.template
TEMPLATES += Varian-Agilent_ErrorA.template

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

#include $(EPICS_LOCAL)/drivers/makefileIncludes/templateMakefile.include

vpath %.protocol ..

#General Turbopump Solution
#%.proto: %.protocol Varian-Agilent_Common.protocol
#	cat ../Varian-Agilent_Common.protocol $< > $@
	
#Specific Turbopump Solution
%.proto: %.protocol CCFE_Varian-Agilent_Common.protocol
	cat ../CCFE_Varian-Agilent_Common.protocol $< > $@
