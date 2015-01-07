EPICS_CA_SERVER_PORT=5071

terminal -e ./camonitor5.sh

IPPORT1=localhost:5066

IPPORT2=localhost:5067

IPPORT3=localhost:5068

IPPORT4=localhost:5069

IPPORT5=localhost:5070

caput TURBOSIM:1:WarningHighLoad 1
sleep 10
caput TURBOSIM:1:WarningTemperature 1
sleep 10
caput TURBOSIM:1:Fault 1
caput TURBOSIM:1:Fault.PROC 1
sleep 10

caput TURBOSIM:2:WarningHighLoad 1
sleep 10
caput TURBOSIM:2:WarningTemperature 1
sleep 10
caput TURBOSIM:2:Fault 1
caput TURBOSIM:2:Fault.PROC 1
sleep 10

caput TURBOSIM:3:WarningHighLoad 1
sleep 10
caput TURBOSIM:3:WarningTemperature 1
sleep 10
caput TURBOSIM:3:Fault 1
caput TURBOSIM:3:Fault.PROC 1
sleep 10

caput TURBOSIM:4:WarningHighLoad 1
sleep 10
caput TURBOSIM:4:WarningTemperature 1
sleep 10
caput TURBOSIM:4:Fault 1
caput TURBOSIM:4:Fault.PROC 1
sleep 10

caput TURBOSIM:5:WarningHighLoad 1
sleep 10
caput TURBOSIM:5:WarningTemperature 1
sleep 10
caput TURBOSIM:5:Fault 1
caput TURBOSIM:5:Fault.PROC 1

