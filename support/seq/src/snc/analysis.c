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
                Analysis of parse tree
\*************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>

#include "types.h"
#include "sym_table.h"
#include "main.h"
#include "node.h"
#include "builtin.h"
#include "gen_code.h"
#include "var_types.h"
#include "analysis.h"

static const int impossible = 0;

/* unique dummy pointer to distinguish struct
   names from other stuff in the symbol table */
void *structdefs = &structdefs;

static void analyse_definitions(Program *p);
static void analyse_option(Options *options, Node *defn);
static void analyse_state_option(StateOptions *options, Node *defn);
static void analyse_declaration(SymTable st, Node *scope, Node *defn);
static void analyse_assign(SymTable st, ChanList *chan_list, Node *scope, Node *defn);
static void analyse_monitor(SymTable st, Node *scope, Node *defn);
static void analyse_sync(SymTable st, Node *scope, Node *defn);
static void analyse_syncq(SymTable st, SyncQList *syncq_list, Node *scope, Node *defn);
static void assign_subscript(ChanList *chan_list, Node *defn, Var *vp, Node *subscr, Node *pv_name);
static void assign_single(ChanList *chan_list, Node *defn, Var *vp, Node *pv_name);
static void assign_multi(ChanList *chan_list, Node *defn, Var *vp, Node *pv_name_list);
static Chan *new_channel(ChanList *chan_list, Var *vp, uint count, uint index);
static SyncQ *new_sync_queue(SyncQList *syncq_list, uint size);
static void connect_variables(SymTable st, Node *scope);
static void connect_state_change_stmts(SymTable st, Node *scope);
static uint connect_states(SymTable st, Node *ss_list);
static void check_states_reachable_from_first(Node *ss);
static void add_var(Var *vp, Node *scope);
static Var *find_var(SymTable st, char *name, Node *scope);
static uint assign_ef_bits(Node *scope);

Program *analyse_program(Node *prog, Options options)
{
	Program *p = new(Program);
	Node *ss;

	assert(prog);	/* precondition */
#ifdef DEBUG
	report("-------------------- Analysis --------------------\n");
#endif

	p->options = options;
	p->prog = prog;

	p->name			= prog->token.str;
	if (prog->prog_param)
		p->param	= prog->prog_param->token.str;
	else
		p->param	= "";

	p->sym_table = sym_table_create();

	register_builtin_consts(p->sym_table);
	register_builtin_funcs(p->sym_table);

	p->chan_list = new(ChanList);
	p->syncq_list = new(SyncQList);

#ifdef DEBUG
	report("created symbol table, channel list, and syncq list\n");
#endif

	analyse_definitions(p);
	p->num_ss = connect_states(p->sym_table, prog);
	connect_variables(p->sym_table, prog);
	connect_state_change_stmts(p->sym_table, prog);
	foreach(ss, prog->prog_statesets)
		check_states_reachable_from_first(ss);
	p->num_event_flags = assign_ef_bits(p->prog);
	return p;
}

static void analyse_structdef(SymTable st, Node *defn)
{
	assert(defn->tag == D_STRUCTDEF);
	if (!sym_table_insert(st, defn->token.str, structdefs, 
		mk_structure_type(defn->token.str, defn->structdef_members)))
	{
		warning_at_node(defn, "ignoring duplicate struct declaration\n");
	}
}

static void analyse_funcdef(Node *defn)
{
	Node *decl;
	Var *var;
	Node *p;

	assert(defn->tag == D_FUNCDEF);
	decl = defn->funcdef_decl;
	assert(decl->tag == D_DECL);
	var = decl->extra.e_decl;
	if (var->type->tag != T_FUNCTION)
	{
		error_at_node(decl, "not a function type\n");
		return;
	}

	foreach(p, var->type->val.function.param_decls)
	{
		/* check parameter has a name */
		if (!p->extra.e_decl->name)
		{
			error_at_node(p, "function parameter must have a name\n");
		}
	}
}

static void analyse_defns(Node *defn_list, Node *scope, Program *p)
{
	Node *defn;

	assert(is_scope(scope));				/* precondition */
	foreach (defn, defn_list)
	{
		switch (defn->tag)
		{
		case D_OPTION:
			if (scope->tag == D_PROG)
			{
				analyse_option(&p->options, defn);
			}
			else if (scope->tag == D_STATE)
			{
				analyse_state_option(&scope->extra.e_state->options, defn);
			}
			break;
		case D_DECL:
			analyse_declaration(p->sym_table, scope, defn);
			break;
		case D_FUNCDEF:
			analyse_funcdef(defn);
			analyse_declaration(p->sym_table, scope, defn->funcdef_decl);
			break;
		case D_STRUCTDEF:
			analyse_structdef(p->sym_table, defn);
			break;
		case D_ASSIGN:
			analyse_assign(p->sym_table, p->chan_list, scope, defn);
			break;
		case D_MONITOR:
			analyse_monitor(p->sym_table, scope, defn);
			break;
		case D_SYNC:
			analyse_sync(p->sym_table, scope, defn);
			break;
		case D_SYNCQ:
			analyse_syncq(p->sym_table, p->syncq_list, scope, defn);
			break;
		case T_TEXT:
			break;
		default:
			assert(impossible);
		}
	}
}

static int analyse_scope(Node *scope, Node *parent_scope, void *parg)
{
	Program	*p = (Program *)parg;
	Node *defn_list;
	VarList **pvar_list;

	assert(scope);	/* precondition */

#ifdef DEBUG
	report("analyse_defn: scope=(%s:%s)\n",
		node_name(scope), scope->token.str);
#endif

	assert(is_scope(scope));				/* precondition */
	assert(!parent_scope || is_scope(parent_scope));	/* precondition */

	defn_list = defn_list_from_scope(scope);
	pvar_list = pvar_list_from_scope(scope);

	/* NOTE: We always need to allocate a var_list, even if there are no
	   definitions on this level, so later on (see connect_variables below)
	   we can traverse in the other direction to find the nearest enclosing
	   scope. */
	if (!*pvar_list)
	{
		*pvar_list = new(VarList);
		(*pvar_list)->parent_scope = parent_scope;
	}

	analyse_defns(defn_list, scope, p);
	if (scope->tag == D_PROG)
	{
		analyse_defns(scope->prog_xdefns, scope, p);
	}

	return TRUE; /* always descend into children */
}

