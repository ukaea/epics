/*************************************************************************\
Copyright (c) 2013-2015 Helmholtz-Zentrum Berlin f. Materialien
                        und Energie GmbH, Germany (HZB)
This file is distributed subject to a Software License Agreement found
in the file LICENSE that is included with this distribution.
\*************************************************************************/
#ifndef INCLtype_checkh
#define INCLtype_checkh

#include "types.h"
#include "var_types.h"

/* return a first approximation of the type of an expression */
Type *type_of(Node *e);

/* return whether the given expression has type function or pointer to function */
int type_is_function(Node *e);

#endif /*INCLtype_checkh */
