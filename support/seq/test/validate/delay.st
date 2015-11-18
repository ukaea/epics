/*************************************************************************\
This file is distributed subject to a Software License Agreement found
in the file LICENSE that is included with this distribution.
\*************************************************************************/
program delayTest
/* snc test program to demonstrate delay and state transition without
   using external events.
*/
option +s;

%%#include "pv.h"

double v = 0;
assign v;

ss test {
    double thr = 5.0;
    double hyst = 0.45;
    state low {
        entry {
            printf("state low: v = %g\n",v);
            printf("state low: thr = %g, hyst = %g\n",thr,hyst);
        }
        when (v >= thr+hyst) {
            printf("low, v = %g, transition to high\n",v);
        } state high
    }
    state high {
        entry {
            printf("state high: v = %g\n",v);
            printf("state high: thr = %g, hyst = %g\n",thr,hyst);
        }
        when (v <= thr-hyst) {
            printf("high, v = %g, transition to low\n",v);
        } state low
    }
}

ss ramp {
    double lo = 4.0;
    double hi = 6.0;
    double step = 0.1;
    double dt = 0.1;
    double t = 0.0;
    double start_time;
    state init {
        when() {
            pvTimeGetCurrentDouble(&start_time);
        } state up
    }
    state up {
        when (v >= hi) {
            double real_time;
            pvTimeGetCurrentDouble(&real_time);
            v = hi;
            printf("go down, v = %g, t = %g, rt = %g\n",v,t,real_time-start_time);
            pvPut(v);
        } state down
        when (delay(dt)) {
            double real_time;
            pvTimeGetCurrentDouble(&real_time);
            t += dt;
            v += step;
            printf("up, v = %g, t = %g, rt = %g\n",v,t,real_time-start_time);
            pvPut(v);
        } state up
    }
    state down {
        when (v <= lo) {
            double real_time;
            pvTimeGetCurrentDouble(&real_time);
            v = lo;
            printf("go up, v = %g, t = %g, rt = %g\n",v,t,real_time-start_time);
            pvPut(v);
        } state up
        when (delay(dt)) {
            double real_time;
            pvTimeGetCurrentDouble(&real_time);
            v -= step;
            t += dt;
            printf("down, v = %g, t = %g, rt = %g\n",v,t,real_time-start_time);
            pvPut(v);
        } state down
    }
}