VarList **pvar_list_from_scope(Node *scope)
{
	assert(scope);			/* precondition */
	assert(is_scope(scope));	/* precondition */

	switch(scope->tag)
	{
	case D_PROG:
		return &scope->extra.e_prog;
	case D_SS:
		assert(scope->extra.e_ss);	/* invariant */
		return &scope->extra.e_ss->var_list;
	case D_STATE:
		assert(scope->extra.e_state);	/* invariant */
		return &scope->extra.e_state->var_list;
	case S_CMPND:
		return &scope->extra.e_cmpnd;
	case D_FUNCDEF:
		return &scope->extra.e_funcdef;
	default:
		assert(impossible); return NULL;
	}
}

Node *defn_list_from_scope(Node *scope)
{
	assert(scope);			/* precondition */
	assert(is_scope(scope));	/* precondition */

	switch(scope->tag)
	{
	case D_PROG:
		return scope->prog_defns;
	case D_SS:
		return scope->ss_defns;
	case D_STATE:
		return scope->state_defns;
	case S_CMPND:
		return scope->cmpnd_defns;
	case D_FUNCDEF:
		return scope->funcdef_params;
	default:
		assert(impossible); return NULL;
	}
}

static void analyse_definitions(Program *p)
{
#ifdef DEBUG
	report("**begin** analyse definitions\n");
#endif

	traverse_syntax_tree(p->prog, scope_mask, ~has_sub_scope_mask, 0, analyse_scope, p);

#ifdef DEBUG
	report("**end** analyse definitions\n");
#endif
}

/* Options at the top-level. Note: latest given value for option wins. */
static void analyse_option(Options *options, Node *defn)
{
	char	*optname;
	uint	optval;

	assert(options);		/* precondition */
	assert(defn);			/* precondition */
	assert(defn->tag == D_OPTION);	/* precondition */

	optname = defn->token.str;
	optval = defn->extra.e_option;

	for (; *optname; optname++)
	{
		switch(*optname)
		{
		case 'a': options->async = optval; break;
		case 'c': options->conn = optval; break;
		case 'd': options->debug = optval; break;
		case 'e': options->newef = optval; break;
		case 'l': options->line = optval; break;
		case 'm': options->main = optval; break;
		case 'r': options->reent = optval; break;
		case 's': options->safe = optval; break;
		case 'w': options->warn = optval; break;
		case 'W': options->xwarn = optval; break;
		default: report_at_node(defn,
		  "warning: unknown option '%s'\n", optname);
		}
	}
	if (options->safe) {
		options->reent = TRUE;
	}
}

/* Options in state declarations. Note: latest given value for option wins. */
static void analyse_state_option(StateOptions *options, Node *defn)
{
	char	*optname;
	uint	optval;

	assert(options);		/* precondition */
	assert(defn);			/* precondition */
	assert(defn->tag == D_OPTION);	/* precondition */

	optname = defn->token.str;
	optval = defn->extra.e_option;

	for (; *optname; optname++)
	{
		switch(*optname)
		{
		case 't': options->do_reset_timers = optval; break;
		case 'e': options->no_entry_from_self = optval; break;
		case 'x': options->no_exit_to_self = optval; break;
		default: report_at_node(defn,
		  "warning: unknown state option '%s'\n", optname);
		}
	}
}

/* Traverse a type and replace foreign struct types with SNL struct
   type if one exists with the same name */
static void fixup_struct_refs(SymTable st, Type *t)
{
	Node *d;
	Type *r;

#ifdef DEBUG
	report("fixup_struct_refs():\n");
	dump_type(t, 1);
#endif
	switch (t->tag)
	{
	case T_NONE:
	case T_EVFLAG:
	case T_VOID:
	case T_PRIM:
		break;
	case T_FOREIGN:
		switch (t->val.foreign.tag)
		{
		case F_ENUM:
			break;
		case F_STRUCT:
			r = sym_table_lookup(st, t->val.foreign.name, structdefs);
			if (r)
			{
				assert(r->tag == T_STRUCT);
				t->tag = r->tag;
				t->val.structure.name = r->val.structure.name;
				t->val.structure.member_decls = r->val.structure.member_decls;
				/* NOTE: do *not* change parent! */
			}
			break;
		case F_UNION:
		case F_TYPENAME:
			break;
		}
		break;
	case T_POINTER:
		fixup_struct_refs(st, t->val.pointer.value_type);
		break;
	case T_ARRAY:
		fixup_struct_refs(st, t->val.array.elem_type);
		break;
	case T_FUNCTION:
		fixup_struct_refs(st, t->val.function.return_type);
		foreach (d, t->val.function.param_decls)
		{
			if (d->tag == D_DECL)
				fixup_struct_refs(st, d->extra.e_decl->type);
		}
		break;
#if 0
	case T_CONST:
		fixup_struct_refs(st, t->val.constant.value_type);
		break;
#endif
	case T_STRUCT:
		foreach (d, t->val.structure.member_decls)
		{
			if (d->tag == D_DECL)
			{
#ifdef DEBUG
				report("struct member %s.%s:\n", t->val.structure.name, d->extra.e_decl->name);
#endif
				fixup_struct_refs(st, d->extra.e_decl->type);
			}
		}
		break;
	}
}

