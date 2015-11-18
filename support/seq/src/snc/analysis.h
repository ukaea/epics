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
#ifndef INCLanalysish
#define INCLanalysish

#include "types.h"

/* Iteratee ("what gets iterated") for traverse_syntax_tree */
typedef int node_iter(Node *ep, Node *scope, void *parg);

/* Pre-order traversal of the syntax tree. Call the supplied iteratee whenever
 * call_mask has the (ep->type)'th bit set. The function is called with the current
 * ep, the current scope, and an additional user defined argument (argp). Afterwards,
 * if the iteratee returned a non-zero value, recurse into all child nodes except
 * those whose type'th bit is set in stop_mask.
 * The traversal starts at the first argument. The 4th argument is the current
 * scope; 0 may be supplied for it, in which case it will be set to a valid scope as
 * soon as the traversal encounters one.
 * NOTE: next pointer of the start node is ignored,
 * this functions does NOT descend into sibling list elements. */

void traverse_syntax_tree(
	Node		*ep,		/* start node */
	TypeMask	call_mask,	/* when to call iteratee */
	TypeMask	stop_mask,	/* when to stop descending */
	Node		*scope,		/* current scope, 0 at top-level */
	node_iter	*iteratee,	/* function to call */
	void		*parg		/* argument to pass to function */
);

VarList **pvar_list_from_scope(Node *scope);
#define var_list_from_scope(scope) (*pvar_list_from_scope(scope))
Node *defn_list_from_scope(Node *scope);

Program *analyse_program(Node *ep, Options options);

#endif	/*INCLanalysish*/
