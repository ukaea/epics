import epics
import os
import sys

NumPumps='1'
if len(sys.argv) > 1:
	NumPumps=sys.argv[1]


os.environ["EPICS_CA_SERVER_PORT"]="5071"
os.environ["EPICS_CA_AUTO_ADDR_LIST"]="NO"
os.environ["EPICS_CA_ADDR_LIST"]="localhost"

def caGetAndMonitor(Pump, PV):
	epics.caget('LEYBOLDTURBOSIM:' + str(Pump) + ':' + PV, use_monitor=True)
	epics.camonitor('LEYBOLDTURBOSIM:' + str(Pump) + ':' + PV)
	return;

#		CircuitVoltage:			The pump's circuit voltage value.									#
for Pump in range(1, int(NumPumps)+1):
	caGetAndMonitor(Pump, 'Running')
	caGetAndMonitor(Pump, 'Fault')
	caGetAndMonitor(Pump, 'FirmwareVersion')
	caGetAndMonitor(Pump, 'WarningTemperature')
	caGetAndMonitor(Pump, 'WarningPurge')
	caGetAndMonitor(Pump, 'StatorFrequency')
	caGetAndMonitor(Pump, 'ConverterTemperature')
	caGetAndMonitor(Pump, 'MotorCurrent')
	caGetAndMonitor(Pump, 'PumpTemperature')
	caGetAndMonitor(Pump, 'CircuitVoltage')

sys.stdin.read(1)