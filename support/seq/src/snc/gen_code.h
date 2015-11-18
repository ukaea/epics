/*************************************************************************\
Copyright (c) 1990      The Regents of the University of California
                        and the University of Chicago.
                        Los Alamos National Laboratory
Copyright (c) 2010-2015 Helmholtz-Zentrum Berlin f. Materialien
                        und Energie GmbH, Germany (HZB)
This file is distributed subject to a Software License Agreement found
in the file LICENSE that is included with this distribution.
\*************************************************************************/
/*************************************************************************\
                Code generation
\*************************************************************************/
#ifndef INCLgencodeh
#define INCLgencodeh

#include "types.h"

void generate_code(Program *p);
void gen_defn_c_code(Node *scope, int level);
void gen_var_decl(Var *vp);
void indent(int level);

/* names and name prefixes for generated structs */
#define NM_VARS		"seqg_vars"
#define NM_CHANS	"seqg_chans"
#define NM_STATES	"seqg_states"
#define NM_STATESETS	"seqg_statesets"

/* names and name prefixes for generated functions */
#define NM_ENTRY	"seqg_entry"
#define NM_EXIT		"seqg_exit"
#define NM_INIT		"seqg_init"
#define NM_ACTION	"seqg_action"
#define NM_EVENT	"seqg_event"
#define NM_MASK		"seqg_mask"

/* names of generated function arguments */
#define NM_VAR		"seqg_var"
#define NM_ENV		"seqg_env"
#define NM_TRN		"seqg_trn"
#define NM_PTRN		"seqg_ptrn"
#define NM_PNST		"seqg_pnst"

/* prefix for generated inititialization variable names */
#define NM_INITVAR	"seqg_initvar_"

#define NM_DUMMY	"seqg_dummy"

#endif	/*INCLgencodeh*/
