EPICS_CA_SERVER_PORT=5071

# get command line argument(s)
if [ $# -eq 0 ]; then Pump=1; else Pump=$1;fi
if [ $# -le 1 ]; then Run=1; else Run=$2;fi

caput TURBOSIM:$Pump:Running $Run
caput TURBOSIM:$Pump:Running.PROC 1

