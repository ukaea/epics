#####################################################################################################
#																									#
#	Module:																							#
#		VQM_ITMSApp.py																			#
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
#	VQM_ITMS is distributed subject to a Software License Agreement								#
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

epics_host_arch = os.getenv('EPICS_HOST_ARCH', 'win32-x86-static')

Simulated=''
if len(sys.argv) > 3 and sys.argv[3].lower() == "sim":
	Simulated='.simulated'
else:
	if epics_host_arch.startswith('win'):
		for ComPort in range(1, int(NumPumps)+1):
			# 1 -> 1, 2 -> 2, etc - but overridden by command line values
			os.environ["COMPORT" + str(ComPort)]="COM"+chr(ComPort)+":" if len(sys.argv) <= ComPort+2 else sys.argv[ComPort+2]
	else:
		for ComPort in range(1, int(NumPumps)+1):
			# 1 -> 0, 2 -> 1, etc - but overridden by command line values
			os.environ["COMPORT" + str(ComPort)]="/dev/ttyS"+chr(ComPort-1)+":" if len(sys.argv) <= ComPort+2 else sys.argv[ComPort+2]

cmdline = '..' + os.sep + '..' + os.sep + 'bin' + os.sep + epics_host_arch + os.sep + 'VQM_ITMSApp ../../iocBoot/iocVQM_ITMS/st' + NumPumps + Simulated + '.cmd'

os.system(cmdline)
