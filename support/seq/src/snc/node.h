/*************************************************************************\
Copyright (c) 1989-1993 The Regents of the University of California
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
#ifndef INCLparseh
#define INCLparseh

#include "types.h"

/* defined in node.c */
Node *node(
	enum node_tag	tag,
	Token		tok,
	...
);

Node *opt_defn(
	Token	name,
	Token	value
);

Node *link_node(
	Node	*ep1,
	Node	*ep2
);

uint strtoui(
	char *str,		/* string representing a number */
	uint limit,		/* result should be < limit */
	uint *pnumber		/* location for result if successful */
);

#endif	/*INCLparseh*/
