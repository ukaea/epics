echo off
set EPICS_CA_SERVER_PORT=5071

set IPPORT1=localhost:5066

caput TURBOSIM:1:WarningHighLoad 1
timeout 10
caput TURBOSIM:1:WarningTemperature 1
timeout 10
caput TURBOSIM:1:Fault 1
caput TURBOSIM:1:Fault.PROC 1

