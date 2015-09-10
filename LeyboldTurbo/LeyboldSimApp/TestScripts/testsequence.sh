#####################################################################################################
#																									#
#	Module:																							#
#		testsequence.sh																				#
#																									#
#	Description:																					#
#		This is a Linux wrapper that invokes a series of seperate test scripts in order to 			#
# 		excercise a test sequence.																	#
# 		Most of the individual test scripts are Python and Pyepics based.							#
#																									#
#	Author:  Peter Heesterman (Tessella plc). Date: 03 Sep 2015.									#
#	Written for CCFE (Culham Centre for Fusion Energy).												#
#																									#
#	LeyboldTurbo is distributed subject to a Software License Agreement								#
#	found in file LICENSE that is included with this distribution.									#
#																									#
#####################################################################################################

NUMPUMPS="1"; 
if [ $# -ge 1 ]; then NUMPUMPS=$1; fi
echo "Number of pumps " $NUMPUMPS

NOOFPZD="6";
if [ $# -ge 2 ]; then NOOFPZD=$2; fi
echo "Number of pzd " $NOOFPZD

gnome-terminal -e "python ../LeyboldSim.py $NUMPUMPS $NOOFPZD"

gnome-terminal -e "python ../../LeyboldTurboApp/LeyboldTurboApp.py $NUMPUMPS $NOOFPZD Sim"

gnome-terminal -e "python camonitor.py $NUMPUMPS"
gnome-terminal -e "python ../../LeyboldTurboApp/Scripts/camonitor.py $NUMPUMPS"

python testsequence.py $NUMPUMPS

sleep 10
python ../../LeyboldTurboApp/Scripts/Reset.py 1 $NUMPUMPS

sleep 10
python ../../LeyboldTurboApp/Scripts/SetRunning.py 1 $NUMPUMPS 1

