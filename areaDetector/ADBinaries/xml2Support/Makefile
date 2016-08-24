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
INC_WIN32 += libxml/DOCBparser.h
INC_WIN32 += libxml/globals.h 
INC_WIN32 += libxml/tree.h
INC_WIN32 += libxml/xmlregexp.h
INC_WIN32 += libxml/HTMLparser.h
INC_WIN32 += libxml/HTMLtree.h
INC_WIN32 += libxml/SAX.h
INC_WIN32 += libxml/SAX2.h
INC_WIN32 += libxml/c14n.h
INC_WIN32 += libxml/catalog.h
INC_WIN32 += libxml/chvalid.h
INC_WIN32 += libxml/debugXML.h
INC_WIN32 += libxml/dict.h
INC_WIN32 += libxml/encoding.h
INC_WIN32 += libxml/entities.h
INC_WIN32 += libxml/hash.h
INC_WIN32 += libxml/list.h
INC_WIN32 += libxml/nanoftp.h
INC_WIN32 += libxml/nanohttp.h
INC_WIN32 += libxml/parser.h
INC_WIN32 += libxml/parserInternals.h
INC_WIN32 += libxml/pattern.h
INC_WIN32 += libxml/relaxng.h
INC_WIN32 += libxml/schemasInternals.h
INC_WIN32 += libxml/schematron.h
INC_WIN32 += libxml/threads.h
INC_WIN32 += libxml/uri.h
INC_WIN32 += libxml/valid.h
INC_WIN32 += libxml/xinclude.h
INC_WIN32 += libxml/xlink.h
INC_WIN32 += libxml/xmlIO.h
INC_WIN32 += libxml/xmlautomata.h
INC_WIN32 += libxml/xmlerror.h
INC_WIN32 += libxml/xmlexports.h
INC_WIN32 += libxml/xmlmemory.h
INC_WIN32 += libxml/xmlreader.h
INC_WIN32 += libxml/xmlsave.h
INC_WIN32 += libxml/xmlschemas.h
INC_WIN32 += libxml/xmlschemastypes.h
INC_WIN32 += libxml/xpointer.h
INC_WIN32 += libxml/xmlstring.h
INC_WIN32 += libxml/xmlunicode.h
INC_WIN32 += libxml/xmlversion.h
INC_WIN32 += libxml/xmlwin32version.h
INC_WIN32 += libxml/xmlwriter.h
INC_WIN32 += libxml/xpath.h
INC_WIN32 += libxml/xpathInternals.h
INC_WIN32 += libxml/xmlmodule.h


ifeq (win32-x86, $(findstring win32-x86, $(T_A)))
  ifeq ($(STATIC_BUILD), NO)
    XML2_LIBS = $(addprefix ../os/win32-x86-shared/, libxml2.lib)
    XML2_BINS = $(addprefix ../os/win32-x86-shared/, libxml2.dll)
    LIB_INSTALLS_WIN32   += $(XML2_LIBS)
    BIN_INSTALLS_WIN32   += $(XML2_BINS)
  else
    XML2_LIBS = $(addprefix ../os/win32-x86-static/, libxml2.lib)
    LIB_INSTALLS_WIN32   += $(XML2_LIBS)
  endif

else ifeq (windows-x64, $(findstring windows-x64, $(T_A)))
  ifeq ($(STATIC_BUILD), NO)
    XML2_LIBS = $(addprefix ../os/windows-x64-shared/, libxml2.lib)
    XML2_BINS = $(addprefix ../os/windows-x64-shared/, libxml2.dll)
    LIB_INSTALLS_WIN32   += $(XML2_LIBS)
    BIN_INSTALLS_WIN32   += $(XML2_BINS)
  else
    XML2_LIBS = $(addprefix ../os/windows-x64-static/, libxml2.lib)
    LIB_INSTALLS_WIN32   += $(XML2_LIBS)
  endif
endif

#=============================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

