#####################################################################################################
#																									#
#	Module:																							#
#		Reset.py																					#
#																									#
#	Description:																					#
#		Uses pyepics to set the 'Reset' flag for the pump.											#
#		NB, this does not need to be cleared - it will happen automatically.						#
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

FirstPump='1'
if len(sys.argv) > 1:
	FirstPump=sys.argv[1]

LastPump=FirstPump
if len(sys.argv) > 2:
	LastPump=sys.argv[2]

os.environ["EPICS_CA_AUTO_ADDR_LIST"]="YES"
os.environ["EPICS_CA_SERVER_PORT"]="5064"

for Pump in range(int(FirstPump), int(LastPump)+1):
	print("Resetting ", Pump)
	epics.caput('LEYBOLDTURBO:' + str(Pump) + ':Reset', 1)