static void analyse_declaration(SymTable st, Node *scope, Node *defn)
{
	Var *vp;
        VarList *var_list;
	static uint seen_foreign = FALSE;

	assert(scope);			/* precondition */
	assert(defn);			/* precondition */
	assert(defn->tag == D_DECL);	/* precondition */

	vp = defn->extra.e_decl;

	assert(vp);			/* invariant */
#ifdef DEBUG
	report("declaration: %s\n", vp->name);
#endif
	if (vp->type->tag == T_NONE && !seen_foreign)
	{
		warning_at_node(defn,
			"foreign declarations are deprecated\n");
		seen_foreign = TRUE;
	}
	if (scope->tag != D_PROG)
	{
		const char *things = 0;

		switch (vp->type->tag)
		{
		case T_NONE: things = "foreign objects"; break;
		case T_EVFLAG: things = "event flags"; break;
		case T_FUNCTION: things = "functions"; break;
		default: break;
		}
		if (things)
			error_at_node(defn,
				"%s can only be declared at the top-level\n", things);
	}
	if (vp->type->tag == T_EVFLAG)
	{
		vp->chan.evflag = new(EvFlag);
	}
#ifdef DEBUG
	report("name=%s, before fixup:\n", vp->name);
	dump_type(vp->type, 0);
#endif
	fixup_struct_refs(st, vp->type);
#ifdef DEBUG
	report("name=%s, after fixup:\n", vp->name);
	dump_type(vp->type, 0);
#endif

	var_list = var_list_from_scope(scope);

	assert(vp->name);

	if (!sym_table_insert(st, vp->name, var_list, vp))
	{
		Var *vp2 = (Var *)sym_table_lookup(st, vp->name, var_list);

		/* Note: functions can be declared more than once */
		/* We let the C compiler check that they are consistent */
		if (vp2->type->tag != T_FUNCTION)
		{
			if (vp2->decl)
				error_at_node(defn,
				 "variable '%s' already declared at %s:%d\n",
				 vp->name, vp2->decl->token.file, vp2->decl->token.line);
			else
				error_at_node(defn,
				 "variable '%s' already (implicitly) declared\n",
				 vp->name);
		}
	}
	else
	{
		add_var(vp, scope);
	}
}

static void analyse_assign(SymTable st, ChanList *chan_list, Node *scope, Node *defn)
{
	char *name;
	Var *vp;

	assert(chan_list);		/* precondition */
	assert(scope);			/* precondition */
	assert(defn);			/* precondition */
	assert(defn->tag == D_ASSIGN);	/* precondition */

	name = defn->token.str;
	vp = find_var(st, name, scope);

	if (!vp)
	{
		error_at_node(defn, "cannot assign variable '%s': "
			"variable was not declared\n", name);
		return;
	}
	assert(vp->type);		/* invariant */
	if (!type_assignable(vp->type))
	{
		error_at_node(defn, "cannot assign variable '%s': wrong type\n", name);
		return;
	}
	if (vp->scope != scope)
	{
		error_at_node(defn, "cannot assign variable '%s': "
			"assign must be in the same scope as declaration\n", name);
		return;
	}
	if (scope->tag == D_STATE)
	{
		warning_at_node(defn, "state local assign is deprecated\n");
	}
	if (defn->assign_subscr)
	{
		assign_subscript(chan_list, defn, vp, defn->assign_subscr, defn->assign_pvs);
	}
	else if (!defn->assign_pvs)
	{
		assign_single(chan_list, defn, vp, 0);
	}
	else if (defn->assign_pvs->tag == E_INIT)
	{
		assign_multi(chan_list, defn, vp, defn->assign_pvs->init_elems);
	}
	else
	{
		assign_single(chan_list, defn, vp, defn->assign_pvs);
	}
}

/* Assign a (whole) variable to a channel.
 * Format: assign <variable> to <string>;
 */
static void assign_single(
	ChanList	*chan_list,
	Node		*defn,
	Var		*vp,
	Node		*pv_name
)
{
	char *name = pv_name ? pv_name->token.str : "";

	assert(chan_list);
	assert(defn);
	assert(vp);

#ifdef DEBUG
	report("assign %s to %s;\n", vp->name, name);
#endif

	if (vp->assign != M_NONE)
	{
		error_at_node(defn, "variable '%s' already assigned\n", vp->name);
		return;
	}
	vp->assign = M_SINGLE;
	vp->chan.single = new_channel(
		chan_list, vp, type_array_length1(vp->type) * type_array_length2(vp->type), 0);
	vp->chan.single->name = name;
}

static void assign_elem(
	ChanList	*chan_list,
	Node		*defn,
	Var		*vp,
	uint		n_subscr,
	char		*pv_name
)
{
	assert(chan_list);				/* precondition */
	assert(defn);					/* precondition */
	assert(vp);					/* precondition */
	assert(n_subscr < type_array_length1(vp->type));/*precondition */
	assert(vp->assign != M_SINGLE);			/* precondition */

	if (vp->assign == M_NONE)
	{
		uint n;

		vp->assign = M_MULTI;
		vp->chan.multi = newArray(Chan*, type_array_length1(vp->type));
		for (n = 0; n < type_array_length1(vp->type); n++)
		{
			vp->chan.multi[n] = new_channel(
				chan_list, vp, type_array_length2(vp->type), n);
		}
	}
	assert(vp->assign == M_MULTI);
	if (vp->chan.multi[n_subscr]->name)
	{
		error_at_node(defn, "array element '%s[%d]' already assigned to '%s'\n",
			vp->name, n_subscr, vp->chan.multi[n_subscr]->name);
		return;
	}
	vp->chan.multi[n_subscr]->name = pv_name;
}

/* Assign an array element to a channel.
 * Format: assign <variable>[<subscr>] to <string>; */
