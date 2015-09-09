#####################################################################################################
#																									#
#	Module:																							#
#		setrunning.py																				#
#																									#
#	Description:																					#
#		Uses pyepics to set (or unset) the 'Running' state of the simulator.						#
#		NB, this is equivalent to the equipment user physically switching the pump on.				#
#		It is also possible for the software user to turn the pump on or off.						#
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

os.environ["EPICS_CA_SERVER_PORT"]="5071"
os.environ["EPICS_CA_AUTO_ADDR_LIST"]="NO"
os.environ["EPICS_CA_ADDR_LIST"]="localhost"

FirstPump='1'
if len(sys.argv) > 1:
	FirstPump=sys.argv[1]
	
LastPump='1'
if len(sys.argv) > 2:
	LastPump=sys.argv[2]
	
Run='1'
if len(sys.argv) > 3:
	Run=sys.argv[3]
	
for Pump in range(int(FirstPump), int(LastPump)+1):
	time.sleep(10)
	print("Setting pump ", Pump)
	epics.caput("LEYBOLDTURBOSIM:" + str(Pump) + ":Running", Run)
	if "ASYN_VER" not in os.environ or os.environ["ASYN_VER"]<"4-26":
		epics.caput("LEYBOLDTURBOSIM:" + str(Pump) + ":Running.PROC", 1)
