/*************************************************************************\
Copyright (c) 1989-1993 The Regents of the University of California.
                        Los Alamos National Laboratory
Copyright (c) 2010-2015 Helmholtz-Zentrum Berlin f. Materialien
                        und Energie GmbH, Germany (HZB)
This file is distributed subject to a Software License Agreement found
in the file LICENSE that is included with this distribution.
\*************************************************************************/
/*************************************************************************\
		Common types, enumerations, and macros
\*************************************************************************/
#ifndef INCLtypesh
#define INCLtypesh

#include <stdlib.h>

#include "epicsVersion.h"

#include "seq_static_assert.h"

#ifndef	TRUE
#define	TRUE 1
#endif
#ifndef	FALSE
#define	FALSE 0
#endif

typedef	struct sym_table	SymTable;
typedef struct options		Options;
typedef struct state_options	StateOptions;
typedef struct token		Token;
typedef struct when		When;
typedef struct state		State;
typedef struct state_set	StateSet;
typedef struct program		Program;
typedef struct channel		Chan;
typedef struct event_flag	EvFlag;
typedef struct sync_queue	SyncQ;
typedef struct syntax_node	Node;
typedef struct variable		Var;
typedef struct chan_list	ChanList;
typedef struct sync_queue_list	SyncQList;
typedef struct var_list		VarList;
typedef struct func_symbol	FuncSym;
typedef struct const_symbol	ConstSym;
typedef struct type		Type;		/* struct defined in var_types.h */

typedef unsigned long long	TypeMask;
typedef unsigned int		uint;

struct sym_table
{
#if EPICS_REVISION<14 || (EPICS_REVISION==14 && EPICS_MODIFICATION <= 10)
	void *table;
#else
	struct gphPvt *table;
#endif
};

struct options
{					/* run-time options */
	uint	async:1;		/* do pvGet() asynchronously */
	uint	conn:1;			/* wait for all conns to complete */
	uint	debug:1;		/* run-time debug */
	uint	reent:1;		/* reentrant */
	uint	safe:1;			/* safe (no globals) */
	uint	newef:1;		/* new event flag mode */

					/* compile time options */
	uint	main:1;			/* generate main program */
	uint	line:1;			/* generate line markers */
	uint	warn:1;			/* compiler warnings */
	uint	xwarn:1;		/* extra compiler warnings */
};

#define DEFAULT_OPTIONS {0,1,0,0,0,1,0,1,1,0}

struct state_options			/* run-time state options */
{
	uint	do_reset_timers:1;	/* reset timers on state entry from self */
	uint	no_entry_from_self:1;	/* don't do entry actions if entering from same state */
	uint	no_exit_to_self:1;	/* don't do exit actions if exiting to same state */
};

#define DEFAULT_STATE_OPTIONS {1,1,1}

struct token				/* for the lexer and parser */
{
	int		symbol;
	char		*str;
	const char	*file;
	int		line;
};

struct when				/* extra data for when clauses */
{
	Node		*next_state;	/* declaration of target state */
};

struct state				/* extra data for state clauses */
{
	uint		index;		/* index in array of seqState structs */
	uint		is_target;	/* is this state a target state? */
	StateOptions	options;	/* state options */
	VarList		*var_list;	/* list of 'local' variables */
};

struct state_set			/* extra data for state set clauses */
{
	uint		num_states;	/* number of states */
	VarList		*var_list;	/* list of 'local' variables */
};

/* Expression types */
enum node_tag			/* description [child nodes...] */
{
	D_ASSIGN,		/* assign statement [subscr,pvs] */
	D_DECL,			/* variable declaration [init] */
	D_ENTEX,		/* entry or exit statement [block] */
	D_FUNCDEF,		/* function definition [decl,block] */
	D_MONITOR,		/* monitor statement [subscr] */
	D_OPTION,		/* option definition [] */
	D_PROG,			/* whole program [param,defns,entry,statesets,exit,xdefns] */
	D_SS,			/* state set statement [defns,states] */
	D_STATE,		/* state statement [defns,entry,whens,exit] */
	D_STRUCTDEF,		/* struct definition [members] */
	D_SYNC,			/* sync statement [subscr,evflag] */
	D_SYNCQ,		/* syncq statement [subscr,evflag,maxqsize] */
	D_WHEN,			/* when statement [cond,block] */

