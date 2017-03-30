TOP = ../../../..
include $(TOP)/configure/CONFIG

LIBRARY = CORE_Magick++

USR_CXXFLAGS_WIN32 += -DWIN32
USR_CXXFLAGS_WIN32 += -D_VISUALC_

USR_CXXFLAGS += -DNeedFunctionPrototypes

USR_INCLUDES += -I$(TOP)/supportApp/GraphicsMagick
USR_INCLUDES += -I$(TOP)/supportApp/GraphicsMagick/lcms/inc

INC += Magick++.h
INC += Magick++/Include.h
INC += Magick++/Image.h
INC += Magick++/Pixels.h
INC += Magick++/STL.h
INC += Magick++/Blob.h
INC += Magick++/Color.h
INC += Magick++/Drawable.h
INC += Magick++/BlobRef.h
INC += Magick++/CoderInfo.h
INC += Magick++/Exception.h
INC += Magick++/Geometry.h
INC += Magick++/ImageRef.h
INC += Magick++/Functions.h
INC += Magick++/Montage.h
INC += Magick++/Options.h
INC += Magick++/Thread.h
INC += Magick++/TypeMetric.h

LIB_SRCS += Blob.cpp
LIB_SRCS += BlobRef.cpp
LIB_SRCS += CoderInfo.cpp
LIB_SRCS += Color.cpp
LIB_SRCS += Drawable.cpp
LIB_SRCS += Exception.cpp
LIB_SRCS += Functions.cpp
LIB_SRCS += Geometry.cpp
LIB_SRCS += Image.cpp
LIB_SRCS += ImageRef.cpp
LIB_SRCS += Montage.cpp
LIB_SRCS += Options.cpp
LIB_SRCS += Pixels.cpp
LIB_SRCS += STL.cpp
LIB_SRCS += Thread.cpp
LIB_SRCS += TypeMetric.cpp

include $(TOP)/configure/RULES
