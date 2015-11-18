/*************************************************************************\
Copyright (c) 2010-2015 Helmholtz-Zentrum Berlin f. Materialien
                        und Energie GmbH, Germany (HZB)
This file is distributed subject to a Software License Agreement found
in the file LICENSE that is included with this distribution.
\*************************************************************************/
#include "builtin.h"

static struct const_symbol const_symbols[] =
{
    {"TRUE",                CT_OTHER },
    {"FALSE",               CT_OTHER },
    {"SYNC",                CT_OTHER },
    {"ASYNC",               CT_OTHER },
    {"NOEVFLAG",            CT_EVFLAG},
    {"pvStatOK",            CT_OTHER },
    {"pvStatERROR",         CT_OTHER },
    {"pvStatDISCONN",       CT_OTHER },
    {"pvStatREAD",          CT_OTHER },
    {"pvStatWRITE",         CT_OTHER },
    {"pvStatHIHI",          CT_OTHER },
    {"pvStatHIGH",          CT_OTHER },
    {"pvStatLOLO",          CT_OTHER },
    {"pvStatLOW",           CT_OTHER },
    {"pvStatSTATE",         CT_OTHER },
    {"pvStatCOS",           CT_OTHER },
    {"pvStatCOMM",          CT_OTHER },
    {"pvStatTIMEOUT",       CT_OTHER },
    {"pvStatHW_LIMIT",      CT_OTHER },
    {"pvStatCALC",          CT_OTHER },
    {"pvStatSCAN",          CT_OTHER },
    {"pvStatLINK",          CT_OTHER },
    {"pvStatSOFT",          CT_OTHER },
    {"pvStatBAD_SUB",       CT_OTHER },
    {"pvStatUDF",           CT_OTHER },
    {"pvStatDISABLE",       CT_OTHER },
    {"pvStatSIMM",          CT_OTHER },
    {"pvStatREAD_ACCESS",   CT_OTHER },
    {"pvStatWRITE_ACCESS",  CT_OTHER },
    {"pvSevrOK",            CT_OTHER },
    {"pvSevrERROR",         CT_OTHER },
    {"pvSevrNONE",          CT_OTHER },
    {"pvSevrMINOR",         CT_OTHER },
    {"pvSevrMAJOR",         CT_OTHER },
    {"pvSevrINVALID",       CT_OTHER },
    {"seqg_var",            CT_OTHER },
    {"seqg_env",            CT_OTHER },
    {0,                     CT_OTHER }
};

/* single parameter descriptors */
static const struct param efP       = { PT_EF, 0 };
static const struct param pvP       = { PT_PV, 0 };
static const struct param pvArrayP  = { PT_PV_ARRAY, 0 };
static const struct param noDefP    = { PT_OTHER, 0 };
static const struct param compTypeP = { PT_OTHER, "DEFAULT" };
static const struct param tmoP      = { PT_OTHER, "DEFAULT_TIMEOUT" };
static const struct param boolP     = { PT_OTHER, "FALSE" };
static const struct param ptrP      = { PT_OTHER, "NULL" };
static const struct param lengthP   = { PT_OTHER, 0 };
static const struct param defLenP   = { PT_OTHER, "1" };

/* multiple parameter descriptors */
static const struct param *noParams[]                    = {0};
static const struct param *otherParams[]                 = {&noDefP,0};
static const struct param *efParams[]                    = {&efP,0};
static const struct param *assignParams[]                = {&pvP,&noDefP,0};
static const struct param *pvParams[]                    = {&pvP,0};
static const struct param *pvArrayParams[]               = {&pvArrayP,&lengthP,0};
static const struct param *pvSyncParams[]                = {&pvP,&efP,0};
static const struct param *pvArraySyncParams[]           = {&pvArrayP,&lengthP,&efP,0};
static const struct param *pvGetPutParams[]              = {&pvP,&compTypeP,&tmoP,0};
static const struct param *pvArrayGetPutCompleteParams[] = {&pvArrayP,&lengthP,&boolP,&ptrP,0};
/* for backward compatibility */
static const struct param *pvPutCompleteParams[]         = {&pvP,&defLenP,&boolP,&ptrP,0};

