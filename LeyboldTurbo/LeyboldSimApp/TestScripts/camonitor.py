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
	return;

FirstPump='1'
if len(sys.argv) > 1:
	FirstPump=sys.argv[1]
	
LastPump=FirstPump
if len(sys.argv) > 2:
	LastPump=sys.argv[2]

sys.stdout = open('camonitor.sim.log', 'w')

os.environ["EPICS_CA_SERVER_PORT"]="5072"
os.environ["EPICS_CA_AUTO_ADDR_LIST"]="NO"
os.environ["EPICS_CA_ADDR_LIST"]="localhost"

PVNames = ["Running", \
			"Fault", \
			"FirmwareVersion", \
			"WarningTemperature", \
			"WarningHighLoad",
			"WarningPurge", \
			"StatorFrequency", \
			"ConverterTemperature", \
			"MotorCurrent", \
			"PumpTemperature", \
			"CircuitVoltage"]

			
ChannelDefaultRoot = os.getenv('ASYNSIMPORT', 'LEYBOLDTURBOSIM')
for Pump in range(int(FirstPump), int(LastPump)+1):
	for index, PVName in enumerate(PVNames):
		ChannelRoot = os.getenv('ASYNSIMPORT'+str(Pump), ChannelDefaultRoot+':'+str(Pump))
		caGetAndMonitor(ChannelRoot + ":" + PVName)

Channel1Root = os.getenv('ASYNSIMPORT'+str(FirstPump), ChannelDefaultRoot+':'+str(FirstPump))
chid = epics.ca.create_channel(Channel1Root + ":Running")
while (epics.ca.isConnected(chid)):
	time.sleep(1)

del os.environ["EPICS_CA_SERVER_PORT"]
del os.environ["EPICS_CA_AUTO_ADDR_LIST"]
del os.environ["EPICS_CA_ADDR_LIST"]
