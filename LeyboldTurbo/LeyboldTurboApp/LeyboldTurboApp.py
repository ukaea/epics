#####################################################################################################
#																									#
#	Module:																							#
#		LeyboldTurboApp.py																			#
#																									#
#	Description:																					#
#		Launches the IOC with appropriate parameter values.											#
#		Script parameters are:																		#
#			1. Number of pumps. Default 1.															#
#			2. The size of the PZD area (see hardware documentation). Default 6.					#
#				This will be 6 for older model controllers.											#
#				It will be 2 for the rear port of the MAG Drive Digital pump.						#
#				It will be 6 for the (undocumented) front port of the MAG Drive Digital.			#
#				This also sets the baud rate.														#
#				It will be 19200 for older model controllers.										#
#				It will be 9600 for the rear port of the MAG Drive Digital.							#
#				It will be 19200 for thew front port of the MAG Drive Digital.						#
#			3. If a third parameter is passed, it means the IOC will work with the simulator,		#
#				and not real hardware. Default is real.												#
#		Pyepics is used for the convenience of cross-platform scripting.							#
#																									#
#	Author:  Peter Heesterman (Tessella plc). Date: 03 Sep 2015.									#
#	Written for CCFE (Culham Centre for Fusion Energy).												#
#																									#
#	LeyboldTurbo is distributed subject to a Software License Agreement								#
#	found in file LICENSE that is included with this distribution.									#
#																									#
#####################################################################################################

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
if len(sys.argv) > 3 and sys.argv[3].lower() == "sim":
	Simulated='.simulated'
	os.environ["IPPORT1"]="localhost:5066"
	os.environ["IPPORT2"]="localhost:5067"
	os.environ["IPPORT3"]="localhost:5068"
	os.environ["IPPORT4"]="localhost:5069"
	os.environ["IPPORT5"]="localhost:5070"
else:
	if epics_host_arch.startswith('win'):
		os.environ["COMPORT1"]="COM1:" if len(sys.argv) <= 3 else sys.argv[3]
		os.environ["COMPORT2"]="COM2:" if len(sys.argv) <= 4 else sys.argv[4]
		os.environ["COMPORT3"]="COM3:" if len(sys.argv) <= 5 else sys.argv[5]
		os.environ["COMPORT4"]="COM4:" if len(sys.argv) <= 6 else sys.argv[6]
		os.environ["COMPORT5"]="COM5:" if len(sys.argv) <= 7 else sys.argv[7]
	else:
		os.environ["COMPORT1"]="/dev/ttyS0" if len(sys.argv) <= 3 else sys.argv[3]
		os.environ["COMPORT2"]="/dev/ttyS1" if len(sys.argv) <= 4 else sys.argv[4]
		os.environ["COMPORT3"]="/dev/ttyS2" if len(sys.argv) <= 5 else sys.argv[5]
		os.environ["COMPORT4"]="/dev/ttyS3" if len(sys.argv) <= 6 else sys.argv[6]
		os.environ["COMPORT5"]="/dev/ttyS4" if len(sys.argv) <= 7 else sys.argv[7]

cmdline = '..' + os.sep + 'bin' + os.sep + epics_host_arch + os.sep + 'LeyboldTurboApp ../iocBoot/iocLeyboldTurbo/st' + NumPumps + Simulated + '.cmd'

os.system(cmdline)
