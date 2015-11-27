program pvArrayTest

double a[1];
assign a to {};

ss simple {
    state simple {
        when () {
            /* these should all give a compiler error */
            pvAssign(a,"");
            pvAssigned(a);
            pvConnected(a);
            pvCount(a);
            pvFlushQ(a);
            pvFreeQ(a);
            pvGet(a);
            pvGetCancel(a);
            pvGetComplete(a);
            pvGetQ(a);
            pvIndex(a);
            pvMessage(a);
            pvMonitor(a);
            pvPut(a,1.1);
            pvPutCancel(a);
            pvPutComplete(a);
            pvSeverity(a);
            pvStatus(a);
            pvStopMonitor(a);
            pvSync(a,NOEVFLAG);
            pvTimeStamp(a);

            /* these should all work */
            pvArrayConnected(a,1);
            pvArrayGetCancel(a,1);
            pvArrayGetComplete(a,1);
            pvArrayMonitor(a,1);
            pvArrayPutCancel(a,1);
            pvArrayPutComplete(a,1);
            pvArrayStopMonitor(a,1);
            pvArraySync(a,1,NOEVFLAG);
        } exit
    }
}
