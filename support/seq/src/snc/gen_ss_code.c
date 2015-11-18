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
                State set code generation
\*************************************************************************/
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "node.h"
#include "analysis.h"
#include "gen_code.h"
#include "main.h"
#include "builtin.h"
#include "gen_ss_code.h"
#include "type_check.h"

static const int impossible = 0;

static void gen_local_var_decls(Node *scope, int context, int level);
static void gen_state_func(
	const char *ss_name,
	uint ss_num,
	const char *state_name,
	Node *xp,
	void (*gen_body)(Node *xp, int context),
	int context,
	const char *title,
	const char *prefix,
	const char *rettype,
	const char *extra_args
);
static void gen_entex_body(Node *xp, int context);
static void gen_event_body(Node *xp, int context);
static void gen_action_body(Node *xp, int context);
static void gen_expr(int context, Node *ep, int level);
static void gen_builtin_call(int context, Node *ep);
static void gen_ef_arg(
	int		context,
	const char	*func_name,	/* function name */
	Node		*ap,		/* argument expression */
	uint		index);		/* argument index */
static void gen_pv_arg(
	int		context,
	const char	*func_name,	/* function name */
	Node		*ap,		/* argument expression */
	uint		index,		/* argument index */
	uint		pv_array);	/* function expects a pv array */

static void gen_prog_func(
	Node *prog,
	const char *doc,
	const char *name,
	void (*gen_body)(Node *prog)
);
static void gen_prog_entex_func(
	Node *prog,
	const char *doc,
	const char *name,
	void (*gen_body)(Node *)
);
static void gen_prog_init_body(Node *prog);
static void gen_prog_entry_body(Node *prog);
static void gen_prog_exit_body(Node *prog);

/*
 * Expression context. Certain nodes of the syntax tree are
 * interpreted differently depending on the context in which
 * they appear. For instance, the state change command is only
 * allowed in transition action context (C_TRANS).
 */
enum context
{
	C_COND,		/* when() condition */
	C_TRANS,	/* state transition actions */
	C_SS,		/* otherwise inside a state set */
	C_FUNC,		/* inside a function definition */
	C_GLOBAL	/* outside state sets and functions */
};

/*
 * HACK: use global variable to make reentrant option available
 * to gen_var_access below. Otherwise we'd have to pass it through
 * almost every subroutine in this module.
 */
static Options global_options;

/* Generate state set C code from analysed syntax tree */
void gen_ss_code(Node *prog, Options options)
{
	Node	*sp, *ssp;
	uint	ss_num = 0;

	/* HACK: intialise global variable as implicit parameter */
	global_options = options;

	gen_code("\n#define " NM_VAR " (*(struct " NM_VARS " *const *)" NM_ENV ")\n");

	/* Generate program init func */
	gen_prog_func(prog, "init", NM_INIT, gen_prog_init_body);

	/* Generate program entry func */
	if (prog->prog_entry)
		gen_prog_entex_func(prog, "entry", NM_ENTRY, gen_prog_entry_body);

	/* For each state set ... */
	foreach (ssp, prog->prog_statesets)
	{
		/* For each state ... */
		foreach (sp, ssp->ss_states)
		{
			gen_code("\n/****** Code for state \"%s\" in state set \"%s\" ******/\n",
				sp->token.str, ssp->token.str);

			/* Generate entry and exit functions */
			if (sp->state_entry)
				gen_state_func(ssp->token.str, ss_num, sp->token.str, 
					sp->state_entry, gen_entex_body,
					C_SS, "Entry", NM_ENTRY, "void", "");
			if (sp->state_exit)
				gen_state_func(ssp->token.str, ss_num, sp->token.str,
					sp->state_exit, gen_entex_body,
					C_SS, "Exit", NM_EXIT, "void", "");
			/* Generate event processing function */
			gen_state_func(ssp->token.str, ss_num, sp->token.str,
				sp->state_whens, gen_event_body,
				C_SS, "Event", NM_EVENT, "seqBool",
				", int *"NM_PTRN", int *"NM_PNST);
			/* Generate action processing function */
			gen_state_func(ssp->token.str, ss_num, sp->token.str,
				sp->state_whens, gen_action_body,
				C_TRANS, "Action", NM_ACTION, "void",
				", int "NM_TRN", int *"NM_PNST);
		}
		ss_num++;
	}

	/* Generate program exit func */
	if (prog->prog_exit)
		gen_prog_entex_func(prog, "exit", NM_EXIT, gen_prog_exit_body);

	gen_code("\n#undef " NM_VAR "\n");
}

/* Generate a local C variable declaration for each variable declared
   inside the body of an entry, exit, when, or compound statement block. */
static void gen_local_var_decls(Node *scope, int context, int level)
{
	Var	*vp;
	VarList	*var_list;

	var_list = var_list_from_scope(scope);

	/* Convert internal type to `C' type */
	foreach (vp, var_list->first)
	{
		assert(vp->type->tag != T_NONE);
		assert(vp->type->tag != T_EVFLAG);
		assert(vp->decl);

		gen_line_marker(vp->decl);
		indent(level);
		gen_var_decl(vp);

		/* optional initialisation */
		if (vp->decl->decl_init)
		{
			gen_code(" = ");
			gen_expr(context, vp->decl->decl_init, level);
		}
		gen_code(";\n");
	}
}

static void gen_state_func(
	const char *ss_name,
	uint ss_num,
	const char *state_name,
	Node *xp,
	void (*gen_body)(Node *xp, int context),
	int context,
	const char *title,
	const char *prefix,
	const char *rettype,
	const char *extra_args
)
{
	gen_code("\n/* %s function for state \"%s\" in state set \"%s\" */\n",
		title, state_name, ss_name);
	gen_code("static %s %s_%s_%d_%s(SS_ID " NM_ENV "%s)\n",
		rettype, prefix, ss_name, ss_num, state_name, extra_args);
	gen_body(xp, context);
}

static void gen_block(Node *xp, int context, int level)
{
	Node	*cxp;

	assert(xp->tag == S_CMPND);
	gen_code("{\n");
	gen_local_var_decls(xp, context, level+1);
	gen_defn_c_code(xp, level+1);
	foreach (cxp, xp->cmpnd_stmts)
	{
		gen_expr(context, cxp, level+1);
	}
	indent(level); gen_code("}\n");
}

static void gen_entex_body(Node *xp, int context)
{
	assert(xp->tag == D_ENTEX);
	gen_block(xp->entex_block, context, 0);
}

/* Generate action processing functions:
   Each state has one action routine.  It's name is derived from the
   state set name and the state name. */
static void gen_action_body(Node *xp, int context)
{
	Node		*tp;
	int		trans_num;
	const int	level = 1;

	gen_code("{\n");
	/* "switch" statment based on the transition number */
	indent(level); gen_code("switch(" NM_TRN ")\n");
	indent(level); gen_code("{\n");
	trans_num = 0;

	/* For each transition ("when" statement) ... */
	foreach (tp, xp)
	{
		assert(tp->tag == D_WHEN);
		/* one case for each transition */
		indent(level); gen_code("case %d:\n", trans_num);
		indent(level+1); gen_block(tp->when_block, context, level+1);
		/* end of case */
		indent(level+1); gen_code("return;\n");
		trans_num++;
	}
	/* end of switch stmt */
	indent(level); gen_code("}\n");
	/* end of function */
	gen_code("}\n");
}

/* Generate a C function that checks events for a particular state */
static void gen_event_body(Node *xp, int context)
{
	Node		*tp;
	int		trans_num;
	const int	level = 1;

	gen_code("{\n");
	trans_num = 0;
	/* For each transition generate an "if" statement ... */
	foreach (tp, xp)
	{
		Node *next_sp;

		assert(tp->tag == D_WHEN);
		if (tp->when_cond)
			gen_line_marker(tp->when_cond);
		indent(level); gen_code("if (");
		if (tp->when_cond == 0)
			gen_code("TRUE");
		else
			gen_expr(C_COND, tp->when_cond, 0);
		gen_code(")\n");
		indent(level); gen_code("{\n");

		next_sp = tp->extra.e_when->next_state;
		if (!next_sp)
		{
			/* "when(...) {...} exit" -> exit from program */
			indent(level+1);
			gen_code("seq_exit(" NM_ENV ");\n");
		}
		else
		{
			indent(level+1);
			gen_code("*" NM_PNST " = %d;\n", next_sp->extra.e_state->index);
		}
		indent(level+1);gen_code("*" NM_PTRN " = %d;\n", trans_num);
		indent(level+1); gen_code("return TRUE;\n");
		indent(level); gen_code("}\n");
		trans_num++;
	}
	indent(level); gen_code("return FALSE;\n");
	/* end of function */
	gen_code("}\n");
}

