import epics
import os
import sys
import time

os.environ["EPICS_CA_SERVER_PORT"]="5071"
os.environ["EPICS_CA_AUTO_ADDR_LIST"]="NO"
os.environ["EPICS_CA_ADDR_LIST"]="localhost"

NumPumps='1'
if len(sys.argv) > 1:
	NumPumps=sys.argv[1]
	
Run='1'
if len(sys.argv) > 2:
	Run=sys.argv[2]
	

for Pump in range(1, int(NumPumps)+1):
	time.sleep(10)
	print("Setting pump ", Pump)
	epics.caput("LEYBOLDTURBOSIM:" + str(Pump) + ":Running", Run)
	epics.caput("LEYBOLDTURBOSIM:" + str(Pump) + ":Running.PROC", 1)
