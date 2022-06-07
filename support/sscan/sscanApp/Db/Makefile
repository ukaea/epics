# Makefile.Host
TOP = ../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

#----------------------------------------------------
#  Optimization of db files using dbst (DEFAULT: NO)
#DB_OPT = YES

#----------------------------------------------------
# Create and install (or just install) dbd files
# <name>.dbd will be created from <name>Include.dbd
#DBD += example.dbd

#----------------------------------------------------
# Create and install (or just install)
# databases, templates, substitutions like this
#DB += example.db

DB += alignParms.db
DB += saveData.db
DB += scanAux.db
DB += scan.db
DB += scanParms2D.db
DB += scanParms2Pos.db
DB += scanParms.db
DB += scanParmsRemote.db
DB += scanProgress.db
DB += standardScans.db


REQ += alignParms_settings.req
REQ += saveData_settings.req
REQ += scanAux_settings.req
REQ += scanParms2D_settings.req
REQ += scanParmsRemote_settings.req
REQ += scanParms_settings.req
REQ += scanProgress_settings.req
REQ += scan_settings.req
REQ += sscanRecord_settings.req
REQ += standardScans_settings.req


#----------------------------------------------------
# Declare template files which do not show up in DB
#USES_TEMPLATE += dbExample2.template

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE
