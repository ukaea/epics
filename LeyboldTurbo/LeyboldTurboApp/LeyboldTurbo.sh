if [ $# -eq 0 ]; then NumPumps=1; else NumPumps=$1;fi

export COMPORT1=/dev/ttyS0
export COMPORT2=/dev/ttyS1
export COMPORT3=/dev/ttyS2
export COMPORT4=/dev/ttyS3
export COMPORT5=/dev/ttyS4
export IPPORT1=localhost:5066
export IPPORT2=localhost:5067
export IPPORT3=localhost:5068
export IPPORT4=localhost:5069
export IPPORT5=localhost:5070
../bin/linux-x86_64/LeyboldTurbo ../iocBoot/iocLeyboldTurbo/st$NumPumps.simulated.cmd