static void assign_subscript(
	ChanList	*chan_list,
	Node		*defn,
	Var		*vp,
	Node		*subscr,
	Node		*pv_name
)
{
	uint n_subscr;

	assert(chan_list);			/* precondition */
	assert(defn);				/* precondition */
	assert(vp);				/* precondition */
	assert(subscr);				/* precondition */
	assert(subscr->tag == E_CONST);	/* syntax */
	assert(pv_name);			/* precondition */

#ifdef DEBUG
	report("assign %s[%s] to '%s';\n", vp->name, subscr->token.str, pv_name->token.str);
#endif

	if (vp->type->tag != T_ARRAY)	/* establish L3 */
	{
		error_at_node(defn, "variable '%s' is not an array\n", vp->name);
		return;
	}
	if (vp->assign == M_SINGLE)
	{
		error_at_node(defn, "variable '%s' already assigned\n", vp->name);
		return;
	}
	if (!strtoui(subscr->token.str, type_array_length1(vp->type), &n_subscr))
	{
		error_at_node(subscr, "subscript in '%s[%s]' out of range\n",
			vp->name, subscr->token.str);
		return;
	}
	assign_elem(chan_list, defn, vp, n_subscr, pv_name->token.str);
}

/* Assign an array variable to multiple channels.
 * Format: assign <variable> to { <string>, <string>, ... };
 */
static void assign_multi(
	ChanList	*chan_list,
	Node		*defn,
	Var		*vp,
	Node		*pv_name_list
)
{
	Node	*pv_name;
	uint	n_subscr = 0;

	assert(chan_list);
	assert(defn);
	assert(vp);

#ifdef DEBUG
	report("assign %s to {", vp->name);
#endif

	if (vp->type->tag != T_ARRAY)	/* establish L3 */
	{
		error_at_node(defn, "variable '%s' is not an array\n", vp->name);
		return;
	}
	if (vp->assign == M_SINGLE)
	{
		error_at_node(defn, "assign: variable '%s' already assigned\n", vp->name);
		return;
	}
	foreach (pv_name, pv_name_list)
	{
#ifdef DEBUG
		report("'%s'%s", pv_name->token.str, pv_name->next ? ", " : "};\n");
#endif
		if (n_subscr >= type_array_length1(vp->type))
		{
			warning_at_node(pv_name, "discarding excess PV names "
				"in multiple assign to variable '%s'\n", vp->name);
			break;
		}
		assign_elem(chan_list, defn, vp, n_subscr++, pv_name->token.str);
	}
	/* for the remaining array elements, assign to "" */
	while (n_subscr < type_array_length1(vp->type))
	{
		assign_elem(chan_list, defn, vp, n_subscr++, "");
	}
}

static void monitor_var(Node *defn, Var *vp)
{
	assert(defn);
	assert(vp);

#ifdef DEBUG
	report("monitor %s;", vp->name);
#endif

	if (vp->assign == M_NONE)		/* establish L2a */
	{
		error_at_node(defn, "cannot monitor variable '%s': not assigned\n", vp->name);
		return;
	}
	if (vp->monitor == M_SINGLE)
	{
		warning_at_node(defn,
			"variable '%s' already monitored\n", vp->name);
		return;				/* nothing to do */
	}
	vp->monitor = M_SINGLE;			/* strengthen if M_MULTI */
	if (vp->assign == M_SINGLE)
	{
		vp->chan.single->monitor = TRUE;
	}
	else
	{
		uint n;
		assert(vp->assign == M_MULTI);	/* by L2a and else */
		for (n = 0; n < type_array_length1(vp->type); n++)
		{
			vp->chan.multi[n]->monitor = TRUE;
		}
	}
}

static void monitor_elem(Node *defn, Var *vp, Node *subscr)
{
	uint	n_subscr;

	assert(defn);
	assert(vp);
	assert(subscr);
	assert(subscr->tag == E_CONST);		/* syntax */

#ifdef DEBUG
	report("monitor %s[%s];\n", vp->name, subscr->token.str);
#endif

	if (vp->type->tag != T_ARRAY)	/* establish L3 */
	{
		error_at_node(defn, "variable '%s' is not an array\n", vp->name);
		return;
	}
	if (!strtoui(subscr->token.str, type_array_length1(vp->type), &n_subscr))
	{
		error_at_node(subscr, "subscript in '%s[%s]' out of range\n",
			vp->name, subscr->token.str);
		return;
	}
	if (vp->assign == M_NONE)	/* establish L2a */
	{
		error_at_node(defn, "array element '%s[%d]' not assigned\n",
			vp->name, n_subscr);
		return;
	}
	if (vp->monitor == M_SINGLE)
	{
		warning_at_node(defn, "array element '%s[%d]' already monitored\n",
			vp->name, n_subscr);
		return;		/* nothing to do */
	}
	if (vp->assign == M_SINGLE)	/* establish L1a */
	{
		error_at_node(defn, "variable '%s' is assigned to a single channel "
			"and can only be monitored wholesale\n", vp->name);
		return;
	}
	assert(vp->assign == M_MULTI);	/* by L1a and L2a */
	if (!vp->chan.multi[n_subscr]->name)
	{
		error_at_node(defn, "array element '%s[%d]' not assigned\n",
			vp->name, n_subscr);
		return;
	}
	if (vp->chan.multi[n_subscr]->monitor)
	{
		warning_at_node(defn, "'%s[%d]' already monitored\n",
			vp->name, n_subscr);
		return;					/* nothing to do */
	}
	vp->chan.multi[n_subscr]->monitor = TRUE;	/* do it */
}

