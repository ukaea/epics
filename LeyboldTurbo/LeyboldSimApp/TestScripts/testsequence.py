#####################################################################################################
#																									#
#	Module:																							#
#		testsequience.py																			#
#																									#
#	Description:																					#
#		Uses pyepics to control the simulator values and thereby excercise the (simulated)			#
#		pump through a sequence of value change events.												#
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
import time

def caput(PVName, AsynVersion, Value):
	epics.caput(PVName, Value)
	ChannelDefaultRoot = os.getenv('ASYNSIMPORT', 'LEYBOLDTURBOSIM')
	if AsynVersion < "4-26":
		epics.caput(PVName+'.PROC', 1)

def TestSequenceOnePump(Pump):
	ChannelDefaultRoot = os.getenv('ASYNSIMPORT', 'LEYBOLDTURBOSIM')
	ChannelRoot = os.getenv('ASYNSIMPORT'+str(Pump), ChannelDefaultRoot+':'+str(Pump))

	AsynVersion = epics.caget(ChannelDefaultRoot + ':AsynVersion')
	
	print("First turn the pump off ", ChannelRoot)
	caput(ChannelRoot + ':Running', AsynVersion, 0)

	time.sleep(5)

	print("Then turn it on again ", ChannelRoot)
	caput(ChannelRoot + ':Running', AsynVersion, 1)

	time.sleep(5)

	print("Setting some values indicating high pump stress on ", ChannelRoot)
	caput(ChannelRoot + ':StatorFrequency', AsynVersion, 200 + Pump)
	caput(ChannelRoot + ':ConverterTemperature', AsynVersion, 70 + Pump)
	caput(ChannelRoot + ':MotorCurrent', AsynVersion, 15.4 + Pump)
	caput(ChannelRoot + ':CircuitVoltage', AsynVersion, 40.6 + Pump)
	caput(ChannelRoot + ':WarningHighLoad', AsynVersion, 4096 + 8192 + Pump)

	time.sleep(5)

	print("Setting some more values indicating high pump stress on ", ChannelRoot)
	caput(ChannelRoot + ':PumpTemperature', AsynVersion, 80)
	caput(ChannelRoot + ':WarningTemperature', AsynVersion, 1 + 2 + 4)

	time.sleep(5)

	print("Motor temperature too high on ", ChannelRoot)
	caput(ChannelRoot + ':Fault', AsynVersion, 2)

	time.sleep(5)

	print("Overload on ", ChannelRoot)
	caput(ChannelRoot + ':Fault', AsynVersion, 1)
	time.sleep(5)

	print("No motor current on ", ChannelRoot)
	caput(ChannelRoot + ':Fault', AsynVersion, 17)
	time.sleep(5)

	print("Communication connection lost ", ChannelRoot)
	caput(ChannelRoot + ':Disconnected', AsynVersion, 1)

def ResumeConnectionOnePump(Pump):
	ChannelDefaultRoot = os.getenv('ASYNSIMPORT', 'LEYBOLDTURBOSIM')
	ChannelRoot = os.getenv('ASYNSIMPORT'+str(Pump), ChannelDefaultRoot+':'+str(Pump))
	AsynVersion = epics.caget(ChannelDefaultRoot + ':AsynVersion')
	time.sleep(5)
	print("Communication connection resumed ", ChannelRoot)
	caput(ChannelRoot + ':Disconnected', AsynVersion, 0)

os.environ["EPICS_CA_SERVER_PORT"]="5072"
os.environ["EPICS_CA_AUTO_ADDR_LIST"]="NO"
os.environ["EPICS_CA_ADDR_LIST"]="localhost"

NumPumps='1'
if len(sys.argv) > 1:
	NumPumps=sys.argv[1]

for Pump in range(1, int(NumPumps)+1):
	TestSequenceOnePump(Pump)
	
for Pump in range(1, int(NumPumps)+1):
	ResumeConnectionOnePump(Pump)
	
del os.environ["EPICS_CA_SERVER_PORT"]
del os.environ["EPICS_CA_AUTO_ADDR_LIST"]
del os.environ["EPICS_CA_ADDR_LIST"]

# https://github.com/pyepics/pyepics/issues/34 
# epics.ca.finalize_libca()