	E_BINOP,		/* binary operator [left,right] */
	E_BUILTIN,		/* builtin function [] */
	E_CAST,			/* type cast [type,operand] */
	E_CONST,		/* numeric (inkl. character) constant [] */
	E_FUNC,			/* function call [expr,args] */
	E_INIT,			/* array or struct initializer [elems] */
	E_MEMBER,		/* struct or union member [] */
	E_PAREN,		/* parenthesized expression [expr] */
	E_POST,			/* unary postfix operator [operand] */
	E_PRE,			/* unary prefix operator [operand] */
	E_SELECT,		/* member selection [left,right] */
	E_STRING,		/* string constant [] */
	E_SUBSCR,		/* subscripted expr [operand,index] */
	E_TERNOP,		/* ternary operator [cond,then,else] */
	E_VAR,			/* variable [] */

	S_CHANGE,		/* state change statement [] */
	S_CMPND,		/* compound statement [defns,stmts] */
	S_FOR,			/* for statement [init,cond,iter,stmt] */
	S_IF,			/* if statement [cond,then,else] */
	S_JUMP,			/* break or continue stmt [] */
	S_RETURN,		/* return stmt [expr] */
	S_STMT,			/* simple statement, i.e. 'expr;'  [expr] */
	S_WHILE,		/* while statement [cond,stmt] */

	T_TEXT,			/* C code or other text to be inserted [] */

	NUM_EXPR_TYPES
};

/* make sure we have no more node types than bits */
STATIC_ASSERT(NUM_EXPR_TYPES <= 8*sizeof(TypeMask));

struct syntax_node			/* generic syntax node */
{
	Node		*next;		/* next in list */
	Node		*last;		/* last in list */
	Node		**children;	/* array of children [left,right,...] */
	enum node_tag   tag;		/* what kind of node */
        Token		token;
	union				/* extra data, depends on tag */
	{
		Var	*e_var;		/* variable reference */
		Var	*e_decl;	/* variable declaration */
		uint	e_option;	/* option value (1 or 0) */
		VarList	*e_prog;	/* top-level declarations */
		StateSet *e_ss;		/* state set data */
		State	*e_state;	/* state data */
		When	*e_when;	/* transition data */
		Node	*e_change;	/* declaration of target state */
		VarList	*e_cmpnd;	/* block local declarations */
		FuncSym	*e_builtin;	/* builtin function */
		ConstSym *e_const;	/* builtin constant */
		VarList *e_funcdef;	/* parameters */
	}	extra;
};

struct event_flag
{
	uint	index;
	uint	queued;
};

struct variable				/* variable definition */
{
	Var	*next;			/* link to next variable in list */
	char	*name;			/* variable name */
	Node	*decl;			/* declaration of this variable
					   (or NULL if not declared) */
	Node	*scope;			/* scope of this variable */
	Type	*type;			/* type of this variable */
	/* channel stuff */
	uint	assign:2;		/* assigned: one of enum multiplicity */
	uint	monitor:2;		/* monitored: one of enum multiplicity */
	uint	sync:2;			/* sync'd: one of enum multiplicity */
	uint	syncq:2;		/* syncq'd: one of enum multiplicity */
	union {
		Chan	*single;	/* single channel if assign == ALL */
		Chan	**multi;	/* multiple channels if assign == SOME */
		EvFlag	*evflag;	/* event flag data if this is an event flag */
	} chan;
	uint	index;			/* index (base) in seqChan array */
};
/* Laws (Invariants):
L1a:	monitor	== M_MULTI	=> assign == M_MULTI
L1b:	sync	== M_MULTI	=> assign == M_MULTI
L1c:	syncq	== M_MULTI	=> assign == M_MULTI
L2a:	assign	== M_NONE	=> monitor == M_NONE
L2b:	assign	== M_NONE	=> sync == M_NONE
L2c:	assign	== M_NONE	=> syncq == M_NONE
L3:	assign	== M_MULTI	=> type->tag == T_ARRAY
*/

