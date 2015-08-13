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
	epics.camonitor('LEYBOLDTURBOSIM:' + str(Pump) + ':Running')
	epics.camonitor('LEYBOLDTURBOSIM:' + str(Pump) + ':FirmwareVersion')
	epics.camonitor('LEYBOLDTURBOSIM:' + str(Pump) + ':Fault')
	epics.camonitor('LEYBOLDTURBOSIM:' + str(Pump) + ':WarningTemperature')
	epics.camonitor('LEYBOLDTURBOSIM:' + str(Pump) + ':WarningHighLoad')
	epics.camonitor('LEYBOLDTURBOSIM:' + str(Pump) + ':Fault')
	epics.camonitor('LEYBOLDTURBOSIM:' + str(Pump) + ':StatorFrequency')
	epics.camonitor('LEYBOLDTURBOSIM:' + str(Pump) + ':ConverterTemperature')
	epics.camonitor('LEYBOLDTURBOSIM:' + str(Pump) + ':MotorCurrent')
	epics.camonitor('LEYBOLDTURBOSIM:' + str(Pump) + ':PumpTemperature')
	epics.camonitor('LEYBOLDTURBOSIM:' + str(Pump) + ':CircuitVoltage')

sys.stdin.read(1)