static void gen_var_access(Var *vp)
{
	const char *pre = global_options.reent ? NM_VAR "->" : "";

	assert(vp);
	assert(vp->scope);

#ifdef DEBUG
	report("var_access: %s, scope=(%s,%s)\n",
		vp->name, node_name(vp->scope), vp->scope->token.str);
#endif
	assert(is_scope(vp->scope));

	if (vp->type->tag == T_EVFLAG)
	{
		gen_code("%s", vp->name);
	}
	else if (vp->type->tag == T_NONE || vp->type->tag == T_FUNCTION)
	{
		gen_code("%s", vp->name);
	}
	else if (vp->scope->tag == D_PROG)
	{
		gen_code("%s%s", pre, vp->name);
	}
	else if (vp->scope->tag == D_SS)
	{
		gen_code("%s%s_%s.%s", pre, NM_VARS, vp->scope->token.str, vp->name);
	}
	else if (vp->scope->tag == D_STATE)
	{
		gen_code("%s%s_%s.%s_%s.%s", pre, NM_VARS,
			vp->scope->extra.e_state->var_list->parent_scope->token.str,
			NM_VARS, vp->scope->token.str, vp->name);
	}
	else	/* compound or when stmt => generate a local C variable */
	{
		gen_code("%s", vp->name);
	}
}