struct channel				/* channel assignment info */
{
	Chan	*next;			/* link to next channel in list */
	char	*name;			/* channel (pv) name */
	uint	index;			/* index (offset) if array element */
	Var	*var;			/* variable definition */
	uint	count;			/* request count for pv access */
	uint	monitor:1;		/* whether this channel is monitored */
	Var	*sync;			/* event flag variable if sync'd */
	SyncQ	*syncq;			/* sync queue if syncQ'd */
};

struct sync_queue
{
	SyncQ	*next;
	uint	index;
	uint	size;
};

struct chan_list
{
	Chan	*first, *last;		/* first and last member of the list */
	uint	num_elems;		/* number of elements in this list */
};

struct sync_queue_list
{
	SyncQ	*first, *last;		/* first and last member of the list */
	uint	num_elems;		/* number of elements in this list */
};

struct var_list
{
	Var	*first, *last;		/* first and last member of the list */
	Node	*parent_scope;		/* next surrounding scope */
};

struct program
{
	/* result of parsing phase */
	Node		*prog;		/* the whole syntax tree */

	/* these point into children of the prog node, for convenience */
	char		*name;		/* ptr to program name (string) */
	char		*param;		/* parameter string for program stmt */

	/* these are calculated in the analysis phase */
	Options		options;	/* program options, from source or command line */
	SymTable	sym_table;	/* symbol table */
	ChanList	*chan_list;	/* channel list, incl. number of channels */
	SyncQList	*syncq_list;	/* syncq list, incl. number of syncqs */
	uint		num_ss;		/* number of state sets */
	uint		num_event_flags;/* number of event flags */
};

/* Allocation */
#define newArray(type,count)	(type *)calloc(count, sizeof(type))
#define new(type)		newArray(type,1)

/* Generic iteration on lists */
#define foreach(e,l)		for (e = l; e != 0; e = e->next)
#define bit(bitn)		((TypeMask)(1ull)<<(bitn))

/* Commonly used sets of syntax_node tags */

/* Expression types that are scopes. By definition, a scope is a node
   that allows variable declarations as (immediate) subnodes. */
#define scope_mask		( bit(D_PROG)    | bit(D_FUNCDEF) \
				| bit(D_SS)      | bit(D_STATE)   | bit(S_CMPND) )
/* Whether a node is a scope */
#define is_scope(e)		((bit((e)->tag) & scope_mask) != 0)

/* Expression types that may have sub-scopes */
#define has_sub_scope_mask	( bit(D_ENTEX)   | bit(D_FUNCDEF) | bit(D_PROG)   | bit(D_SS)\
				| bit(D_STATE)   | bit(D_WHEN)    | bit(S_CMPND)  | bit(S_FOR)\
				| bit(S_IF)      | bit(S_STMT)    | bit(S_WHILE) )
/* Node types that are expressions i.e. no definitions or statements.
   These are the ones that start with E_. */
#define	expr_mask		( bit(E_BINOP)   | bit(E_CAST)    | bit(E_CONST)\
				| bit(E_FUNC)    | bit(E_INIT)\
				| bit(E_PAREN)   | bit(E_POST)    | bit(E_PRE)\
				| bit(E_SELECT)	 | bit(E_STRING)\
				| bit(E_SUBSCR)  | bit(E_TERNOP)  | bit(E_VAR)    | bit(T_TEXT) )

#define node_name(e)		node_info[(e)->tag].name

