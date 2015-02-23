import epics
import os
import sys

NumPumps='1'
if len(sys.argv) > 1:
	NumPumps=sys.argv[1]

os.environ["EPICS_CA_SERVER_PORT"]="5071"
os.environ["EPICS_CA_AUTO_ADDR_LIST"]="NO"
os.environ["EPICS_CA_ADDR_LIST"]="localhost"

for Pump in range(1, int(NumPumps)+1):
	epics.camonitor('TURBOSIM:' + str(Pump) + ':Running')
	epics.camonitor('TURBOSIM:' + str(Pump) + ':Fault')
	epics.camonitor('TURBOSIM:' + str(Pump) + ':WarningTemperature')
	epics.camonitor('TURBOSIM:' + str(Pump) + ':WarningHighLoad')
	epics.camonitor('TURBOSIM:' + str(Pump) + ':Fault')
	epics.camonitor('TURBOSIM:' + str(Pump) + ':StatorFrequency')
	epics.camonitor('TURBOSIM:' + str(Pump) + ':ConverterTemperature')
	epics.camonitor('TURBOSIM:' + str(Pump) + ':MotorCurrent')
	epics.camonitor('TURBOSIM:' + str(Pump) + ':PumpTemperature')
	epics.camonitor('TURBOSIM:' + str(Pump) + ':CircuitVoltage')

sys.stdin.read(1)