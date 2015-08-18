import epics
import os
import sys

os.environ["EPICS_CA_AUTO_ADDR_LIST"]="YES"
os.environ["EPICS_CA_SERVER_PORT"]="5064"

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
	print("Setting pump", Pump, Run)
	epics.caput('LEYBOLDTURBO:' + str(Pump) + ':Running', Run)
	epics.caput('LEYBOLDTURBO:' + str(Pump) + ':Running.PROC', 1)
