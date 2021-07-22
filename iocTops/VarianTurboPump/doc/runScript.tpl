#!/bin/sh

export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${EPICS_MODULES}/support/asyn/4-13/lib/_ARCH_"
export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${EPICS_MODULES}/support/sscan/2-6-6/lib/_ARCH_"
export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${EPICS_MODULES}/support/calc/2-8/lib/_ARCH_"
export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${EPICS_MODULES}/support/stream/2-4-1/lib/_ARCH_"
export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${EPICS_LOCAL}/drivers/MoxaSerialMode/1-0/lib/_ARCH_"

../../bin/_ARCH_/VarianTurboPump st.cmd
