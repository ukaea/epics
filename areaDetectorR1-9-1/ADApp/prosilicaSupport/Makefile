TOP=../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

# The files in this directory are installed first because they are
# used both by the NDPluginColorConvert and by the Prosilica driver
INC += PvApi.h

# Note, the following files were manually copied from the Prosilica GigESDK/bin-pc directory
# to this directory after building
ifeq (win32-x86, $(findstring win32-x86, $(T_A)))
LIB_INSTALLS_WIN32    += ../os/win32-x86/PvAPI.lib
BIN_INSTALLS_WIN32    += ../os/win32-x86/PvAPI.dll

else ifeq (windows-x64, $(findstring windows-x64, $(T_A)))
LIB_INSTALLS_WIN32    += ../os/windows-x64/PvAPI.lib
BIN_INSTALLS_WIN32    += ../os/windows-x64/PvAPI.dll

else ifeq (cygwin-x86, $(findstring cygwin-x86, $(T_A)))
LIB_INSTALLS_cygwin32    += ../os/win32-x86/PvAPI.lib
BIN_INSTALLS_cygwin32    += ../os/win32-x86/PvAPI.dll

else ifeq (linux-x86_64, $(findstring linux-x86_64, $(T_A)))
LIB_INSTALLS_Linux += ../os/linux-x86_64/libPvAPI.a

else ifeq (linux-x86, $(findstring linux-x86, $(T_A)))
LIB_INSTALLS_Linux += ../os/linux-x86/libPvAPI.a

else ifeq (darwin-x86, $(findstring darwin-x86, $(T_A)))
LIB_INSTALLS_Darwin += ../os/darwin-x86/libPvAPI.a
endif

#=============================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