static void analyse_monitor(SymTable st, Node *scope, Node *defn)
{
	Var	*vp;
	char	*var_name;

	assert(scope);
	assert(defn);
	assert(defn->tag == D_MONITOR);

	var_name = defn->token.str;
	assert(var_name);

	vp = find_var(st, var_name, scope);
	if (!vp)
	{
		error_at_node(defn,
			"cannot monitor variable '%s': not declared\n", var_name);
		return;
	}
	if (vp->scope != scope)
	{
		error_at_node(defn, "cannot monitor variable '%s': "
			"monitor must be in the same scope as declaration\n", var_name);
		return;
	}
	if (scope->tag == D_STATE)
	{
		warning_at_node(defn, "state local monitor is deprecated\n");
	}
	if (defn->monitor_subscr)
	{
		monitor_elem(defn, vp, defn->monitor_subscr);
	}
	else
	{
		monitor_var(defn, vp);
	}
}

static void sync_var(Node *defn, Var *vp, Var *evp)
{
	assert(defn);
	assert(vp);
	assert(evp);
	assert(vp->sync != M_SINGLE);			/* call */

#ifdef DEBUG
	report("sync %s to %s;\n", vp->name, evp->name);
#endif

	if (vp->sync == M_MULTI)
	{
		error_at_node(defn, "some array elements of '%s' already sync'd\n",
			vp->name);
		return;
	}
	if (vp->assign == M_NONE)		/* establish L2b */
	{
		error_at_node(defn, "variable '%s' not assigned\n", vp->name);
		return;
	}
	vp->sync = M_SINGLE;
	if (vp->assign == M_SINGLE)
	{
		assert(vp->chan.single);
		assert(vp->sync != M_MULTI);	/* by L1b */
		vp->chan.single->sync = evp;
		vp->sync = M_SINGLE;
	}
	else
	{
		uint n;
		assert(vp->assign == M_MULTI);	/* else */
		vp->sync = M_SINGLE;
		for (n = 0; n < type_array_length1(vp->type); n++)
		{
			assert(!vp->chan.multi[n]->sync);
			vp->chan.multi[n]->sync = evp;
		}
	}
}

static void sync_elem(Node *defn, Var *vp, Node *subscr, Var *evp)
{
	uint	n_subscr;

	assert(defn);					/* syntax */
	assert(vp);					/* call */
	assert(subscr);					/* call */
	assert(subscr->tag == E_CONST);		/* syntax */
	assert(evp);					/* syntax */

	assert(vp->sync != M_SINGLE);			/* call */

#ifdef DEBUG
	report("sync %s[%s] to %s;\n", vp->name, subscr->token.str, evp->name);
#endif

	if (vp->type->tag != T_ARRAY)	/* establish L3 */
	{
		error_at_node(defn, "variable '%s' is not an array\n", vp->name);
		return;
	}
	if (!strtoui(subscr->token.str, type_array_length1(vp->type), &n_subscr))
	{
		error_at_node(subscr, "subscript in '%s[%s]' out of range\n",
			vp->name, subscr->token.str);
		return;
	}
	/* establish L1b */
	if (vp->assign == M_SINGLE)
	{
		error_at_node(defn, "variable '%s' is assigned to a single channel "
			"and can only be sync'd wholesale\n", vp->name);
		return;
	}
	if (vp->assign == M_NONE || !vp->chan.multi[n_subscr]->name)
	{
		error_at_node(defn, "array element '%s[%d]' not assigned\n",
			vp->name, n_subscr);
		return;
	}
	assert(vp->assign == M_MULTI);	/* L1b */
	if (vp->chan.multi[n_subscr]->sync)
	{
		warning_at_node(defn, "'%s[%d]' already sync'd\n",
			vp->name, n_subscr);
		return;					/* nothing to do */
	}
	vp->sync = M_MULTI;
	vp->chan.multi[n_subscr]->sync = evp;		/* do it */
}

static void analyse_sync(SymTable st, Node *scope, Node *defn)
{
	char	*var_name, *ef_name;
	Var	*vp, *evp;

	assert(scope);
	assert(defn);
	assert(defn->tag == D_SYNC);

	var_name = defn->token.str;
	assert(var_name);

	assert(defn->sync_evflag);
	ef_name = defn->sync_evflag->token.str;
	assert(ef_name);

	vp = find_var(st, var_name, scope);
	if (!vp)
	{
		error_at_node(defn, "variable '%s' not declared\n", var_name);
		return;
	}
	if (vp->scope != scope)
	{
		error_at_node(defn, "cannot sync variable '%s' to event flag '%s': "
			"sync must be in the same scope as (variable) declaration\n",
			var_name, ef_name);
		return;
	}
	if (scope->tag == D_STATE)
	{
		warning_at_node(defn, "state local sync is deprecated\n");
	}
	if (vp->sync == M_SINGLE)
	{
		error_at_node(defn, "variable '%s' already sync'd\n", vp->name);
		return;
	}
	evp = find_var(st, ef_name, scope);
	if (!evp)
	{
		error_at_node(defn, "event flag '%s' not declared\n", ef_name);
		return;
	}
	if (evp->type->tag != T_EVFLAG)
	{
		error_at_node(defn, "variable '%s' is not an event flag\n", ef_name);
		return;
	}
	if (defn->sync_subscr)
	{
		sync_elem(defn, vp, defn->sync_subscr, evp);
	}
	else
	{
		sync_var(defn, vp, evp);
	}
}

static void syncq_var(Node *defn, Var *vp, SyncQ *qp)
{
	assert(defn);
	assert(vp);
	assert(qp);				/* call */
	assert(vp->syncq != M_SINGLE);		/* call */

	if (vp->syncq == M_MULTI)
	{
		error_at_node(defn, "some array elements of '%s' already syncq'd\n",
			vp->name);
		return;
	}
	if (vp->assign == M_NONE)		/* establish L2c */
	{
		error_at_node(defn, "variable '%s' not assigned\n", vp->name);
		return;
	}
	vp->syncq = M_SINGLE;
	if (vp->assign == M_SINGLE)
	{
		assert(vp->chan.single);	/* invariant */
		assert(vp->syncq != M_MULTI);	/* by L1c */
		vp->chan.single->syncq = qp;
		vp->syncq = M_SINGLE;
	}
	else
	{
		uint n;
		assert(vp->assign == M_MULTI);	/* else */
		vp->syncq = M_SINGLE;
		for (n = 0; n < type_array_length1(vp->type); n++)
		{
			assert(!vp->chan.multi[n]->syncq);
			vp->chan.multi[n]->syncq = qp;
		}
	}
}

