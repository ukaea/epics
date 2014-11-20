TOP=..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

DBD += exampleDatabase.dbd

INC += exampleDatabase.h
INC += exampleMonitorPlugin.h

LIBRARY += exampleDatabase
exampleDatabase_SRCS += exampleDatabase.cpp
exampleDatabase_SRCS += exampleMonitorPlugin.cpp
exampleDatabase_SRCS += exampleDatabaseRegister.cpp
exampleDatabase_SRCS += exampleMonitorPluginRegister.cpp
exampleDatabase_LIBS += powerSupply
exampleDatabase_LIBS += pvDatabase
exampleDatabase_LIBS += pvAccess
exampleDatabase_LIBS += pvData
exampleDatabase_LIBS += Com
exampleDatabase_LIBS += $(EPICS_BASE_IOC_LIBS)

PROD_HOST += exampleDatabaseMain
exampleDatabaseMain_SRCS += exampleDatabaseMain.cpp
exampleDatabaseMain_LIBS += exampleDatabase
exampleDatabaseMain_LIBS += powerSupply
exampleDatabaseMain_LIBS += pvDatabase
exampleDatabaseMain_LIBS += pvAccess
exampleDatabaseMain_LIBS += pvData
exampleDatabaseMain_LIBS += Com

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