/* Recursively generate code for a syntax node */
static void gen_expr(
	int context,
	Node *ep,		/* node to generate code for */
	int level		/* indentation level */
)
{
	Node	*cep;		/* child node */

	if (ep == 0)
		return;

#ifdef	DEBUG
	report("gen_expr(%s,%s)\n", node_name(ep), ep->token.str);
#endif

	switch(ep->tag)
	{
	/* Statements */
	case S_CMPND:
		indent(level); gen_block(ep, context, level);
		break;
	case S_STMT:
		gen_line_marker(ep);
		indent(level);
		gen_expr(context, ep->stmt_expr, 0);
		gen_code(";\n");
		break;
	case S_IF:
		gen_line_marker(ep);
		indent(level);
		gen_code("if (");
		gen_expr(context, ep->if_cond, 0);
		gen_code(")\n");
		cep = ep->if_then;
		gen_expr(context, cep, cep->tag == S_CMPND ? level : level+1);
		if (ep->if_else)
		{
			indent(level);
			gen_code("else\n");
			cep = ep->if_else;
			gen_expr(context, cep, cep->tag == S_CMPND ? level : level+1);
		}
		break;
	case S_WHILE:
		gen_line_marker(ep);
		indent(level);
		gen_code("while (");
		gen_expr(context, ep->while_cond, 0);
		gen_code(")\n");
		cep = ep->while_stmt;
		gen_expr(context, cep, cep->tag == S_CMPND ? level : level+1);
		break;
	case S_FOR:
		gen_line_marker(ep);
		indent(level);
		gen_code("for (");
		gen_expr(context, ep->for_init, 0);
		gen_code("; ");
		gen_expr(context, ep->for_cond, 0);
		gen_code("; ");
		gen_expr(context, ep->for_iter, 0);
		gen_code(")\n");
		cep = ep->for_stmt;
		gen_expr(context, cep, cep->tag == S_CMPND ? level : level+1);
		break;
	case S_JUMP:
		indent(level);
		gen_code("%s;\n", ep->token.str);
		break;
	case S_CHANGE:
		if (context != C_TRANS)
		{
			error_at_node(ep, "state change statement not allowed here\n");
			break;
		}
		indent(level);
		gen_code("{*" NM_PNST " = %d; return;}\n", ep->extra.e_change->extra.e_state->index);
		break;
	case S_RETURN:
		if (context != C_FUNC)
		{
			error_at_node(ep, "return statement not allowed here\n");
			break;
		}
		indent(level);
		gen_code("return "); gen_expr(context, ep->return_expr, level); gen_code(";\n");
		break;
	/* Expressions */
	case E_VAR:
		gen_var_access(ep->extra.e_var);
		break;
	case E_SUBSCR:
		gen_expr(context, ep->subscr_operand, 0);
		gen_code("[");
		gen_expr(context, ep->subscr_index, 0);
		gen_code("]");
		break;
	case E_CONST:
		if (ep->extra.e_const)
			gen_code("%s", ep->extra.e_const->name);
		else
			gen_code("%s", ep->token.str);
		break;
	case E_STRING:
		gen_code("\"%s\"", ep->token.str);
		break;
	case E_FUNC:
		if (ep->func_expr->tag == E_BUILTIN)
		{
			gen_builtin_call(context, ep);
			break;
		}
		gen_expr(context, ep->func_expr, 0);
		gen_code("(");
		if (type_is_function(ep->func_expr))
		{
			/* add arguments for implicit parameters */
			gen_code(NM_ENV);
			if (ep->func_args)
				gen_code(", ");
		}
		foreach (cep, ep->func_args)
		{
			gen_expr(context, cep, 0);
			if (cep->next)
				gen_code(", ");
		}
		gen_code(")");
		break;
	case E_INIT:
		gen_code("{");
		foreach (cep, ep->init_elems)
		{
			gen_expr(context, cep, 0);
			if (cep->next)
				gen_code(", ");
		}
		gen_code("}");
		break;
	case E_TERNOP:
		gen_expr(context, ep->ternop_cond, 0);
		gen_code(" ? ");
		gen_expr(context, ep->ternop_then, 0);
		gen_code(" : ");
		gen_expr(context, ep->ternop_else, 0);
		break;
	case E_BINOP:
		gen_expr(context, ep->binop_left, 0);
		gen_code(" %s ", ep->token.str);
		gen_expr(context, ep->binop_right, 0);
		break;
	case E_SELECT:
		gen_expr(context, ep->select_left, 0);
		gen_code("%s", ep->token.str);
		gen_expr(context, ep->select_right, 0);
		break;
	case E_MEMBER:
		gen_code("%s", ep->token.str);
		break;
	case E_PAREN:
		gen_code("(");
		gen_expr(context, ep->paren_expr, 0);
		gen_code(")");
		break;
	case E_CAST:
		gen_code("(");
		gen_expr(context, ep->cast_type, 0);
		gen_code(")");
		gen_expr(context, ep->cast_operand, 0);
		break;
	case E_PRE:
		gen_code("%s", ep->token.str);
		gen_expr(context, ep->pre_operand, 0);
		break;
	case E_POST:
		gen_expr(context, ep->post_operand, 0);
		gen_code("%s", ep->token.str);
		break;
	/* C-code can be either definition, statement, or expression */
	case T_TEXT:
		indent(level);
		gen_code("%s\n", ep->token.str);
		break;
	case D_DECL:
		gen_var_decl(ep->extra.e_decl);
		break;
	default:
		assert_at_node(impossible, ep, "unhandled expression (%s:%s)\n",
			node_name(ep), ep->token.str);
	}
}

