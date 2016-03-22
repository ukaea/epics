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
#	VQM_ITMS is distributed subject to a Software License Agreement								#
#	found in file LICENSE that is included with this distribution.									#
#																									#
#####################################################################################################


import epics
import os
import sys
import datetime
import time

# camonitor calls sys.stdout.write() by default.
# Redirecting this causes it not to put any end-of-line charachters on it.
# Also, I would prefer to flush the file each time so that the information is up-to-date.
class cFile:
	def __init__ (self, fileName, attributes='w'):
		self.f = open(fileName, attributes)
	def __enter__ (self):
		return self.f
	def __exit__ (self, exc_type, exc_value, traceback):
		self.f.close()
	def writeln(self, string):
		self.f.write(string + '\n')
		self.f.flush()

# pyepics camonitor doesn't output the initial value of the PV when it starts up.
# It only reports subsequent changes.
# I want to know the initial value.
# It's not a good solution but I'm using pvget to read and print that.
def caGetAndMonitor(PVName, File):
	PV = epics.PV(PVName)
	File.writeln("%s %s %s" % (PVName, datetime.datetime.fromtimestamp(PV.timestamp).strftime('%Y-%m-%d %H:%M:%S'), PV.value))
	epics.camonitor(PVName, File.writeln)

FirstPump='1'
if len(sys.argv) > 1:
	FirstPump=sys.argv[1]
	
LastPump=FirstPump
if len(sys.argv) > 2:
	LastPump=sys.argv[2]

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

			
ChannelDefaultRoot = os.getenv('ASYNSIMPORT', 'VQM_ITMS_SIMM')
for Pump in range(int(FirstPump), int(LastPump)+1):
	ChannelRoot = os.getenv('ASYNSIMPORT'+str(Pump), ChannelDefaultRoot+':'+str(Pump))
	FileName = ChannelRoot
	FileName = FileName.replace(':', '_')
	FileName = FileName + '.log'
	File = cFile(FileName, 'w')
	for index, PVName in enumerate(PVNames):
		caGetAndMonitor(ChannelRoot + ":" + PVName, File)

Channel1Root = os.getenv('ASYNSIMPORT'+str(FirstPump), ChannelDefaultRoot+':'+str(FirstPump))
chid = epics.ca.create_channel(Channel1Root + ":Running")
while (epics.ca.isConnected(chid)):
	time.sleep(1)

del os.environ["EPICS_CA_SERVER_PORT"]
del os.environ["EPICS_CA_AUTO_ADDR_LIST"]
del os.environ["EPICS_CA_ADDR_LIST"]
