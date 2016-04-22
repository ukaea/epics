#!/bin/sh

runProcServ() {
	if pgrep -f 'procServ .* perl .*:'$2 ; then
		echo "Simulators are already running."
	else
		/home/epics/bin/linux-x86_64/procServ --noautorestart $1 /usr/bin/perl simVarianTurbo_IP.pl -c ip -d 127.0.0.1:${2}
		echo $2
	fi
}

source /home/epics/Startup/startup.R3.14.12.sh

runProcServ 8001 9001
runProcServ 8002 9002
runProcServ 8003 9003
