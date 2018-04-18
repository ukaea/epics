#!/bin/env bash
#/bin/sh
# 
cd "$(dirname "$0")"
#    export HOME_DIR="$(cd "$(dirname "$0")"/../..; pwd)"
# cd "$HOME_DIR"
# run 2 simulated Turbo's
./../../../../data/EdwardsNextTurbo_sim.py -i 9001 &
t1=$!
./../../../../data/EdwardsNextTurbo_sim.py -i 9002 &
t2=$!
echo task ids: $t1 $t2 
./example_EdwardsNextTurbo stexample_EdwardsNextTurbo_sim.boot
kill  $t1 $t2
