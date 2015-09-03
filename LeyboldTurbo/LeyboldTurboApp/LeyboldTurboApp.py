import os
import sys
os.chdir(os.path.dirname(os.path.realpath(sys.argv[0])))


NumPumps='1'
if len(sys.argv) > 1:
	NumPumps=sys.argv[1]

NoOfPZD = '6'
if len(sys.argv) > 2:
	NoOfPZD=sys.argv[2]

os.environ["BAUD"]="19200"
if NoOfPZD=='2':
	os.environ["BAUD"]="9600"

	
os.environ["NOOFPZD"]=NoOfPZD

epics_host_arch = os.getenv('EPICS_HOST_ARCH', 'win32-x86')

Simulated=''
if len(sys.argv) > 3:
	Simulated='.simulated'
	os.environ["IPPORT1"]="localhost:5066"
	os.environ["IPPORT2"]="localhost:5067"
	os.environ["IPPORT3"]="localhost:5068"
	os.environ["IPPORT4"]="localhost:5069"
	os.environ["IPPORT5"]="localhost:5070"
else:
	if epics_host_arch.startswith('win'):
		os.environ["COMPORT1"]="COM1:"
		os.environ["COMPORT2"]="COM2:"
		os.environ["COMPORT3"]="COM3:"
		os.environ["COMPORT4"]="COM4:"
		os.environ["COMPORT5"]="COM5:"
	else:
		os.environ["COMPORT1"]="/dev/ttyS0"
		os.environ["COMPORT2"]="/dev/ttyS1"
		os.environ["COMPORT3"]="/dev/ttyS2"
		os.environ["COMPORT4"]="/dev/ttyS3"
		os.environ["COMPORT5"]="/dev/ttyS4"

cmdline = '..' + os.sep + 'bin' + os.sep + epics_host_arch + os.sep + 'LeyboldTurboApp ../iocBoot/iocLeyboldTurbo/st' + NumPumps + Simulated + '.cmd'

os.system(cmdline)
