import epics
import os
import sys

NumPumps='1'
if len(sys.argv) > 1:
	NumPumps=sys.argv[1]
	
os.environ["EPICS_CA_AUTO_ADDR_LIST"]="NO"
os.environ["EPICS_CA_ADDR_LIST"]="localhost"

for Pump in range(1, int(NumPumps)+1):
	epics.camonitor('LEYBOLDTURBO:' + str(Pump) + ':Running')
	epics.camonitor('LEYBOLDTURBO:' + str(Pump) + ':FirmwareVersion')
	epics.camonitor('LEYBOLDTURBO:' + str(Pump) + ':Reset')
	epics.camonitor('LEYBOLDTURBO:' + str(Pump) + ':Fault')
	epics.camonitor('LEYBOLDTURBO:' + str(Pump) + ':FaultStr')
	epics.camonitor('LEYBOLDTURBO:' + str(Pump) + ':WarningTemperature')
	epics.camonitor('LEYBOLDTURBO:' + str(Pump) + ':WarningTemperatureStr')
	epics.camonitor('LEYBOLDTURBO:' + str(Pump) + ':WarningHighLoad')
	epics.camonitor('LEYBOLDTURBO:' + str(Pump) + ':WarningHighLoadStr')
	epics.camonitor('LEYBOLDTURBO:' + str(Pump) + ':WarningPurge')
	epics.camonitor('LEYBOLDTURBO:' + str(Pump) + ':WarningPurgeStr')
	epics.camonitor('LEYBOLDTURBO:' + str(Pump) + ':StatorFrequency')
	epics.camonitor('LEYBOLDTURBO:' + str(Pump) + ':ConverterTemperature')
	epics.camonitor('LEYBOLDTURBO:' + str(Pump) + ':MotorCurrent')
	epics.camonitor('LEYBOLDTURBO:' + str(Pump) + ':PumpTemperature')
	epics.camonitor('LEYBOLDTURBO:' + str(Pump) + ':CircuitVoltage')

sys.stdin.read(1)

