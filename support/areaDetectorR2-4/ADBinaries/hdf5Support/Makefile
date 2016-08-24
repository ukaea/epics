TOP=..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

# This directory contains files to support the HDF5 library version 1.8.7
# so users don't need to install it.
# Read the README file in this directory to see where these files came from.

# Note, the HFpubconf.h file is OS-specific supplied by HDF5 Group (Windows)

# OS-specific files in os/ARCH
INC_WIN32 += H5ACpublic.h
INC_WIN32 += H5api_adpt.h
INC_WIN32 += H5Apublic.h
INC_WIN32 += H5Cpublic.h
INC_WIN32 += H5Dpublic.h
INC_WIN32 += H5Epubgen.h
INC_WIN32 += H5Epublic.h
INC_WIN32 += H5FDcore.h
INC_WIN32 += H5FDdirect.h
INC_WIN32 += H5FDfamily.h
INC_WIN32 += H5FDlog.h
INC_WIN32 += H5FDmpi.h
INC_WIN32 += H5FDmpio.h
INC_WIN32 += H5FDmulti.h
INC_WIN32 += H5FDpublic.h
INC_WIN32 += H5FDsec2.h
INC_WIN32 += H5FDstdio.h
INC_WIN32 += H5FDwindows.h
INC_WIN32 += H5Fpublic.h
INC_WIN32 += H5Gpublic.h
INC_WIN32 += H5Ipublic.h
INC_WIN32 += H5Lpublic.h
INC_WIN32 += H5MMpublic.h
INC_WIN32 += H5Opublic.h
INC_WIN32 += H5overflow.h
INC_WIN32 += H5PLextern.h
INC_WIN32 += H5PLpublic.h
INC_WIN32 += H5Ppublic.h
INC_WIN32 += H5public.h
INC_WIN32 += H5Rpublic.h
INC_WIN32 += H5Spublic.h
INC_WIN32 += H5Tpublic.h
INC_WIN32 += H5version.h
INC_WIN32 += H5Zpublic.h
INC_WIN32 += hdf5.h
INC_WIN32 += H5pubconf.h 
INC_WIN32 += H5pubconf_32_shared.h H5pubconf_32_static.h 
INC_WIN32 += H5pubconf_64_shared.h H5pubconf_64_static.h

ifeq (win32-x86, $(findstring win32-x86, $(T_A)))
    HDF5_LIBS = $(addprefix ../os/win32-x86-shared/, hdf5.lib szip.lib zlib.lib)
    HDF5_BINS = $(addprefix ../os/win32-x86-shared/, hdf5.dll szip.dll zlib.dll)
    BIN_INSTALLS_WIN32   += $(HDF5_BINS)
    HDF5_LIBS += $(addprefix ../os/win32-x86-static/, libhdf5.lib libszip.lib libzlib.lib)
    LIB_INSTALLS_WIN32   += $(HDF5_LIBS)
else ifeq (windows-x64, $(findstring windows-x64, $(T_A)))
    HDF5_LIBS = $(addprefix ../os/windows-x64-shared/, hdf5.lib szip.lib zlib.lib)
    HDF5_BINS = $(addprefix ../os/windows-x64-shared/, hdf5.dll szip.dll zlib.dll)
    BIN_INSTALLS_WIN32   += $(HDF5_BINS)
    HDF5_LIBS += $(addprefix ../os/windows-x64-static/, libhdf5.lib libszip.lib libzlib.lib)
    LIB_INSTALLS_WIN32   += $(HDF5_LIBS)
endif

#=============================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

