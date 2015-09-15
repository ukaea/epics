#####################################################################################################
#																									#
#	Module:																							#
#		camonitor.py																				#
#																									#
#	Description:																					#
#		Uses pyepics camonitor to record the value of each of the PVs as they change.				#
#		Pyepics is used for the convenience of cross-platform scripting.							#
#																									#
#	Author:  Peter Heesterman (Tessella plc). Date: 03 Sep 2015.									#
#	Written for CCFE (Culham Centre for Fusion Energy).												#
#																									#
#	LeyboldTurbo is distributed subject to a Software License Agreement								#
#	found in file LICENSE that is included with this distribution.									#
#																									#
#####################################################################################################

import epics
import os
import sys
import datetime
import time

# pyepics camonitor doesn't output the initial value of the PV when it starts up.
# It only reports subsequent changes.
# I want to know the initial value.
# It's not a good solution but I'm using pvget to read and print that.
def caGetAndMonitor(PVName):
	PV = epics.PV(PVName)
	print(PVName, datetime.datetime.fromtimestamp(PV.timestamp).strftime('%Y-%m-%d %H:%M:%S'), PV.value)
	epics.camonitor(PVName)

NumPumps='1'
if len(sys.argv) > 1:
	NumPumps=sys.argv[1]
	

ArchivePath = 'camonitor.app.log'
if len(sys.argv) > 2:
	ArchivePath=sys.argv[2]
	

sys.stdout = open(ArchivePath, 'w')

PVNames = ["Running", \
			"Reset", \
			"Fault", \
			"FaultStr", \
			"FirmwareVersion", \
			"WarningTemperature", \
			"WarningTemperatureStr", \
			"WarningHighLoad",
			"WarningHighLoadStr", \
			"WarningPurge", \
			"WarningPurgeStr", \
			"StatorFrequency", \
			"ConverterTemperature", \
			"MotorCurrent", \
			"PumpTemperature", \
			"CircuitVoltage"]

ChannelDefaultRoot = os.getenv('ASYNPORT', 'LEYBOLDTURBO')
for Pump in range(1, int(NumPumps)+1):
	ChannelRoot = os.getenv('ASYNPORT'+str(Pump), ChannelDefaultRoot+':'+str(Pump))
	for index, PVName in enumerate(PVNames):
		caGetAndMonitor(ChannelRoot + ":" + PVName)

Channel1Root = os.getenv('ASYNPORT1', ChannelDefaultRoot+':1')
chid = epics.ca.create_channel(Channel1Root+':Running')
while (epics.ca.isConnected(chid)):
	time.sleep(1)

# https://github.com/pyepics/pyepics/issues/34 
epics.ca.finalize_libca()
