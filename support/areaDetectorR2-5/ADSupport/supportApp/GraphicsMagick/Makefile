TOP = ../..
include $(TOP)/configure/CONFIG

#---------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=======================================

ifeq ($(WITH_GRAPHICSMAGICK),YES)
  ifeq ($(GRAPHICSMAGICK_EXTERNAL),NO)

    DIRS += lcms
    DIRS += bzlib
    DIRS += ttf
    DIRS += Magick
    DIRS += Magick++

  endif # ($(GRAPHICSMAGICK_EXTERNAL),NO)
endif # ($(WITH_GRAPHICSMAGICK),YES)

include $(TOP)/configure/RULES_DIRS

#----------------------------------------
#  ADD RULES AFTER THIS LINE
