#!/bin/sh
# Copyright 2007 Canadian Light Source, Inc.
# 
V1=0
V2=0
V3=0

while getopts "r:p:ndmu" FlagChar ; do
	case "$FlagChar" in
	n) let V=V1*10000+V2*100+V3; echo $V;;
	d) echo ${V1}.${V2}.${V3} ;;
	m) echo ${V1} ;;
	u) echo ${V1}_${V2}_${V3} ;;
	r) eval `echo "$OPTARG" | awk -F. ' { printf "V1=%s V2=%s V3=%s\n", $1, $2, $3}'` ;;
	p) eval `echo "$OPTARG" | awk ' { v=$(NF-1);n=split(v,ar,/\./); printf "V1=%s ", ar[1]; if(n>1) printf "V2=%s ", ar[2]; if(n>3) printf "V3=%s ", ar[3]; print "";}' `
	esac
done
