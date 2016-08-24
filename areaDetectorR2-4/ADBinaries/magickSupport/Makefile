TOP=..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

# This directory contains files to support GraphicsMagick so users don't need to install it
 
INC_WIN32 += Magick++.h
INC_WIN32 += $(foreach dir, Magick++ magick, $(subst ../,,$(wildcard ../$(dir)/*.h)) )
INC_cygwin32 += Magick++.h
INC_cygwin32 += $(foreach dir, Magick++ magick wand, \
     $(subst ../,,$(wildcard ../$(dir)/*.h)) )

INC_WIN32 += Magick++/Include.h
INC_WIN32 += Magick++/Image.h
INC_WIN32 += Magick++/Blob.h
INC_WIN32 += Magick++/Color.h
INC_WIN32 += Magick++/Drawable.h
INC_WIN32 += Magick++/Geometry.h
INC_WIN32 += Magick++/Exception.h
INC_WIN32 += Magick++/TypeMetric.h
INC_WIN32 += Magick++/Pixels.h
INC_WIN32 += Magick++/STL.h
INC_WIN32 += Magick++/CoderInfo.h
INC_WIN32 += Magick++/Montage.h

INC_WIN32 += magick/magick_config.h
INC_WIN32 += magick/api.h
INC_WIN32 += magick/common.h
INC_WIN32 += magick/magick_types.h
INC_WIN32 += magick/analyze.h
INC_WIN32 += magick/image.h
INC_WIN32 += magick/forward.h
INC_WIN32 += Magick/colorspace.h
INC_WIN32 += magick/error.h
INC_WIN32 += magick/log.h
INC_WIN32 += magick/timer.h
INC_WIN32 += magick/attribute.h
INC_WIN32 += magick/average.h
INC_WIN32 += magick/blob.h
INC_WIN32 += magick/cdl.h
INC_WIN32 += magick/channel.h
INC_WIN32 += magick/color.h
INC_WIN32 += magick/color_lookup.h
INC_WIN32 += magick/colormap.h
INC_WIN32 += magick/command.h
INC_WIN32 += magick/compare.h
INC_WIN32 += magick/composite.h
INC_WIN32 += magick/compress.h
INC_WIN32 += magick/confirm_access.h
INC_WIN32 += magick/constitute.h
INC_WIN32 += magick/decorate.h
INC_WIN32 += magick/delegate.h
INC_WIN32 += magick/deprecate.h
INC_WIN32 += magick/describe.h
INC_WIN32 += magick/draw.h
INC_WIN32 += magick/render.h
INC_WIN32 += magick/type.h
INC_WIN32 += magick/effect.h
INC_WIN32 += magick/enhance.h
INC_WIN32 += magick/fx.h
INC_WIN32 += magick/gem.h
INC_WIN32 += magick/random.h
INC_WIN32 += magick/gradient.h
INC_WIN32 += magick/hclut.h
INC_WIN32 += magick/list.h
INC_WIN32 += magick/magic.h
INC_WIN32 += magick/magick.h
INC_WIN32 += magick/memory.h
INC_WIN32 += magick/module.h
INC_WIN32 += magick/monitor.h
INC_WIN32 += magick/montage.h
INC_WIN32 += magick/operator.h
INC_WIN32 += magick/paint.h
INC_WIN32 += magick/pixel_cache.h
INC_WIN32 += magick/pixel_iterator.h
INC_WIN32 += magick/plasma.h
INC_WIN32 += magick/profile.h
INC_WIN32 += magick/quantize.h
INC_WIN32 += magick/registry.h
INC_WIN32 += magick/resize.h
INC_WIN32 += magick/resource.h
INC_WIN32 += magick/shear.h
INC_WIN32 += magick/signature.h
INC_WIN32 += magick/statistics.h
INC_WIN32 += magick/texture.h
INC_WIN32 += magick/transform.h
INC_WIN32 += magick/utility.h
INC_WIN32 += magick/version.h

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

