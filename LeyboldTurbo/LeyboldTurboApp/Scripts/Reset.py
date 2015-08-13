import epics
import sys

Pump='1'
if len(sys.argv) > 1:
	Pump=sys.argv[1]

epics.caput('LEYBOLDTURBO:' + str(Pump) + ':Reset', 1)

