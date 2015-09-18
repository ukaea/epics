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

gnome-terminal -e "python LeyboldSim.py $NUMPUMPS $NOOFPZD"

gnome-terminal -e "python ../../LeyboldTurboApp/Scripts/LeyboldTurboApp.py $NUMPUMPS $NOOFPZD Sim"

gnome-terminal -e "python camonitor.py 1 $NUMPUMPS"
gnome-terminal -e "python ../../LeyboldTurboApp/Scripts/camonitor.py 1 $NUMPUMPS"

python testsequence.py $NUMPUMPS

# NB - there is an issue here.
# This script previously invoked ../../LeyboldTurboApp/Scripts/Reset.py and ../../LeyboldTurboApp/Scripts/SetRunning.py.
# However, each caput/caget causes warnings due to multiple IP nodes - IP V4. IP V6 and localhost, that have the same PV.
# (The fact that these are one and the same is not taken in to account.)
# When there are too many warnings, the (pytyhon) script fails with errors:
# "sys.excepthook is missing".
sleep 10
for ((Pump=1; Pump<=$NUMPUMPS; Pump++))
do
	caput LEYBOLDTURBO:$Pump:Reset 1
done

sleep 10
for ((Pump=1; Pump<=$NUMPUMPS; Pump++))
do
	caput LEYBOLDTURBO:$Pump:Running 1
done

