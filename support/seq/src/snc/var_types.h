/*************************************************************************\
Copyright (c) 2010-2015 Helmholtz-Zentrum Berlin f. Materialien
                        und Energie GmbH, Germany (HZB)
This file is distributed subject to a Software License Agreement found
in the file LICENSE that is included with this distribution.
\*************************************************************************/
#ifndef INCLvar_typesh
#define INCLvar_typesh

#define declare_prim_type_names
#include "seq_prim_types.h"
#include "types.h"

enum type_tag {
    T_NONE,     /* undeclared (or declared as foreign) variable */
    T_EVFLAG,   /* event flags */
    T_VOID,     /* void type */
    T_PRIM,     /* primitive types: numbers, char, string */
    T_FOREIGN,  /* foreign types (declared in C code) */
    T_POINTER,
    T_ARRAY,
    T_FUNCTION,
#if 0
    T_CONST,
#endif
    T_STRUCT,   /* struct defined in SNL code */
};

enum foreign_type_tag {
    F_ENUM,
    F_STRUCT,
    F_UNION,
    F_TYPENAME,
};

struct type {
    enum type_tag tag;
    unsigned is_const:1;
    union {
        enum prim_type_tag prim;
        struct {
            enum foreign_type_tag tag;
            char *name;
        } foreign;
        struct {
            Type *value_type;
        } pointer;
        struct {
            unsigned num_elems;
            Type *elem_type;
        } array;
        struct {
            Type *return_type;
            Node *param_decls;
        } function;
#if 0
        struct {
            Type *value_type;
        } constant;
#endif
        struct {
            const char *name;
            Node *member_decls;
        } structure;
    } val;
};

/* base type for any combination of pointers and arrays */
Type *base_type(Type *t);

/* array length in 1st and 2nd dimension */
unsigned type_array_length1(Type *t);
unsigned type_array_length2(Type *t);

/* whether type can be assign'ed to a PV */
unsigned type_assignable(Type *t);

/* generate code for a type, name is an optional variable name  */
void gen_type(Type *t, const char *prefix, const char *name);

/* creating types */
Type *mk_prim_type(enum prim_type_tag tag);
Type *mk_foreign_type(enum foreign_type_tag tag, char *name);
Type *mk_ef_type();
Type *mk_void_type();
Type *mk_no_type();
Type *mk_pointer_type(Type *t);
Type *mk_array_type(Type *t, unsigned n);
Type *mk_const_type(Type *t);
Type *mk_function_type(Type *t, Node *ps);
Type *mk_structure_type(const char *name, Node *members);

Node *mk_decl(Node *d, Type *t);
Node *mk_decls(Node *ds, Type *t);

void dump_type(Type *t, int l);

#ifndef var_types_GLOBAL
extern
#endif
const char *foreign_type_prefix[]
#ifdef var_types_GLOBAL
= {
    "enum ",
    "struct ",
    "union ",
    "",
}
#endif
;

#ifndef var_types_GLOBAL
extern
#endif
const char *type_tag_names[]
#ifdef var_types_GLOBAL
= {
    "T_NONE",
    "T_EVFLAG",
    "T_VOID",
    "T_PRIM",
    "T_FOREIGN",
    "T_POINTER",
    "T_ARRAY",
    "T_FUNCTION",
#if 0
    "T_CONST",
#endif
    "T_STRUCT",
}
#endif
;

#endif /*INCLvar_typesh */
