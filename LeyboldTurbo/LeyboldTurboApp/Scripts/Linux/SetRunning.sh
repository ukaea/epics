# get command line argument(s)
if [ $# -eq 0 ]; then Pump=1; else Pump=$1;fi
if [ $# -le 1 ]; then Run=1; else Run=$2;fi

caput TURBO:%Pump%:Running %Run%
caput TURBO:%Pump%:Running.PROC 1

