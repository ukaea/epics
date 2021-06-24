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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "analysis.h"
#include "gen_ss_code.h"
#include "gen_tables.h"
#include "main.h"
#include "var_types.h"
#include "gen_code.h"

static const int impossible = 0;

static void gen_main(char *prog_name);
static void gen_var_struct(Node *prog, uint opt_reent);
static void gen_init_reg(char *prog_name);
static void gen_func_decls(Node *prog);
static void gen_global_defn(Node *defn);

static int assert_var_declared(Node *ep, Node *scope, void *parg)
{
#ifdef DEBUG
	report("assert_var_declared: '%s' in scope (%s:%s)\n",
		ep->token.str, node_name(scope), scope->token.str);
#endif
	assert(ep->tag == E_VAR);
	assert(ep->extra.e_var != 0);
	assert(ep->extra.e_var->decl != 0 ||
		ep->extra.e_var->type->tag == T_NONE);
	return TRUE;		/* there are no children anyway */
}

/* Generate C code from parse tree. */
void generate_code(Program *p)
{
	Node *defn;

	/* assume there have been no errors, so all vars are declared */
	traverse_syntax_tree(p->prog, bit(E_VAR), 0, 0, assert_var_declared, 0);

#ifdef DEBUG
	report("-------------------- Code Generation --------------------\n");
#endif

	/* Initial comments */
	gen_code("/* C code for program %s, ", p->name);
	gen_code("generated by snc from %s */\n", p->prog->token.file);

	/* Includes */
	gen_code("#include <string.h>\n");
	gen_code("#include <stddef.h>\n");
	gen_code("#include <stdio.h>\n");
	gen_code("#include <limits.h>\n");
	gen_code("\n");
	gen_code("#include \"seq_snc.h\"\n");

	/* Initial definitions *except* global variable declarations,
	   in the order in which they appear in the program.
	   Note: this includes escaped C code. */
	foreach (defn, p->prog->prog_defns) gen_global_defn(defn);

	/* Variable declarations */
	gen_var_struct(p->prog, p->options.reent);

	/* Function declarations */
	gen_func_decls(p->prog);

	/* State and state set functions */
	gen_ss_code(p->prog, p->options);

	/* Channel, state set, and program tables */
	gen_tables(p);

	/* Extra definitions */
	foreach (defn, p->prog->prog_xdefns) gen_global_defn(defn);

	/* Main function */
	if (p->options.main) gen_main(p->name);

	/* Sequencer registration */
	gen_init_reg(p->name);
}

/* Generate main program */
static void gen_main(char *prog_name)
{
	gen_code("\n#define PROG_NAME %s\n", prog_name);
	gen_code("#include \"seqMain.c\"\n");
}

void gen_var_decl(Var *vp)
{
	gen_type(vp->type, "", vp->name);
}

static void gen_func_decls(Node *prog)
{
	Var	*vp;

	assert(prog->tag == D_PROG);
	gen_code("\n/* Function declarations */\n");

	/* function declarations are always global and static */
	foreach (vp, var_list_from_scope(prog)->first)
	{
		if (vp->decl && vp->type->tag == T_FUNCTION)
		{
			gen_line_marker(vp->decl);
			gen_code("static ");
			gen_var_decl(vp);
			gen_code(";\n");
		}
	}
}

/* Generate the struct containing all program variables with
   'infinite' (global) lifetime. These are: variables declared at the
   top-level, inside a state set, and inside a state. Note that state
   set and state local variables are _visible_ only inside the block
   where they are declared, but still have global lifetime. To avoid
   name collisions, generate a nested struct for each state set, and
   for each state in a state set. */
