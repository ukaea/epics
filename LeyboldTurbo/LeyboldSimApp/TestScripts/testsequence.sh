NUMPUMPS="1"; 
if [ $# -ge 1 ]; then NUMPUMPS=$1; fi
echo "Number of pumps " $NUMPUMPS

NOOFPZD="6";
if [ $# -ge 2 ]; then NOOFPZD=$1; fi
echo "Number of pzd " $NOOFPZD

gnome-terminal -e "python ../LeyboldSim.py $NUMPUMPS $NOOFPZD"

gnome-terminal -e "python ../../LeyboldTurboApp/LeyboldTurbo.py $NUMPUMPS $NOOFPZD Sim"

gnome-terminal -e "python camonitor.py $NUMPUMPS"
gnome-terminal -e "python ../../LeyboldTurboApp/Scripts/camonitor.py $NUMPUMPS"

python testsequence.py $NUMPUMPS

sleep 10
for ((Pump=1; Pump<=$NUMPUMPS; Pump++))
do
	caput "LEYBOLDTURBO:$Pump:Reset" 1
	caput "LEYBOLDTURBO:$Pump:Reset.PROC" 1
	sleep 10
	caput "LEYBOLDTURBO:$Pump:Running" 1
	caput "LEYBOLDTURBO:$Pump:Running.PROC" 1
done

