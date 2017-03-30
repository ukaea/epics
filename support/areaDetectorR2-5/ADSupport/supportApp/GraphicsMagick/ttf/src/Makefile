TOP = ../../../..
include $(TOP)/configure/CONFIG

LIBRARY = CORE_ttf

USR_CFLAGS_WIN32 += -DWIN32
USR_CFLAGS_WIN32 += -D_VISUALC_

USR_CFLAGS += -DNeedFunctionPrototypes
USR_CFLAGS += -D_TTFLIB_
USR_CFLAGS += -DFT2_BUILD_LIBRARY

TTF = $(TOP)/supportApp/GraphicsMagick/ttf

USR_INCLUDES += -I$(TTF)/inc
SRC = $(TTF)/src

include $(SRC)/autofit/Makefile
include $(SRC)/base/Makefile
include $(SRC)/cache/Makefile
#include $(SRC)/gzip/Makefile
include $(SRC)/lzw/Makefile
include $(SRC)/pcf/Makefile
include $(SRC)/pfr/Makefile
include $(SRC)/psaux/Makefile
include $(SRC)/pshinter/Makefile
include $(SRC)/psnames/Makefile
include $(SRC)/raster/Makefile
include $(SRC)/sfnt/Makefile
include $(SRC)/smooth/Makefile
include $(SRC)/sfnt/Makefile
include $(SRC)/truetype/Makefile
include $(SRC)/type1/Makefile
include $(SRC)/cid/Makefile
include $(SRC)/type42/Makefile
include $(SRC)/winfonts/Makefile
include $(SRC)/bdf/Makefile
include $(SRC)/cff/Makefile

# LIB_RCS = ttf.rc
# D:\downloads\GraphicsMagick\GraphicsMagick-1.3.25\VisualMagick\ttf\ttf.rc

include $(TOP)/configure/RULES
