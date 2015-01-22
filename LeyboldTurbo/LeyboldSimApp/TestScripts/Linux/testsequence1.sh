export EPICS_CA_SERVER_PORT=5071

# get command line argument(s)
if [ $# -eq 0 ]; then Pump=1; else Pump=$1;fi

caput TURBOSIM:$Pump:WarningHighLoad 1
caput TURBOSIM:$Pump:WarningHighLoad.PROC 1
sleep 10
caput TURBOSIM:$Pump:WarningTemperature 1
caput TURBOSIM:$Pump:WarningTemperature.PROC 1
sleep 10
caput TURBOSIM:$Pump:Fault 1
caput TURBOSIM:$Pump:Fault.PROC 1

