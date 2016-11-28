TOP=..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

# This directory contains files to support GraphicsMagick so users don't need to install it
 
INC_WIN32 += Magick++.h
INC_WIN32 += $(foreach dir, Magick++ magick wand, \
     $(subst ../,,$(wildcard ../$(dir)/*.h)) )
INC_cygwin32 += Magick++.h
INC_cygwin32 += $(foreach dir, Magick++ magick wand, \
     $(subst ../,,$(wildcard ../$(dir)/*.h)) )

ifeq (win32-x86, $(findstring win32-x86, $(T_A)))
LIB_INSTALLS_WIN32    += ../os/WIN32/CORE_RL_Magick++_.lib
LIB_INSTALLS_WIN32    += ../os/WIN32/CORE_RL_bzlib_.lib
LIB_INSTALLS_WIN32    += ../os/WIN32/CORE_RL_coders_.lib
LIB_INSTALLS_WIN32    += ../os/WIN32/CORE_RL_filters_.lib
LIB_INSTALLS_WIN32    += ../os/WIN32/CORE_RL_jbig_.lib
LIB_INSTALLS_WIN32    += ../os/WIN32/CORE_RL_jp2_.lib
LIB_INSTALLS_WIN32    += ../os/WIN32/CORE_RL_jpeg_.lib
LIB_INSTALLS_WIN32    += ../os/WIN32/CORE_RL_lcms_.lib
LIB_INSTALLS_WIN32    += ../os/WIN32/CORE_RL_libxml_.lib
LIB_INSTALLS_WIN32    += ../os/WIN32/CORE_RL_magick_.lib
LIB_INSTALLS_WIN32    += ../os/WIN32/CORE_RL_png_.lib
LIB_INSTALLS_WIN32    += ../os/WIN32/CORE_RL_tiff_.lib
LIB_INSTALLS_WIN32    += ../os/WIN32/CORE_RL_ttf_.lib
LIB_INSTALLS_WIN32    += ../os/WIN32/CORE_RL_wand_.lib
LIB_INSTALLS_WIN32    += ../os/WIN32/CORE_RL_wmf_.lib
LIB_INSTALLS_WIN32    += ../os/WIN32/CORE_RL_xlib_.lib
LIB_INSTALLS_WIN32    += ../os/WIN32/CORE_RL_zlib_.lib

else ifeq (windows-x64, $(findstring windows-x64, $(T_A)))
LIB_INSTALLS_WIN32    += ../os/WIN32/x64/CORE_RL_Magick++_.lib
LIB_INSTALLS_WIN32    += ../os/WIN32/x64/CORE_RL_bzlib_.lib
LIB_INSTALLS_WIN32    += ../os/WIN32/x64/CORE_RL_coders_.lib
LIB_INSTALLS_WIN32    += ../os/WIN32/x64/CORE_RL_filters_.lib
LIB_INSTALLS_WIN32    += ../os/WIN32/x64/CORE_RL_jbig_.lib
LIB_INSTALLS_WIN32    += ../os/WIN32/x64/CORE_RL_jp2_.lib
LIB_INSTALLS_WIN32    += ../os/WIN32/x64/CORE_RL_jpeg_.lib
LIB_INSTALLS_WIN32    += ../os/WIN32/x64/CORE_RL_lcms_.lib
LIB_INSTALLS_WIN32    += ../os/WIN32/x64/CORE_RL_libxml_.lib
LIB_INSTALLS_WIN32    += ../os/WIN32/x64/CORE_RL_magick_.lib
LIB_INSTALLS_WIN32    += ../os/WIN32/x64/CORE_RL_png_.lib
LIB_INSTALLS_WIN32    += ../os/WIN32/x64/CORE_RL_tiff_.lib
LIB_INSTALLS_WIN32    += ../os/WIN32/x64/CORE_RL_ttf_.lib
LIB_INSTALLS_WIN32    += ../os/WIN32/x64/CORE_RL_wand_.lib
LIB_INSTALLS_WIN32    += ../os/WIN32/x64/CORE_RL_wmf_.lib
LIB_INSTALLS_WIN32    += ../os/WIN32/x64/CORE_RL_xlib_.lib
LIB_INSTALLS_WIN32    += ../os/WIN32/x64/CORE_RL_zlib_.lib

else ifeq (cygwin-x86, $(findstring cygwin-x86, $(T_A)))
MAGICK_LIBS  = $(addprefix ../os/cygwin32/, libGraphicsMagick++.lib libGraphicsMagickWand.lib libGraphicsMagick.lib)
MAGICK_LIBS += $(addprefix ../os/cygwin32/, liblcms.lib libpng14.lib libbz2.lib libfreetype.lib)
MAGICK_LIBS += $(addprefix ../os/cygwin32/, libjbig.lib libxml2.dll.lib)
LIB_INSTALLS_cygwin32 += $(MAGICK_LIBS)
BIN_INSTALLS_cygwin32 += ../os/cygwin32/cygxml2-2.dll

endif


#=============================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

