echo off
set EPICS_CA_SERVER_PORT=5071

set IPPORT1=localhost:5066

set IPPORT2=localhost:5067

set IPPORT3=localhost:5068

set IPPORT4=localhost:5069

set IPPORT5=localhost:5070

caput TURBOSIM:1:WarningHighLoad 1
timeout 10
caput TURBOSIM:1:WarningTemperature 1
timeout 10
caput TURBOSIM:1:Fault 1
caput TURBOSIM:1:Fault.PROC 1
timeout 10

caput TURBOSIM:2:WarningHighLoad 1
timeout 10
caput TURBOSIM:2:WarningTemperature 1
timeout 10
caput TURBOSIM:2:Fault 1
caput TURBOSIM:2:Fault.PROC 1
timeout 10

caput TURBOSIM:3:WarningHighLoad 1
timeout 10
caput TURBOSIM:3:WarningTemperature 1
timeout 10
caput TURBOSIM:3:Fault 1
caput TURBOSIM:3:Fault.PROC 1
timeout 10

caput TURBOSIM:4:WarningHighLoad 1
timeout 10
caput TURBOSIM:4:WarningTemperature 1
timeout 10
caput TURBOSIM:4:Fault 1
caput TURBOSIM:4:Fault.PROC 1
timeout 10

caput TURBOSIM:5:WarningHighLoad 1
timeout 10
caput TURBOSIM:5:WarningTemperature 1
timeout 10
caput TURBOSIM:5:Fault 1
caput TURBOSIM:5:Fault.PROC 1

