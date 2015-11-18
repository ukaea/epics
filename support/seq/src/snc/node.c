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
                Parser support routines
\*************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include <errno.h>
#include <limits.h>

#define node_info_GLOBAL
#include "types.h"
#include "node.h"
#include "main.h"

static const StateOptions default_state_options = DEFAULT_STATE_OPTIONS;

Node *node(
	enum node_tag	tag,
	Token		tok,
	...			/* variable number of child arguments */
)
{
	va_list	argp;
	uint	i, num_children;
	Node	*ep;

	num_children = node_info[tag].num_children;

	ep = new(Node);
	ep->next = 0;
	ep->last = ep;
	ep->tag = tag;
        ep->token = tok;
	ep->children = newArray(Node *, num_children);
	/* allocate extra data */
	switch (tag)
	{
	case D_SS:
		ep->extra.e_ss = new(StateSet);
		break;
	case D_STATE:
		ep->extra.e_state = new(State);
		ep->extra.e_state->options = default_state_options;
		break;
	case D_WHEN:
		ep->extra.e_when = new(When);
		break;
	default:
		break;
	}

#ifdef	DEBUG
	report_at_node(ep, "node: ep=%p, tag=%s, value=\"%s\", file=%s, line=%d",
		ep, node_name(ep), tok.str, tok.file, tok.line);
#endif	/*DEBUG*/
	va_start(argp, tok);
	for (i = 0; i < num_children; i++)
	{
		ep->children[i] = va_arg(argp, Node*);
#ifdef	DEBUG
		report(", child[%d]=%p", i, ep->children[i]);
#endif	/*DEBUG*/
	}
	va_end(argp);
#ifdef	DEBUG
	report(")\n");
#endif	/*DEBUG*/

	return ep;
}

Node *opt_defn(Token name, Token value)
{
	Node *opt = node(D_OPTION, name);
	opt->extra.e_option = (value.str[0] == '+');
	return opt;
}

/* Link two expression structures and/or lists.  Returns ptr to combined list.
   Note: last ptrs are correct only for 1-st element of the resulting list */
Node *link_node(
	Node	*ep1,	/* 1-st structure or list */
	Node	*ep2	/* 2-nd (append it to 1-st) */
)
{
	if (ep1 == 0)
		return ep2;
	if (ep2 == 0)
		return ep1;
	ep1->last->next = ep2;
	ep1->last = ep2->last;
	ep2->last = 0;
	return ep1;
}

uint strtoui(
	char *str,		/* string representing a number */
	uint limit,		/* result should be < limit */
	uint *pnumber		/* location for result if successful */
)
{
	unsigned long result;

	errno = 0;
	result = strtoul(str, 0, 0);
	if (errno || result >= limit)
		return FALSE;
	*pnumber = result;
	return TRUE;
}
