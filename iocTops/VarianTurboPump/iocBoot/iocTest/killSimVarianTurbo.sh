#!/bin/sh

killProcServ() {
	( echo -e "\030"; sleep 1; echo -e "\021"; sleep 1 ) | telnet 127.0.0.1 $1
}

#NOTE ^X = ASCII 030, ^Q = ASCII 021 (octal)
killProcServ 8001
killProcServ 8002
killProcServ 8003
