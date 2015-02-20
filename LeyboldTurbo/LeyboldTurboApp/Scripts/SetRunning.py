import epics
import sys

Pump='1'
if len(sys.argv) > 1:
	Pump=sys.argv[1]
	
Run='1'
if len(sys.argv) > 2:
	Run=sys.argv[2]

epics.caput('TURBO:' + str(Pump) + ':Running', Run)
epics.caput('TURBO:' + str(Pump) + ':Running.PROC', 1)