static void gen_var_struct(Node *prog, uint opt_reent)
{
	Var	*vp;
	Node	*sp, *ssp;
	uint	num_globals = 0;
	uint	num_decls = 0;

	gen_code("\n/* Variable declarations */\n");

	if (opt_reent)
	{
		gen_code("struct %s {\n", NM_VARS);
	}
	/* Convert internal type to `C' type */
	foreach (vp, var_list_from_scope(prog)->first)
	{
		if (vp->decl && vp->type->tag != T_NONE && vp->type->tag != T_EVFLAG &&
			vp->type->tag != T_FUNCTION)
		{
			gen_line_marker(vp->decl);
			if (!opt_reent) gen_code("static");
			indent(1);
			gen_var_decl(vp);
			num_decls++;
			gen_code(";\n");
			num_globals++;
		}
	}
	if (opt_reent && !num_globals)
	{
		indent(1); gen_code("char "NM_DUMMY";\n");
	}
	foreach (ssp, prog->prog_statesets)
	{
		int level = opt_reent;
		int ss_empty = !ssp->extra.e_ss->var_list->first;

		if (ss_empty)
		{
			foreach (sp, ssp->ss_states)
			{
				if (sp->extra.e_state->var_list->first)
				{
					ss_empty = 0;
					break;
				}
			}
		}

		if (!ss_empty)
		{
			indent(level); gen_code("struct %s_%s {\n", NM_VARS, ssp->token.str);
			foreach (vp, ssp->extra.e_ss->var_list->first)
			{
				gen_line_marker(vp->decl);
				indent(level+1); gen_var_decl(vp); gen_code(";\n");
				num_decls++;
			}
			foreach (sp, ssp->ss_states)
			{
				int s_empty = !sp->extra.e_state->var_list->first;
				if (!s_empty)
				{
					indent(level+1);
					gen_code("struct {\n");
					foreach (vp, sp->extra.e_state->var_list->first)
					{
						gen_line_marker(vp->decl);
						indent(level+2); gen_var_decl(vp); gen_code(";\n");
						num_decls++;
					}
					indent(level+1);
					gen_code("} %s_%s;\n", NM_VARS, sp->token.str);
				}
			}
			indent(level); gen_code("} %s_%s", NM_VARS, ssp->token.str);
			gen_code(";\n");
		}
	}
	if (opt_reent)
	{
		if (!num_decls)
		{
			indent(1); gen_code("char dummy;\n");
		}
		gen_code("};\n");
	}
	gen_code("\n");
}

/* Generate C code in definition section */
void gen_defn_c_code(Node *scope, int level)
{
	Node	*ep;
	int	first = TRUE;
	Node	*defn_list = defn_list_from_scope(scope);

	foreach (ep, defn_list)
	{
		if (ep->tag == T_TEXT)
		{
			if (first)
			{
				first = FALSE;
				indent(level);
				gen_code("/* C code definitions */\n");
			}
			gen_line_marker(ep);
			indent(level);
			gen_code("%s\n", ep->token.str);
		}
	}
}

static void gen_global_defn(Node *ep)
{
	Node *member;
	Var *vp;

	switch(ep->tag)
	{
	case T_TEXT:
		gen_line_marker(ep);
		gen_code("%s\n", ep->token.str);
		break;
	case D_FUNCDEF:
		gen_funcdef(ep);
		break;
	case D_STRUCTDEF:
		gen_code("\nstruct %s {\n", ep->token.str);
		foreach (member, ep->structdef_members)
		{
			gen_line_marker(member);
			indent(1);
			switch (member->tag)
			{
			case D_DECL:
				gen_var_decl(member->extra.e_decl);
				gen_code(";\n");
				break;
			case T_TEXT:
				gen_code("%s\n", member->token.str);
				break;
			default:
				assert_at_node(impossible, member, "member->tag==%s\n",
					node_name(member));
			}
		}
		gen_code("};\n");
		break;
	case D_DECL:
		/* we handle only event flags here, for everything else see gen_var_struct */
		vp = ep->extra.e_decl;
		if (vp->type->tag == T_EVFLAG)
		{
			gen_line_marker(vp->decl);
			gen_code("static const EF_ID %s = %d;\n", vp->name, vp->chan.evflag->index);
		}
                break;
	case D_ASSIGN:
	case D_MONITOR:
	case D_OPTION:
	case D_SYNC:
	case D_SYNCQ:
		/* these have no direct correspondence to a C declaration */
		break;
	default:
		assert_at_node(impossible, ep, "ep->tag==%s\n", node_name(ep));
	}
}

static void gen_init_reg(char *prog_name)
{
	gen_code("\n/* Register sequencer commands and program */\n");
	gen_code("#include \"epicsExport.h\"\n");
	gen_code("static void %sRegistrar (void) {\n", prog_name);
	gen_code("    seqRegisterSequencerCommands();\n");
	gen_code("    seqRegisterSequencerProgram (&%s);\n", prog_name);
	gen_code("}\n");
	gen_code("epicsExportRegistrar(%sRegistrar);\n", prog_name);
}

void indent(int level)
{
	while (level-- > 0)
		gen_code("\t");
}
