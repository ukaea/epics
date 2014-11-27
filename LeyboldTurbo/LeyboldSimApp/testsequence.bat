echo off
set EPICS_CA_SERVER_PORT=5071

set IPPORT1=localhost:5066

set IPPORT2=localhost:5067

set IPPORT3=localhost:5068

set IPPORT4=localhost:5069

set IPPORT5=localhost:5070

caput TURBOSIM:1:WarningHighLoad 1
Wait 10000
caput TURBOSIM:1:WarningTemperature 1
wait 10000
caput TURBOSIM:1:Fault 1