/* Generate builtin function call */
static void gen_builtin_call(int context, Node *ep)
{
	Node *ap;	/* argument node */
	struct func_symbol *fsym = ep->func_expr->extra.e_builtin;
	const struct param **ppp;
	uint n = 1;

	assert(ep->func_expr->tag == E_BUILTIN);
	assert(fsym);

	/* All builtin functions require ssId as 1st parameter */
	assert_at_node(context != C_GLOBAL, ep,
		"calling built-in function %s not allowed here\n", fsym->name);
	gen_code("seq_%s("NM_ENV, fsym->c_name ? fsym->c_name : fsym->name);
	if (fsym->cond_only && context != C_COND)
	{
		error_at_node(ep,
			"calling %s is only allowed inside a when condition\n", fsym->name);
		return;
	}
	if (fsym->action_only && context == C_COND)
	{
		error_at_node(ep,
			"calling %s is not allowed inside a when condition\n", fsym->name);
		return;
	}

	assert(fsym->params);
	ppp = fsym->params;
	ap = ep->func_args;
	while (*ppp)
	{
		const struct param *pp = *ppp;
		gen_code(", ");
		if (!ap)
		{
			if (pp->default_value)
				gen_code("%s", pp->default_value);
			else
			{
				error_at_node(ep,
					"not enough arguments to built-in function '%s'\n",
					fsym->name);
				return;
			}
		}
		else
		{
			switch(pp->type)
			{
			case PT_OTHER:
				gen_expr(context, ap, 0);
				break;
			case PT_EF:
				gen_ef_arg(context, fsym->name, ap, n);
				break;
			case PT_PV:
				gen_pv_arg(context, fsym->name, ap, n, FALSE);
				break;
			case PT_PV_ARRAY:
				gen_pv_arg(context, fsym->name, ap, n, TRUE);
				break;
			}
		}
		ppp++;
		n++;
		if (ap)
			ap = ap->next;
	}
	if (ap)
		error_at_node(ep,
			"too many arguments to built-in function '%s'\n", fsym->name);
	gen_code(")");
}

/* Check an event flag argument */
static void gen_ef_arg(
	int		context,
	const char	*func_name,	/* function name */
	Node		*ap,		/* argument expression */
	uint		index		/* argument index */
)
{
	if (ap->tag == E_CONST && ap->extra.e_const->type == CT_EVFLAG)
		gen_expr(context, ap, 0);
	else if (ap->tag != E_VAR || ap->extra.e_var->type->tag != T_EVFLAG)
		error_at_node(ap,
			"argument %d to built-in function %s must be an event flag\n",
			index, func_name);
	else
		gen_var_access(ap->extra.e_var);
}

static void gen_pv_arg(
	int		context,
	const char	*func_name,	/* function name */
	Node		*ap,		/* argument expression */
	uint		index,		/* argument index */
	uint		pv_array	/* function expects a pv array */
)
{
	Var *vp = 0;
	Node *subscr = 0;

	switch (ap->tag)
	{
	case E_VAR:
		vp = ap->extra.e_var;
		assert(vp);
		if (vp->assign == M_MULTI && !pv_array)
		{
			error_at_node(ap,
				"passing multi-PV array '%s' to function '%s' is no "
				"longer allowed\n",
				vp->name, func_name);
			report_at_node(ap, "Perhaps you meant to pass '%s[0]' or "
				"call the pvArray... variant?\n", vp->name);
			return;
		}
		if (vp->assign == M_SINGLE && pv_array)
		{
			error_at_node(ap,
				"passing single-PV variable '%s' to function '%s' is not "
				"allowed\n",
				vp->name, func_name);
			return;
		}
		break;
	case E_SUBSCR:
		subscr = ap->subscr_index;
		if (ap->subscr_operand->tag == E_VAR)
		{
			vp = ap->subscr_operand->extra.e_var;
			break;
		}
		/* fall through */
	default:
		error_at_node(ap,
			"parameter %d to '%s' must be a variable or subscripted variable\n",
			index, func_name);
		return;
	}
	assert(vp);

	if (vp->assign == M_NONE)
	{
		error_at_node(ap,
			"parameter %d to '%s' was not assigned to a pv\n",
			index, func_name);
		gen_code("?/*%s*/", vp->name);
	}
	else if (ap->tag == E_SUBSCR && vp->assign != M_MULTI)
	{
		error_at_node(ap,
			"parameter %d to '%s' is subscripted but the variable "
			"it refers to has not been assigned to multiple pvs\n",
			index, func_name);
		gen_code("%d/*%s*/", vp->index, vp->name);
	}
	else
	{
		gen_code("%d/*%s*/", vp->index, vp->name);
	}

	if (ap->tag == E_SUBSCR)
	{
		/* e.g. pvPut(xyz[i+2]); => seq_pvPut(ssId, 3 + (i+2)); */
		gen_code(" + (CH_ID)(");
		/* generate the subscript expression */
		gen_expr(context, subscr, 0);
		gen_code(")");
	}
}

