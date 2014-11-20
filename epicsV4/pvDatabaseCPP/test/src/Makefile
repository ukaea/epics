TOP=..

include $(TOP)/configure/CONFIG

DBD += powerSupplyRegister.dbd

INC += powerSupply.h

LIBRARY_IOC += powerSupply
powerSupply_SRCS += powerSupply.cpp
powerSupply_SRCS += powerSupplyRegister.cpp
powerSupply_LIBS += pvDatabase
powerSupply_LIBS += pvAccess
powerSupply_LIBS += pvData
powerSupply_LIBS += Com
powerSupply_LIBS += $(EPICS_BASE_IOC_LIBS)


PROD_HOST += testPVCopy
testPVCopy_SRCS += powerSupply.cpp
testPVCopy_SRCS += testPVCopy.cpp
testPVCopy_LIBS += pvDatabase pvAccess pvData Com

PROD_HOST += testPVRecord
testPVRecord_SRCS += powerSupply.cpp
testPVRecord_SRCS += testPVRecord.cpp
testPVRecord_LIBS += pvDatabase pvAccess pvData Com

PROD_HOST += testExampleRecord
testExampleRecord_SRCS += powerSupply.cpp
testExampleRecord_SRCS += testExampleRecord.cpp
testExampleRecord_LIBS += pvDatabase pvAccess pvData Com

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

