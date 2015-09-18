import os
import sys

os.chdir(os.path.dirname(os.path.abspath(sys.argv[0])))

NumPumps='1'
if len(sys.argv) > 1:
	NumPumps=sys.argv[1]
	
NoOfPZD = '6'
if len(sys.argv) > 2:
	NoOfPZD=sys.argv[2]

os.environ["NOOFPZD"]=NoOfPZD

epics_host_arch = os.getenv('EPICS_HOST_ARCH', 'win32-x86-static')

if "ASYN_VER" in os.environ and os.environ["ASYN_VER"]>="4-26":
	print ("using asyn:READBACK")
	os.environ["SIMDB"]="LeyboldSim.Asyn4-26"

cmdline = '..' + os.sep + '..' + os.sep + 'bin' + os.sep + epics_host_arch + os.sep + 'LeyboldSim ../../iocBoot/iocLeyboldSim/st' + NumPumps + '.simulator.cmd'
print("Command Line ", cmdline)

os.system(cmdline)