static void gen_var_init(Var *vp, int context, int level)
{
	assert(vp);
	assert(vp->decl);

	/* We need to do generate initializing code only if an initializer
	is present, since the generated C variables are either static
	(non-reentrant mode) or allocated with calloc (reentrant mode), and
	thus always initialized to zero. */
	if (vp->decl->decl_init)
	{
		indent(level); gen_code("{\n");
		gen_line_marker(vp->decl->decl_init);
		indent(level); gen_code("static ");
		gen_type(vp->type, NM_INITVAR, vp->name);
		gen_code(" = ");
		gen_expr(context, vp->decl->decl_init, level);
		gen_code(";\n");
		indent(level); gen_code("memcpy(&");
		gen_var_access(vp);
		gen_code(", &" NM_INITVAR "%s, sizeof(" NM_INITVAR "%s));\n",
			vp->name, vp->name);
		indent(level); gen_code("}\n");
	}
}

/* Generate initializers for variables of global lifetime */
static void gen_user_var_init(Node *prog, int level)
{
	Var *vp;
	Node *ssp;

	assert(prog->tag == D_PROG);
	/* global variables */
	foreach(vp, prog->extra.e_prog->first)
	{
		if (vp->decl && vp->decl->decl_init)
		{
			assert(vp->type->tag != T_NONE);	/* syntax */
			if (vp->type->tag == T_EVFLAG)
				error_at_node(vp->decl->decl_init,
					"event flag '%s' cannot be initialized\n",
					vp->name);
			else
				gen_var_init(vp, C_GLOBAL, level);
		}
	}
	/* state and state set variables */
	foreach (ssp, prog->prog_statesets)
	{
		Node *sp;

		assert(ssp->tag == D_SS);
		/* state set variables */
		foreach(vp, ssp->extra.e_ss->var_list->first)
		{
			gen_var_init(vp, C_SS, level);
		}
		foreach (sp, ssp->ss_states)
		{
			assert(sp->tag == D_STATE);
			/* state variables */
			foreach (vp, sp->extra.e_state->var_list->first)
			{
				gen_var_init(vp, C_SS, level);
			}
		}
	}
}

static void gen_prog_func(
	Node *prog,
	const char *doc,
	const char *name,
	void (*gen_body)(Node *prog)
)
{
	assert(prog->tag == D_PROG);
	gen_code("\n/* Program %s func */\n", doc);
	gen_code("static void %s(PROG_ID " NM_ENV ")\n{\n",
		name);
	gen_body(prog);
	gen_code("}\n");
}

static void gen_prog_entex_func(
	Node *prog,
	const char *doc,
	const char *name,
	void (*gen_body)(Node *)
)
{
	assert(prog->tag == D_PROG);
	gen_code("\n/* Program %s func */\n", doc);
	gen_code("static void %s(SS_ID " NM_ENV ")\n",
		name);
	gen_body(prog);
}

static void gen_prog_init_body(Node *prog)
{
	assert(prog->tag == D_PROG);
	gen_user_var_init(prog, 1);
}

static void gen_prog_entry_body(Node *prog)
{
	assert(prog->tag == D_PROG);
	gen_entex_body(prog->prog_entry, C_SS);
}

static void gen_prog_exit_body(Node *prog)
{
	assert(prog->tag == D_PROG);
	gen_entex_body(prog->prog_exit, C_SS);
}

void gen_funcdef(Node *fp)
{
	if (fp->tag == D_FUNCDEF)
	{
		Var *vp = fp->funcdef_decl->extra.e_decl;

		gen_code("\n");
		gen_code("#define " NM_VAR " (*(struct " NM_VARS " *const *)" NM_ENV ")\n");
		gen_line_marker(vp->decl);
		gen_code("static ");
		gen_var_decl(vp);
		gen_code("\n");
		gen_block(fp->funcdef_block, C_FUNC, 0);
		gen_code("#undef " NM_VAR "\n");
	}
}
