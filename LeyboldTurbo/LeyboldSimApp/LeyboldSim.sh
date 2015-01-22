if [ $# -eq 0 ]; then NumPumps=1; else NumPumps=$1;fi
71
export EPICS_CA_SERVER_PORT=5071

export IPPORT1=localhost:5066
export IPPORT2=localhost:5067
export IPPORT3=localhost:5068
export IPPORT4=localhost:5069
export IPPORT5=localhost:5070

# get command line argument(s)
../bin/linux-x86_64/LeyboldSim ../iocBoot/iocLeyboldTurbo/st$NumPumps.simulator.cmd
