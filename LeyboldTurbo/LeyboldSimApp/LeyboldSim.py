import os
import sys
os.chdir(os.path.dirname(sys.argv[0]))

NumPumps='1'
if len(sys.argv) > 1:
	NumPumps=sys.argv[1]
	
NoOfPZD = 6
if len(sys.argv) > 2:
	NoOfPZD=sys.argv[2]

os.environ["EPICS_CA_SERVER_PORT"]="5071"

os.environ["NOOFPZD"]=NoOfPZD

os.environ["IPPORT1"]="localhost:5066"

os.environ["IPPORT2"]="localhost:5067"

os.environ["IPPORT3"]="localhost:5068"

os.environ["IPPORT4"]="localhost:5069"

os.environ["IPPORT5"]="localhost:5070"

epics_host_arch = os.getenv('EPICS_HOST_ARCH', 'win32-x86')

cmdline = '..' + os.sep + 'bin' + os.sep + epics_host_arch + os.sep + 'LeyboldSim ../iocBoot/iocLeyboldSim/st' + NumPumps + '.simulator.cmd'

os.system(cmdline)
