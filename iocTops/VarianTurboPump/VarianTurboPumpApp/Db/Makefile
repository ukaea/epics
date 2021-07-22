TOP=../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE 

#----------------------------------------------------
#  Optimization of db files using dbst (DEFAULT: NO)
#DB_OPT = YES

#----------------------------------------------------
# Create and install (or just install) into <top>/db
# databases, templates, substitutions like this

#General Turbopump Solution
#DB += VarianTurboPump.db
#DB_INSTALLS += Agilent_TT304.proto
#DB_INSTALLS += Agilent_TVRP.proto
#DB_INSTALLS += Varian_TPS.proto
#DB_INSTALLS += Varian_TV551.proto
#DB_INSTALLS += Varian_TV550.proto
#DB_INSTALLS += Varian_TV301.proto
#DB_INSTALLS += Varian_TV81.proto
#DB_INSTALLS += Agilent_TT2300.proto

#Specific Turbopump Solution
DB += VarianTurboPump.db
DB_INSTALLS += Agilent_TT2300.proto
DB += CCFE_VTP.acf


#----------------------------------------------------
# If <anyname>.db template is not named <anyname>*.template add
# <anyname>_template = <templatename>

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

vpath %.proto $(TOP)/templates