/* for channel assign, monitor, sync, and syncq */
enum multiplicity
{
	M_NONE,			/* not at all */
	M_SINGLE,		/* whole variable (array or scalar) */
	M_MULTI			/* array, each element treated separately */
};

/* Accessors for child nodes. Would like to define structs for the
   various node types with children, but then we could no longer
   uniformly iterate over all children... */
#define assign_subscr	children[0]
#define assign_pvs	children[1]
#define binop_left	children[0]
#define binop_right	children[1]
#define cast_type	children[0]
#define cast_operand	children[1]
#define cmpnd_defns	children[0]
#define cmpnd_stmts	children[1]
#define decl_init	children[0]
#define entex_block	children[0]
#define for_init	children[0]
#define for_cond	children[1]
#define for_iter	children[2]
#define for_stmt	children[3]
#define func_expr	children[0]
#define func_args	children[1]
#define funcdef_decl	children[0]
#define funcdef_block	children[1]
#define if_cond		children[0]
#define if_then		children[1]
#define if_else		children[2]
#define init_elems	children[0]
#define monitor_subscr	children[0]
#define paren_expr	children[0]
#define post_operand	children[0]
#define pre_operand	children[0]
#define prog_param	children[0]
#define prog_defns	children[1]
#define prog_entry	children[2]
#define prog_statesets	children[3]
#define prog_exit	children[4]
#define prog_xdefns	children[5]
#define return_expr	children[0]
#define select_left	children[0]
#define select_right	children[1]
#define ss_defns	children[0]
#define ss_states	children[1]
#define state_defns	children[0]
#define state_entry	children[1]
#define state_whens	children[2]
#define state_exit	children[3]
#define stmt_expr	children[0]
#define structdef_members	children[0]
#define subscr_operand	children[0]
#define subscr_index	children[1]
#define sync_subscr	children[0]
#define sync_evflag	children[1]
#define syncq_subscr	children[0]
#define syncq_evflag	children[1]
#define syncq_size	children[2]
#define ternop_cond	children[0]
#define ternop_then	children[1]
#define ternop_else	children[2]
#define when_cond	children[0]
#define when_block	children[1]
#define while_cond	children[0]
#define while_stmt	children[1]

#define funcdef_params	funcdef_decl->extra.e_decl->type->val.function.param_decls

#ifndef node_info_GLOBAL
extern
#endif
struct node_info
{
	const char *name;
	const uint num_children;
}
node_info[]
#ifdef node_info_GLOBAL
= {
	{ "D_ASSIGN",	2 },
	{ "D_DECL",	1 },
	{ "D_ENTEX",	1 },
	{ "D_FUNCDEF",	2 },
	{ "D_MONITOR",	1 },
	{ "D_OPTION",	0 },
	{ "D_PROG",	6 },
	{ "D_SS",	2 },
	{ "D_STATE",	4 },
	{ "D_STRUCTDEF",1 },
	{ "D_SYNC",	2 },
	{ "D_SYNCQ",	3 },
	{ "D_WHEN",	2 },
	{ "E_BINOP",	2 },
	{ "E_BUILTIN",	0 },
	{ "E_CAST",	2 },
	{ "E_CONST",	0 },
	{ "E_FUNC",	2 },
	{ "E_INIT",	1 },
	{ "E_MEMBER",	0 },
	{ "E_PAREN",	1 },
	{ "E_POST",	1 },
	{ "E_PRE",	1 },
	{ "E_SELECT",	2 },
	{ "E_STRING",	0 },
	{ "E_SUBSCR",	2 },
	{ "E_TERNOP",	3 },
	{ "E_VAR",	0 },
	{ "S_CHANGE",	0 },
	{ "S_CMPND",	2 },
	{ "S_FOR",	4 },
	{ "S_IF",	3 },
	{ "S_JUMP",	0 },
	{ "S_RETURN",	1 },
	{ "S_STMT",	1 },
	{ "S_WHILE",	2 },
	{ "T_TEXT",	0 },
}
#endif
;

#endif	/*INCLtypesh*/
