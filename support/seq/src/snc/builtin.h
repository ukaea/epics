/*************************************************************************\
Copyright (c) 2010-2015 Helmholtz-Zentrum Berlin f. Materialien
                        und Energie GmbH, Germany (HZB)
This file is distributed subject to a Software License Agreement found
in the file LICENSE that is included with this distribution.
\*************************************************************************/
#ifndef INCLbuiltinh
#define INCLbuiltinh

#include "sym_table.h"

struct const_symbol {
    const char  *name;
    int         type;
};

enum const_type {
    CT_EVFLAG,
    CT_OTHER
};

enum param_type {
    PT_EF,
    PT_PV,
    PT_PV_ARRAY,
    PT_OTHER
};

struct param {
    enum param_type type;
    const char *default_value;
};

struct func_symbol {
    const char *name;           /* SNL name */
    const char *c_name;         /* C name, or 0 if same as SNL name */
    uint action_only:1;         /* not allowed in when-conditions */
    uint cond_only:1;           /* only allowed in when-conditions */
    const struct param **params;/* parameter descriptions */
};

/* Insert builtin constants into symbol table */
void register_builtin_consts(SymTable sym_table);

/* Insert builtin functions into symbol table */
void register_builtin_funcs(SymTable sym_table);

/* Look up a builtin function from the symbol table */
struct func_symbol *lookup_builtin_func(SymTable sym_table, const char *func_name);

/* Look up a builtin constant from the symbol table */
struct const_symbol *lookup_builtin_const(SymTable sym_table, const char *const_name);

#endif /*INCLbuiltinh */