static struct func_symbol func_symbols[] =
{
    /* name              c_name     action_only cond_only params                    */
    {"delay",               0,          FALSE,  TRUE,   otherParams                 },
    {"efClear",             0,          TRUE,   FALSE,  efParams                    },
    {"efSet",               0,          TRUE,   FALSE,  efParams                    },
    {"efTest",              0,          FALSE,  FALSE,  efParams                    },
    {"efTestAndClear",      0,          FALSE,  FALSE,  efParams                    },
    {"macValueGet",         0,          FALSE,  FALSE,  otherParams                 },
    {"optGet",              0,          FALSE,  FALSE,  otherParams                 },
    {"pvAssign",            0,          FALSE,  FALSE,  assignParams                },
    {"pvAssignCount",       0,          FALSE,  FALSE,  noParams                    },
    {"pvAssignSubst",       0,          FALSE,  FALSE,  assignParams                },
    {"pvAssigned",          0,          FALSE,  FALSE,  pvParams                    },
    {"pvChannelCount",      0,          FALSE,  FALSE,  noParams                    },
    {"pvConnectCount",      0,          FALSE,  FALSE,  noParams                    },
    {"pvConnected",         0,          FALSE,  FALSE,  pvParams                    },
    {"pvArrayConnected",    0,          FALSE,  FALSE,  pvArrayParams               },
    {"pvCount",             0,          FALSE,  FALSE,  pvParams                    },
    {"pvFlush",             0,          FALSE,  FALSE,  noParams                    },
    {"pvFlushQ",            0,          FALSE,  FALSE,  pvParams                    },
    {"pvFreeQ",             0,          FALSE,  FALSE,  pvParams                    },
    {"pvGet",               "pvGetTmo", FALSE,  FALSE,  pvGetPutParams              },
    {"pvGetCancel",         0,          FALSE,  FALSE,  pvParams                    },
    {"pvArrayGetCancel",    0,          FALSE,  FALSE,  pvArrayParams               },
    {"pvGetComplete",       0,          FALSE,  FALSE,  pvParams                    },
    {"pvArrayGetComplete",  0,          FALSE,  FALSE,  pvArrayGetPutCompleteParams },
    {"pvGetQ",              0,          FALSE,  FALSE,  pvParams                    },
    {"pvIndex",             0,          FALSE,  FALSE,  pvParams                    },
    {"pvMessage",           0,          FALSE,  FALSE,  pvParams                    },
    {"pvMonitor",           0,          FALSE,  FALSE,  pvParams                    },
    {"pvArrayMonitor",      0,          FALSE,  FALSE,  pvArrayParams               },
    {"pvName",              0,          FALSE,  FALSE,  pvParams                    },
    {"pvPut",               "pvPutTmo", FALSE,  FALSE,  pvGetPutParams              },
    {"pvPutCancel",         0,          FALSE,  FALSE,  pvParams                    },
    {"pvArrayPutCancel",    0,          FALSE,  FALSE,  pvArrayParams               },
    {"pvPutComplete",       0,          FALSE,  FALSE,  pvPutCompleteParams         },
    {"pvArrayPutComplete",  0,          FALSE,  FALSE,  pvArrayGetPutCompleteParams },
    {"pvSeverity",          0,          FALSE,  FALSE,  pvParams                    },
    {"pvStatus",            0,          FALSE,  FALSE,  pvParams                    },
    {"pvStopMonitor",       0,          FALSE,  FALSE,  pvParams                    },
    {"pvArrayStopMonitor",  0,          FALSE,  FALSE,  pvArrayParams               },
    {"pvSync",              0,          FALSE,  FALSE,  pvSyncParams                },
    {"pvArraySync",         0,          FALSE,  FALSE,  pvArraySyncParams           },
    {"pvTimeStamp",         0,          FALSE,  FALSE,  pvParams                    },
    {0,                     0,          FALSE,  FALSE,  0                           }
};

/* Insert builtin constants into symbol table */
void register_builtin_consts(SymTable sym_table)
{
    struct const_symbol *sym;

    for (sym = const_symbols; sym->name; sym++) {
        /* use address of const_symbols array as the symbol type */
        sym_table_insert(sym_table, sym->name, const_symbols, sym);
    }
}

/* Insert builtin functions into symbol table */
void register_builtin_funcs(SymTable sym_table)
{
    struct func_symbol *sym;

    for (sym = func_symbols; sym->name; sym++) {
        /* use address of func_symbols array as the symbol type */
        sym_table_insert(sym_table, sym->name, func_symbols, sym);
    }
}

/* Look up a builtin function from the symbol table */
struct func_symbol *lookup_builtin_func(SymTable sym_table, const char *func_name)
{
    /* use address of func_symbols array as the symbol type */
    return (struct func_symbol *)sym_table_lookup(sym_table, func_name, func_symbols);
}

/* Look up a builtin constant from the symbol table */
struct const_symbol *lookup_builtin_const(SymTable sym_table, const char *const_name)
{
    /* use address of const_symbols array as the symbol type */
    return (struct const_symbol *)sym_table_lookup(sym_table, const_name, const_symbols);
}
