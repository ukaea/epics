#!/bin/sh
#
# Script test_EdwardsNextTurbo_GUI.sh
#
# Runs the EDM screen for the Edwards nEXT turbo pump controller.
#

#export EDMDATAFILES=.:../../../data
export EDMDATAFILES=.:../../../data:
export PATH=.:../../../bin/linux-x86_64:${PATH}

edm -eolc -x -m "P=TS-VA-TURBO-01,R=''" EdwardsNextTurbo.edl &
#edm -eolc -x -m "P=TS-VA-TURBO-01,R=''" EdNextTStatus.edl &
#edm -eolc -x -m "P=TS-VA-TURBO-01,R=''" EdNextTService.edl &

exit