static void syncq_elem(Node *defn, Var *vp, Node *subscr, SyncQ *qp)
{
	uint	n_subscr;

	assert(defn);					/* syntax */
	assert(vp);					/* call */
	assert(subscr);					/* call */
	assert(subscr->tag == E_CONST);		/* syntax */
	assert(qp);					/* call */

	assert(vp->syncq != M_SINGLE);			/* call */

	if (vp->type->tag != T_ARRAY)	/* establish L3 */
	{
		error_at_node(defn, "variable '%s' is not an array\n", vp->name);
		return;
	}
	if (!strtoui(subscr->token.str, type_array_length1(vp->type), &n_subscr))
	{
		error_at_node(subscr, "subscript in '%s[%s]' out of range\n",
			vp->name, subscr->token.str);
		return;
	}
	/* establish L1c */
	if (vp->assign == M_SINGLE)
	{
		error_at_node(defn, "variable '%s' is assigned to a single channel "
			"and can only be syncq'd wholesale\n", vp->name);
		return;
	}
	if (vp->assign == M_NONE || !vp->chan.multi[n_subscr]->name)
	{
		error_at_node(defn, "array element '%s[%d]' not assigned\n",
			vp->name, n_subscr);
		return;
	}
	assert(vp->assign == M_MULTI);			/* L1c */
	if (vp->chan.multi[n_subscr]->syncq)
	{
		warning_at_node(defn, "'%s[%d]' already syncq'd\n",
			vp->name, n_subscr);
		return;					/* nothing to do */
	}
	vp->syncq = M_MULTI;
	vp->chan.multi[n_subscr]->syncq = qp;		/* do it */
}

static void analyse_syncq(SymTable st, SyncQList *syncq_list, Node *scope, Node *defn)
{
	char	*var_name;
	Var	*vp, *evp = 0;
	SyncQ	*qp;
	uint	n_size = 0;

	assert(scope);
	assert(defn);
	assert(defn->tag == D_SYNCQ);

	var_name = defn->token.str;
	assert(var_name);

	vp = find_var(st, var_name, scope);
	if (!vp)
	{
		error_at_node(defn, "variable '%s' not declared\n", var_name);
		return;
	}
	if (vp->scope != scope)
	{
		error_at_node(defn, "cannot syncq variable '%s': "
			"syncq must be in the same scope as declaration\n",
			var_name);
		return;
	}
	if (scope->tag == D_STATE)
	{
		warning_at_node(defn, "state local syncq is deprecated\n");
	}
	if (vp->syncq == M_SINGLE)
	{
		error_at_node(defn, "variable '%s' already syncq'd\n", vp->name);
		return;
	}
	if (!defn->syncq_size)
	{
		warning_at_node(defn, "leaving out the queue size is deprecated"
			", queue size defaults to 100 elements\n");
	}
	else if (!strtoui(defn->syncq_size->token.str, UINT_MAX, &n_size) || n_size < 1)
	{
		error_at_node(defn->syncq_size, "queue size '%s' out of range\n",
			defn->syncq_size->token.str);
		return;
	}
	if (defn->syncq_evflag)
	{
		char *ef_name = defn->syncq_evflag->token.str;
		assert(ef_name);

		evp = find_var(st, ef_name, scope);
		if (!evp)
		{
			error_at_node(defn, "event flag '%s' not declared\n", ef_name);
			return;
		}
		if (evp->type->tag != T_EVFLAG)
		{
			error_at_node(defn, "variable '%s' is not an event flag\n", ef_name);
			return;
		}
		if (evp->chan.evflag->queued)
		{
			error_at_node(defn, "event flag '%s' is already used for another syncq\n",
				ef_name);
			return;
		}
		evp->chan.evflag->queued = TRUE;
	}
	qp = new_sync_queue(syncq_list, n_size);
	if (defn->syncq_subscr)
	{
		if (evp)
			sync_elem(defn, vp, defn->sync_subscr, evp);
		syncq_elem(defn, vp, defn->syncq_subscr, qp);
	}
	else
	{
		if (evp)
			sync_var(defn, vp, evp);
		syncq_var(defn, vp, qp);
	}
}

/* Allocate a channel structure for this variable, add it to the channel list,
   and initialize members index, var, and count. Also increase channel
   count in the list. */
static Chan *new_channel(ChanList *chan_list, Var *vp, uint count, uint index)
{
	Chan *cp = new(Chan);

	cp->var = vp;
	cp->count = count;
	cp->index = index;
	if (index == 0)
		vp->index = chan_list->num_elems;
	chan_list->num_elems++;
	/* add new channel to chan_list */
	if (!chan_list->first)
		chan_list->first = cp;
	else
		chan_list->last->next = cp;
	chan_list->last = cp;
	cp->next = 0;
	return cp;
}

/* Allocate a sync queue structure, add it to the sync queue list,
   and initialize members index, var, and size. Also increase sync queue
   count in the list. */
static SyncQ *new_sync_queue(SyncQList *syncq_list, uint size)
{
	SyncQ *qp = new(SyncQ);

	qp->index = syncq_list->num_elems++;
	qp->size = size;

	/* add new syncqnel to syncq_list */
	if (!syncq_list->first)
		syncq_list->first = qp;
	else
		syncq_list->last->next = qp;
	syncq_list->last = qp;
	qp->next = 0;

	return qp;
}

