#!/bin/sh
set -e -x

make -j2 $EXTRA

if [ "$TEST" != "NO" ]
then
  make -j2 tapfiles
  make -j2 -s test-results
fi
