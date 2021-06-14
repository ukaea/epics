#include <assert.h>
#include <limits.h>

#include "errlog.h"
#include "cadef.h"

#define epicsExportSharedSymbols
#include "pv.h"

#define INVOKE(x, expr) \
    {\
        int _status = expr;\
        if (!(_status & CA_M_SUCCESS)) {\
            (x)->msg = ca_message(_status);\
            errlogSevPrintf(sevrFromCA(_status), "%s: %s", #expr, ca_message(_status));\
            return statFromCA(_status);\
        }\
    }

epicsShareDef const struct pvSystem nullPvSys = {NULL};
epicsShareDef const struct pvVar nullPvVar = {NULL,NULL,NULL,NULL,NULL,NULL};

/* utilities */
static pvSevr sevrFromCA(long status);  /* CA severity as pvSevr */
static pvStat statFromCA(long status);  /* CA status as pvStat */
static pvType typeFromCA(long type);    /* DBR type as pvType */
static chtype typeToCA(pvType type);    /* pvType as DBR type */

epicsShareFunc pvStat pvSysCreate(pvSystem *pSys)
{
    assert(pSys);
    assert(!ca_current_context());
    INVOKE(pSys, ca_context_create(ca_enable_preemptive_callback));
    pSys->id = ca_current_context();
    return pvStatOK;
}

epicsShareFunc pvStat pvSysFlush(pvSystem sys)
{
    INVOKE(&sys, ca_flush_io());
    return pvStatOK;
}

epicsShareFunc pvStat pvSysAttach(pvSystem sys)
{
    if (!ca_current_context())
        INVOKE(&sys, ca_attach_context(sys.id));
    return pvStatOK;
}

static void pvCaConnectionHandler(struct connection_handler_args args)
{
    pvVar *var = (pvVar *)ca_puser(args.chid);
    var->conn_handler(args.op == CA_OP_CONN_UP, var->arg);
}

epicsShareFunc pvStat pvVarCreate(pvSystem sys, const char *name,
    pvConnFunc *conn_func, pvEventFunc *event_func, void *arg, pvVar *var)
{
    assert(var);
    var->conn_handler = conn_func;
    var->event_handler = event_func;
    var->arg = arg;
    INVOKE(var, ca_create_channel(name, pvCaConnectionHandler, var, CA_PRIORITY_DEFAULT, &var->chid));
    return pvStatOK;
}

epicsShareFunc pvStat pvVarDestroy(pvVar *var)
{
    assert(var);
    INVOKE(var, ca_clear_channel(var->chid));
    *var = nullPvVar;
    return pvStatOK;
}

static void pvCaEventHandler(struct event_handler_args args, pvEventType evt)
{
    pvVar *var = (pvVar *)ca_puser(args.chid);
    unsigned count = (unsigned)args.count;
    assert(args.count >= 0);
    assert((long)count == args.count);
    var->msg = ca_message(args.status);
    var->event_handler(evt, args.usr, typeFromCA(args.type), count, (pvValue*)args.dbr, statFromCA(args.status));
}

static void pvCaGetHandler(struct event_handler_args args)
{
    pvCaEventHandler(args, pvEventGet);
}

static void pvCaPutHandler(struct event_handler_args args)
{
    pvCaEventHandler(args, pvEventPut);
}

static void pvCaMonitorHandler(struct event_handler_args args)
{
    pvCaEventHandler(args, pvEventMonitor);
}

epicsShareFunc pvStat pvVarGetCallback(pvVar *var, pvType type, unsigned count, void *arg)
{
    assert(var);
    assert(pv_is_valid_type(type));
    INVOKE(var, ca_array_get_callback(
        typeToCA(type), count, var->chid, pvCaGetHandler, arg));
    return pvStatOK;
}

epicsShareFunc pvStat pvVarPutNoBlock(pvVar *var, pvType type, unsigned count, pvValue *value)
{
    assert(var);
    assert(pv_is_simple_type(type));
    INVOKE(var, ca_array_put(typeToCA(type), count, var->chid, value));
    return pvStatOK;
}

epicsShareFunc pvStat pvVarPutCallback(pvVar *var, pvType type, unsigned count, pvValue *value, void *arg)
{
    assert(var);
    assert(pv_is_simple_type(type));
    INVOKE(var, ca_array_put_callback(
        typeToCA(type), count, var->chid, value, pvCaPutHandler, arg));
    return pvStatOK;
}

epicsShareFunc pvStat pvVarMonitorOn(pvVar *var, pvType type, unsigned count, void *arg)
{
    assert(var);
    assert(pv_is_valid_type(type));
    if (var->monid == NULL) {
        INVOKE(var, ca_create_subscription(typeToCA(type), count, var->chid,
            DBE_VALUE | DBE_ALARM, pvCaMonitorHandler, arg, &var->monid));
    }
    return pvStatOK;
}

epicsShareFunc pvStat pvVarMonitorOff(pvVar *var)
{
    assert(var);
    if (var->monid != NULL) {
        INVOKE(var, ca_clear_event(var->monid));
        var->monid = NULL;
    }
    return pvStatOK;
}

epicsShareFunc unsigned pvVarGetCount(pvVar *var)
{
    unsigned long c = ca_element_count(var->chid);
    assert(c <= UINT_MAX);
    return (unsigned)c;
}

epicsShareFunc int pvTimeGetCurrentDouble(double *pTime)
{
    epicsTimeStamp stamp;

    assert(pTime);
    *pTime = 0.0;
    if (epicsTimeGetCurrent(&stamp) != epicsTimeOK)
        return pvStatERROR;

    *pTime = (double) stamp.secPastEpoch +  ((double) stamp.nsec / 1e9);
    return pvStatOK;
}

#include "alarm.h"

static pvSevr sevrFromCA(long status)
{
    switch (CA_EXTRACT_SEVERITY(status)) {
        case CA_K_INFO:    return pvSevrNONE;
        case CA_K_SUCCESS: return pvSevrNONE;
        case CA_K_WARNING: return pvSevrMINOR;
        case CA_K_ERROR:   return pvSevrMAJOR;
        case CA_K_SEVERE:  return pvSevrINVALID;
        default:           return pvSevrERROR;
    }
}

static pvStat statFromCA(long status)
{
    pvSevr sevr = sevrFromCA(status);
    return (sevr == pvSevrNONE || sevr == pvSevrMINOR) ?
                pvStatOK : pvStatERROR;
}

static pvType typeFromCA(long type)
{
    switch (type) {
        case DBR_CHAR:          return pvTypeCHAR;
        case DBR_SHORT:         return pvTypeSHORT;
        case DBR_ENUM:          return pvTypeSHORT;
        case DBR_LONG:          return pvTypeLONG;
        case DBR_FLOAT:         return pvTypeFLOAT;
        case DBR_DOUBLE:        return pvTypeDOUBLE;
        case DBR_STRING:        return pvTypeSTRING;
        case DBR_TIME_CHAR:     return pvTypeTIME_CHAR;
        case DBR_TIME_SHORT:    return pvTypeTIME_SHORT;
        case DBR_TIME_ENUM:     return pvTypeTIME_SHORT;
        case DBR_TIME_LONG:     return pvTypeTIME_LONG;
        case DBR_TIME_FLOAT:    return pvTypeTIME_FLOAT;
        case DBR_TIME_DOUBLE:   return pvTypeTIME_DOUBLE;
        case DBR_TIME_STRING:   return pvTypeTIME_STRING;
        default:                return pvTypeERROR;
    }
}

static chtype typeToCA(pvType type)
{
    switch (type) {
        case pvTypeCHAR:        return DBR_CHAR;
        case pvTypeSHORT:       return DBR_SHORT;
        case pvTypeLONG:        return DBR_LONG;
        case pvTypeFLOAT:       return DBR_FLOAT;
        case pvTypeDOUBLE:      return DBR_DOUBLE;
        case pvTypeSTRING:      return DBR_STRING;
        case pvTypeTIME_CHAR:   return DBR_TIME_CHAR;
        case pvTypeTIME_SHORT:  return DBR_TIME_SHORT;
        case pvTypeTIME_LONG:   return DBR_TIME_LONG;
        case pvTypeTIME_FLOAT:  return DBR_TIME_FLOAT;
        case pvTypeTIME_DOUBLE: return DBR_TIME_DOUBLE;
        case pvTypeTIME_STRING: return DBR_TIME_STRING;
        default:                return -1;
    }
}

#include "db_access.h"

typedef struct dbr_time_char    pvTimeChar;
typedef struct dbr_time_short   pvTimeShort;
typedef struct dbr_time_long    pvTimeLong;
typedef struct dbr_time_float   pvTimeFloat;
typedef struct dbr_time_double  pvTimeDouble;
typedef struct dbr_time_string  pvTimeString;

epicsShareDef const size_t pv_sizes[] = {
    sizeof(pvChar      ),
    sizeof(pvShort     ),
    sizeof(pvLong      ),
    sizeof(pvFloat     ),
    sizeof(pvDouble    ),
    sizeof(pvString    ),
    sizeof(pvTimeChar  ),
    sizeof(pvTimeShort ),
    sizeof(pvTimeLong  ),
    sizeof(pvTimeFloat ),
    sizeof(pvTimeDouble),
    sizeof(pvTimeString),
};

epicsShareDef const size_t pv_value_sizes[] = {
    sizeof(pvChar  ),
    sizeof(pvShort ),
    sizeof(pvLong  ),
    sizeof(pvFloat ),
    sizeof(pvDouble),
    sizeof(pvString),
    sizeof(pvChar  ),
    sizeof(pvShort ),
    sizeof(pvLong  ),
    sizeof(pvFloat ),
    sizeof(pvDouble),
    sizeof(pvString),
};

epicsShareDef const size_t pv_value_offsets[] = {
    0,
    0,
    0,
    0,
    0,
    0,
    offsetof(pvTimeChar  , value),
    offsetof(pvTimeShort , value),
    offsetof(pvTimeLong  , value),
    offsetof(pvTimeFloat , value),
    offsetof(pvTimeDouble, value),
    offsetof(pvTimeString, value),
};

epicsShareDef const size_t pv_status_offsets[] = {
    offsetof(pvTimeChar  , status),
    offsetof(pvTimeShort , status),
    offsetof(pvTimeLong  , status),
    offsetof(pvTimeFloat , status),
    offsetof(pvTimeDouble, status),
    offsetof(pvTimeString, status),
};

epicsShareDef const size_t pv_severity_offsets[] = {
    offsetof(pvTimeChar  , severity),
    offsetof(pvTimeShort , severity),
    offsetof(pvTimeLong  , severity),
    offsetof(pvTimeFloat , severity),
    offsetof(pvTimeDouble, severity),
    offsetof(pvTimeString, severity),
};

epicsShareDef const size_t pv_stamp_offsets[] = {
    offsetof(pvTimeChar  , stamp),
    offsetof(pvTimeShort , stamp),
    offsetof(pvTimeLong  , stamp),
    offsetof(pvTimeFloat , stamp),
    offsetof(pvTimeDouble, stamp),
    offsetof(pvTimeString, stamp),
};
