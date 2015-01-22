# get command line argument(s)
if [ $# -eq 0 ]; then Pump=1; else Pump=$1;fi

caput TURBO:$Pump:Reset 1

