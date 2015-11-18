#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "epicsEvent.h"
#include "cadef.h"

struct var {
    struct dbr_time_long get_val;
    dbr_long_t put_val;
    epicsEventId connected;
    epicsEventId put_done;
    epicsEventId get_done;
    chid ch;
};

static struct var var1;
static struct var var2;

static void connectionHandler (struct connection_handler_args args)
{
    struct var *pvar = (struct var *)ca_puser(args.chid);
    switch (args.op) {
    case CA_OP_CONN_UP:
        epicsEventSignal(pvar->connected);
        break;
    case CA_OP_CONN_DOWN:
        exit(EXIT_FAILURE);
    }
}

static void putHandler(struct event_handler_args args)
{
    if (args.status == ECA_NORMAL) {
        struct var *pvar = (struct var *)ca_puser(args.chid);
#if 0
        printf("put type == %ld\n", args.type);
        printf("put count == %lu\n", args.count);
        assert(args.type == DBR_LONG);
        assert(args.count == 1);
#endif
        epicsEventSignal(pvar->put_done);
    } else {
        fprintf(stderr,"bad CA put response for channel %s: %s\n", ca_name(args.chid), ca_message(args.status));
    }
}

static void getHandler(struct event_handler_args args)
{
    if (args.status == ECA_NORMAL) {
        struct var *pvar = (struct var *)ca_puser(args.chid);
        printf("get type == %ld\n", args.type);
        assert(args.type == DBR_TIME_LONG);
        assert(args.count == 1);
#if 0
#endif
        memcpy(&pvar->get_val, args.dbr, sizeof(pvar->get_val));
        epicsEventSignal(pvar->get_done);
    } else {
        fprintf(stderr,"bad CA get response for channel %s: %s\n", ca_name(args.chid), ca_message(args.status));
    }
}

int main ()
{
    int n = 0;
    int wait_status;
    var1.connected = epicsEventMustCreate(epicsEventEmpty);
    var1.put_done = epicsEventMustCreate(epicsEventEmpty);
    var1.get_done = epicsEventMustCreate(epicsEventEmpty);
    var2.connected = epicsEventMustCreate(epicsEventEmpty);
    var2.put_done = epicsEventMustCreate(epicsEventEmpty);
    var2.get_done = epicsEventMustCreate(epicsEventEmpty);
    SEVCHK(ca_context_create(ca_enable_preemptive_callback),"ca_context_create");
    SEVCHK(ca_create_channel("pvGetComplete1", connectionHandler, &var1, CA_PRIORITY_DEFAULT, &var1.ch),"");
    SEVCHK(ca_create_channel("pvGetComplete2", connectionHandler, &var2, CA_PRIORITY_DEFAULT, &var2.ch),"");
    epicsEventWait(var1.connected);
    epicsEventWait(var2.connected);
    printf("connected\n");
    while (n < 10) {
        var1.put_val = 2*n;
        var2.put_val = 2*n+1;
        var1.get_val.value = 0;
        var2.get_val.value = 0;
        SEVCHK(ca_put_callback(DBR_LONG, var1.ch, &var1.put_val, putHandler, 0),"");
        SEVCHK(ca_put_callback(DBR_LONG, var2.ch, &var2.put_val, putHandler, 0),"");
        ca_flush_io();
#if 0
        printf("waiting for put\n");
#endif
        wait_status = epicsEventWait(var1.put_done);
        assert(wait_status == epicsEventWaitOK);
        wait_status = epicsEventWait(var2.put_done);
        assert(wait_status == epicsEventWaitOK);
#if 0
        printf("put done\n");
#endif
        SEVCHK(ca_get_callback(DBR_TIME_LONG, var1.ch, getHandler, 0),"");
        SEVCHK(ca_get_callback(DBR_TIME_LONG, var2.ch, getHandler, 0),"");
        ca_flush_io();
#if 0
        printf("waiting for get\n");
#endif
        wait_status = epicsEventWait(var1.get_done);
        assert(wait_status == epicsEventWaitOK);
        wait_status = epicsEventWait(var2.get_done);
        assert(wait_status == epicsEventWaitOK);
#if 0
        printf("get done\n");
#endif
        if (var1.put_val != var1.get_val.value) {
            fprintf(stderr, "var1: put=%d, get=%d\n", var1.put_val, var1.get_val.value);
            exit(EXIT_FAILURE);
        }
        if (var2.put_val != var2.get_val.value) {
            fprintf(stderr, "var2: put=%d, get=%d\n", var2.put_val, var2.get_val.value);
            exit(EXIT_FAILURE);
        }
#if 0
        printf("test %d done\n", n);
#endif
        n++;
    }
    printf("done\n");
    return 0;
}
