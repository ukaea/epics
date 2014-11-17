# Makefile for the EPICS V4 pvCommon module

TOP = .
include $(TOP)/configure/CONFIG

DIRS := configure

DIRS += boostApp
boostApp_DEPEND_DIRS = configure

DIRS += mbSrc
mbSrc_DEPEND_DIRS = boostApp

DIRS += testApp
testApp_DEPEND_DIRS = mbSrc

include $(TOP)/configure/RULES_TOP