/* Add a variable to a scope (append to the end of the var_list) */
void add_var(Var *vp, Node *scope)
{
	VarList	*var_list = var_list_from_scope(scope);

	if (!var_list->first)
		var_list->first = vp;
	else
		var_list->last->next = vp;
	var_list->last = vp;
	vp->next = 0;

	vp->scope = scope;
}

/* Find a variable by name, given a scope; first searches the given
   scope, then the parent scope, and so on. Returns a pointer to the
   var struct or 0 if the variable is not found. */
Var *find_var(SymTable st, char *name, Node *scope)
{
	VarList *var_list = var_list_from_scope(scope);
	Var	*vp;

#ifdef DEBUG
	report("searching %s in %s:%s, ", name, scope->token.str,
		node_name(scope));
#endif
	vp = (Var *)sym_table_lookup(st, name, var_list);
	if (vp)
	{
#ifdef DEBUG
		report("found\n");
#endif
		return vp;
	}
	else if (!var_list->parent_scope)
	{
#ifdef DEBUG
		report("not found\n");
#endif
		return 0;
	}
	else
		return find_var(st, name, var_list->parent_scope);
}

/* Connect a variable reference (tag E_VAR) to the Var structure.
   If there is no such structure, e.g. because the variable has not been
   declared, then allocate one, assign type T_NONE, and assign the
   top-level scope for the variable. */
static int connect_variable(Node *ep, Node *scope, void *parg)
{
	SymTable	st = *(SymTable *)parg;
	Var		*vp;

	assert(ep);
	assert(ep->tag == E_VAR);
	assert(scope);

#ifdef DEBUG
	report("connect_variable: %s, line %d\n", ep->token.str, ep->token.line);
#endif

	vp = find_var(st, ep->token.str, scope);

#ifdef DEBUG
	if (vp)
	{
		report_at_node(ep, "var %s found in scope (%s:%s)\n", ep->token.str,
			node_name(vp->scope),
			vp->scope->token.str);
	}
	else
		report_at_node(ep, "var %s not found\n", ep->token.str);
#endif
	if (!vp)
	{
		VarList *var_list = var_list_from_scope(scope);
		struct const_symbol *csym;
		struct func_symbol *fsym;

		csym = lookup_builtin_const(st, ep->token.str);
		if (csym)
		{
			ep->tag = E_CONST;
			ep->extra.e_const = csym;
			return FALSE;
		}
		fsym = lookup_builtin_func(st, ep->token.str);
		if (fsym)
		{
			ep->tag = E_BUILTIN;
			ep->extra.e_builtin = fsym;
			return FALSE;
		}

		extra_warning_at_node(ep, "treating undeclared object '%s' as foreign\n",
			ep->token.str);
		/* create a pseudo declaration so we can finish the analysis phase */
		vp = new(Var);
		vp->name = ep->token.str;
                vp->type = new(Type);
		vp->type->tag = T_NONE;	/* undeclared type */
		/* add this variable to the top-level scope, NOT the current scope */
		while (var_list->parent_scope) {
			scope = var_list->parent_scope;
			var_list = var_list_from_scope(scope);
		}
		sym_table_insert(st, vp->name, var_list, vp);
		add_var(vp, scope);
	}
	ep->extra.e_var = vp;		/* make connection */
	return FALSE;			/* there are no children anyway */
}

static void connect_variables(SymTable st, Node *scope)
{
#ifdef DEBUG
	report("**begin** connect_variables\n");
#endif
	traverse_syntax_tree(scope, bit(E_VAR), 0, 0, connect_variable, &st);
#ifdef DEBUG
	report("**end** connect_variables\n");
#endif
}

void traverse_syntax_tree(
	Node		*ep,		/* start node */
	TypeMask	call_mask,	/* when to call iteratee */
	TypeMask	stop_mask,	/* when to stop descending */
	Node		*scope,		/* current scope, 0 at top-level */
	node_iter	*iteratee,	/* function to call */
	void		*parg		/* argument to pass to function */
)
{
	Node	*cep;
	uint	i;
	int	descend = TRUE;

	if (!ep)
		return;

#ifdef DEBUG
	report("traverse_syntax_tree(tag=%s,token.str=%s)\n",
		node_name(ep), ep->token.str);
#endif

	/* Call the function? */
	if (call_mask & bit(ep->tag))
	{
		descend = iteratee(ep, scope, parg);
	}

	if (!descend)
		return;

	/* Are we just entering a new scope? */
	if (is_scope(ep))
	{
#ifdef DEBUG
	report("traverse_syntax_tree: new scope=(%s,%s)\n",
		node_name(ep), ep->token.str);
#endif
		scope = ep;
	}

	/* Descend into children */
	for (i = 0; i < node_info[ep->tag].num_children; i++)
	{
		foreach (cep, ep->children[i])
		{
			if (!(bit(cep->tag) & stop_mask))
			{
				traverse_syntax_tree(cep, call_mask, stop_mask,
					scope, iteratee, parg);
			}
		}
	}
}

