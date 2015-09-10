import os
import sys

os.chdir(os.path.dirname(os.path.abspath(sys.argv[0])))

NumPumps='1'
if len(sys.argv) > 1:
	NumPumps=sys.argv[1]
	
NoOfPZD = '6'
if len(sys.argv) > 2:
	NoOfPZD=sys.argv[2]

os.environ["EPICS_CA_SERVER_PORT"]="5071"

os.environ["NOOFPZD"]=NoOfPZD

for IpPort in range(1, int(NumPumps)+1):
	# 1 -> 6066, 2 -> 5067, etc.
	os.environ["IPPORT" + str(IpPort)]="localhost:506" + str(5+IpPort)

epics_host_arch = os.getenv('EPICS_HOST_ARCH', 'win32-x86')

if "ASYN_VER" in os.environ and os.environ["ASYN_VER"]>="4-26":
	print ("using asyn:READBACK")
	os.environ["DB"]="LeyboldSim.Asyn4-26"

cmdline = '..' + os.sep + 'bin' + os.sep + epics_host_arch + os.sep + 'LeyboldSim ../iocBoot/iocLeyboldSim/st' + NumPumps + '.simulator.cmd'
print("Command Line ", cmdline)

os.system(cmdline)
