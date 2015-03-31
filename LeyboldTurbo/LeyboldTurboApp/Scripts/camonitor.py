import epics
import os
import sys

NumPumps='1'
if len(sys.argv) > 1:
	NumPumps=sys.argv[1]

for Pump in range(1, int(NumPumps)+1):
	epics.camonitor('TURBO:' + str(Pump) + ':Running')
	epics.camonitor('TURBO:' + str(Pump) + ':FirmwareVersion')
	epics.camonitor('TURBO:' + str(Pump) + ':Reset')
	epics.camonitor('TURBO:' + str(Pump) + ':Fault')
	epics.camonitor('TURBO:' + str(Pump) + ':WarningTemperature')
	epics.camonitor('TURBO:' + str(Pump) + ':WarningHighLoad')
	epics.camonitor('TURBO:' + str(Pump) + ':StatorFrequency')
	epics.camonitor('TURBO:' + str(Pump) + ':ConverterTemperature')
	epics.camonitor('TURBO:' + str(Pump) + ':MotorCurrent')
	epics.camonitor('TURBO:' + str(Pump) + ':PumpTemperature')
	epics.camonitor('TURBO:' + str(Pump) + ':CircuitVoltage')

sys.stdin.read(1)

