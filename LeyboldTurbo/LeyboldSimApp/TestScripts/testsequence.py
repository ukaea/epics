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

def caput(PVName, Value):
	epics.caput(PVName, Value)
	if "ASYN_VER" not in os.environ or os.environ["ASYN_VER"]<"4-26":
		epics.caput(PVName+'.PROC', 1)

def TestSequenceOnePump(Pump):
	ChannelRoot = 'LEYBOLDTURBOSIM:' + str(Pump)

	print("First turn the pump off ", Pump)
	caput(ChannelRoot + ':Running', 0)

	time.sleep(10)

	print("Then turn it on again ", Pump)
	caput(ChannelRoot + ':Running', 1)

	time.sleep(10)

	print("Setting some values indicating high pump stress on ", Pump)
	caput(ChannelRoot + ':StatorFrequency', 200)
	caput(ChannelRoot + ':ConverterTemperature', 70)
	caput(ChannelRoot + ':MotorCurrent', 15.4)
	caput(ChannelRoot + ':CircuitVoltage', 40.6)
	caput(ChannelRoot + ':WarningHighLoad', 1)

	time.sleep(10)

	print("Setting some more values indicating high pump stress on ", Pump)
	caput(ChannelRoot + ':PumpTemperature', 80)
	caput(ChannelRoot + ':WarningTemperature', 1)

	time.sleep(10)

	print("Motor temperature too high on ", Pump)
	caput(ChannelRoot + ':Fault', 2)

	time.sleep(10)

	print("Overload on ", Pump)
	caput(ChannelRoot + ':Fault', 1)
	time.sleep(10)

	print("No motor current on ", Pump)
	caput(ChannelRoot + ':Fault', 17)

os.environ["EPICS_CA_SERVER_PORT"]="5071"
os.environ["EPICS_CA_AUTO_ADDR_LIST"]="NO"
os.environ["EPICS_CA_ADDR_LIST"]="localhost"

NumPumps='1'
if len(sys.argv) > 1:
	NumPumps=sys.argv[1]

for Pump in range(1, int(NumPumps)+1):
	time.sleep(10)
	TestSequenceOnePump(Pump)
	
del os.environ["EPICS_CA_SERVER_PORT"]
del os.environ["EPICS_CA_AUTO_ADDR_LIST"]
del os.environ["EPICS_CA_ADDR_LIST"]

# https://github.com/pyepics/pyepics/issues/34 
# epics.ca.finalize_libca()