/* Check for duplicate state set and state names and resolve transitions between states */
static uint connect_states(SymTable st, Node *prog)
{
	Node	*ssp;
	uint	num_ss = 0;

	foreach (ssp, prog->prog_statesets)
	{
		Node *sp;
		uint num_states = 0;

#ifdef DEBUG
		report("connect_states: ss = %s\n", ssp->token.str);
#endif
		if (!sym_table_insert(st, ssp->token.str, prog, ssp))
		{
			Node *ssp2 = (Node *)sym_table_lookup(st, ssp->token.str, prog);
			error_at_node(ssp,
				"a state set with name '%s' was already "
				"declared at line %d\n", ssp->token.str, ssp2->token.line);
		}
		foreach (sp, ssp->ss_states)
		{
			if (!sym_table_insert(st, sp->token.str, ssp, sp))
			{
				Node *sp2 = (Node *)sym_table_lookup(st, sp->token.str, ssp);
				error_at_node(sp,
					"a state with name '%s' in state set '%s' "
					"was already declared at line %d\n",
					sp->token.str, ssp->token.str, sp2->token.line);
			}
			assert(sp->extra.e_state);
#ifdef DEBUG
			report("connect_states: ss = %s, state = %s, index = %d\n",
				ssp->token.str, sp->token.str, num_states);
#endif
			sp->extra.e_state->index = num_states++;
		}
		ssp->extra.e_ss->num_states = num_states;
#ifdef DEBUG
		report("connect_states: ss = %s, num_states = %d\n", ssp->token.str, num_states);
#endif
		foreach (sp, ssp->ss_states)
		{
			Node *tp;

			foreach (tp, sp->state_whens)
			{
				Node *next_sp = 0;

				if (tp->token.str)
				{
					next_sp = (Node *)sym_table_lookup(st, tp->token.str, ssp);
					if (!next_sp)
					{
						error_at_node(tp,
							"a state with name '%s' does not "
							"exist in state set '%s'\n",
					 		tp->token.str, ssp->token.str);
					}
				}
				tp->extra.e_when->next_state = next_sp;
				assert(!next_sp || strcmp(tp->token.str,next_sp->token.str) == 0);
#ifdef DEBUG
				report("connect_states: ss = %s, state = %s, when(...){...} state (%s,%d)\n",
					ssp->token.str, sp->token.str, tp->token.str, next_sp?next_sp->extra.e_state->index:0);
#endif
			}
		}
		num_ss++;
	}
	return num_ss;
}

typedef struct {
	SymTable	st;
	Node		*ssp;
	int		in_when;
} connect_state_change_arg;

static int iter_connect_state_change_stmts(Node *ep, Node *scope, void *parg)
{
	connect_state_change_arg *pcsc_arg = (connect_state_change_arg *)parg;

	assert(pcsc_arg);
	assert(ep);
	if (ep->tag == D_SS)
	{
		pcsc_arg->ssp = ep;
		return TRUE;
	}
	else if (ep->tag == D_ENTEX)
	{
		/* to flag erroneous state change statements, see below */
		pcsc_arg->in_when = FALSE;
		return TRUE;
	}
	else if (ep->tag == D_WHEN)
	{
		pcsc_arg->in_when = TRUE;
		return TRUE;
	}
	else
	{
		assert(ep->tag == S_CHANGE);
		if (!pcsc_arg->ssp || !pcsc_arg->in_when)
		{
			error_at_node(ep, "state change statement not allowed here\n");
		}
		else
		{
			Node *sp = (Node *)sym_table_lookup(
				pcsc_arg->st, ep->token.str, pcsc_arg->ssp);
			if (!sp)
			{
				error_at_node(ep,
					"a state with name '%s' does not "
					"exist in state set '%s'\n",
				 	ep->token.str, pcsc_arg->ssp->token.str);
				return FALSE;
			}
			ep->extra.e_change = sp;
		}
		return FALSE;
	}
}

static void connect_state_change_stmts(SymTable st, Node *scope)
{
	connect_state_change_arg csc_arg;

	csc_arg.st = st;
	csc_arg.ssp = 0;
	csc_arg.in_when = FALSE;
	traverse_syntax_tree(scope,
		bit(S_CHANGE)|bit(D_SS)|bit(D_ENTEX)|bit(D_WHEN),
		expr_mask, 0, iter_connect_state_change_stmts, &csc_arg);
}

static void mark_states_reachable_from(Node *sp);

static int iter_mark_states_reachable(Node *ep, Node *scope, void *parg)
{
	Node *target_state = 0;

	switch (ep->tag ) {
	case S_CHANGE:
		target_state = ep->extra.e_change;
		break;
	case D_WHEN:
		target_state = ep->extra.e_when->next_state;
		break;
	default:
		assert(impossible);
	}
	if (target_state && !target_state->extra.e_state->is_target)
	{
		target_state->extra.e_state->is_target = 1;
		mark_states_reachable_from(target_state);
	}
	return (ep->tag == D_WHEN);
}

static void mark_states_reachable_from(Node *sp)
{
	assert(sp);
	assert(sp->tag == D_STATE);

	traverse_syntax_tree(
		sp,				/* start node */
		bit(S_CHANGE)|bit(D_WHEN),	/* when to call iteratee */
		expr_mask,			/* when to stop descending */
		sp,				/* current scope, 0 at top-level */
		iter_mark_states_reachable,	/* function to call */
		0				/* argument to pass to function */
	);
}

static void check_states_reachable_from_first(Node *ssp)
{
	Node *sp;

	assert(ssp);
	assert(ssp->tag == D_SS);

	sp = ssp->ss_states;
	assert(sp);
	assert(sp->tag == D_STATE);
	assert(sp->extra.e_state->index == 0);

	sp->extra.e_state->is_target = 1;
	mark_states_reachable_from(sp);

	foreach (sp, ssp->ss_states)
	{
		if (!sp->extra.e_state->is_target)
		{
			warning_at_node(sp, "state '%s' in state set '%s' cannot "
				"be reached from start state\n",
				sp->token.str, ssp->token.str);
		}
	}
}

/* Assign event bits to event flags and associate pv channels with
 * event flags. Return number of event flags found.
 */
static uint assign_ef_bits(Node *scope)
{
	Var	*vp;
	uint	num_event_flags = 0;
	VarList	*var_list;

	var_list = var_list_from_scope(scope);

	/* Assign event flag numbers (starting at 1) */
	foreach (vp, var_list->first)
	{
		if (vp->type->tag == T_EVFLAG)
		{
			assert(vp->chan.evflag);
			vp->chan.evflag->index = ++num_event_flags;
		}
	}
	return num_event_flags;
}
