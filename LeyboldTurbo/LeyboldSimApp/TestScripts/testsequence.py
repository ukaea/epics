import epics
import os
import sys
import time

def TestSequenceOnePump(Pump):
	print("Setting some values indicating high pump stress on ", Pump)
	ChannelRoot = 'LEYBOLDTURBOSIM:' + str(Pump)
	epics.caput(ChannelRoot + ':StatorFrequency', 200)
	epics.caput(ChannelRoot + ':StatorFrequency.PROC', 1)
	epics.caput(ChannelRoot + ':ConverterTemperature', 70)
	epics.caput(ChannelRoot + ':ConverterTemperature.PROC', 1)
	epics.caput(ChannelRoot + ':MotorCurrent', 15.4)
	epics.caput(ChannelRoot + ':MotorCurrent.PROC', 1)
	epics.caput(ChannelRoot + ':CircuitVoltage', 20.6)
	epics.caput(ChannelRoot + ':CircuitVoltage.PROC', 1)
	epics.caput(ChannelRoot + ':WarningHighLoad', 1)
	epics.caput(ChannelRoot + ':WarningHighLoad.PROC', 1)

	time.sleep(10)

	print("Setting some more values indicating high pump stress on ", Pump)
	epics.caput(ChannelRoot + ':PumpTemperature', 80)
	epics.caput(ChannelRoot + ':WarningTemperature', 1)
	epics.caput(ChannelRoot + ':WarningTemperature.PROC', 1)

	time.sleep(10)

	print("Motor temperature too high on ", Pump)
	epics.caput(ChannelRoot + ':Fault', 2)
	epics.caput(ChannelRoot + ':Fault.PROC', 1)

	time.sleep(10)

	print("Overload on ", Pump)
	epics.caput(ChannelRoot + ':Fault', 1)
	epics.caput(ChannelRoot + ':Fault.PROC', 1)
	time.sleep(10)

	print("No motor current on ", Pump)
	epics.caput(ChannelRoot + ':Fault', 17)
	epics.caput(ChannelRoot + ':Fault.PROC', 1)

os.environ["EPICS_CA_SERVER_PORT"]="5071"
os.environ["EPICS_CA_AUTO_ADDR_LIST"]="NO"
os.environ["EPICS_CA_ADDR_LIST"]="localhost"

NumPumps='1'
if len(sys.argv) > 1:
	NumPumps=sys.argv[1]

for Pump in range(1, int(NumPumps)+1):
	TestSequenceOnePump(Pump)
	time.sleep(10)
	
