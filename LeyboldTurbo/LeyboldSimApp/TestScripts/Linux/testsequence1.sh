export EPICS_CA_SERVER_PORT=5071

gnome-terminal -e ./camonitor1.sh

IPPORT1=localhost:5066

caput TURBOSIM:1:WarningHighLoad 1
sleep 10
caput TURBOSIM:1:WarningTemperature 1
sleep 10
caput TURBOSIM:1:Fault 1
caput TURBOSIM:1:Fault.PROC 1

