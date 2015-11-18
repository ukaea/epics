/*************************************************************************\
Copyright (c) 2013-2015 Helmholtz-Zentrum Berlin f. Materialien
                        und Energie GmbH, Germany (HZB)
This file is distributed subject to a Software License Agreement found
in the file LICENSE that is included with this distribution.
\*************************************************************************/
/* Extremely rudimentary type checker. We do just enough to find out
   whether a given expression is a call to a C or an SNL function. */
#include <assert.h>
#include <string.h>
#include "main.h"
#include "snl.h"
#include "types.h"
#include "var_types.h"

static const int impossible = FALSE;

/* some type constants */
static Type num_type_s = { T_PRIM, 0, {P_INT} };
static Type str_type_s = { T_PRIM, 0, {P_STRING} };
static Type no_type_s = { T_NONE, 0, {0} };
static Type *num_type = &num_type_s;
static Type *str_type = &str_type_s;
static Type *no_type = &no_type_s;

static Type *type_error_at(Node *e)
{
    error_at_node(e, "type error");
    return no_type;
}

static Type *new_pointer_type(Type *t)
{
    Type *r = new(Type);
    r->tag = T_POINTER;
    r->val.pointer.value_type = t;
    return r;
}

static Type *member_type(Node *members, const char *name)
{
    Node *member;
    foreach(member, members) {
        if (member->tag == D_DECL && strcmp(member->token.str, name) == 0) {
            return member->extra.e_decl->type;
        }
    }
    return no_type;
}

/* Infer the type of an expression. A result of no_type means we can't say,
   either because of a type error, or because we don't have enough information,
   e.g. due to involvement of foreign types, functions, or variables. */
Type *type_of(Node *e)
{
    Type *t, *l, *r;

    switch (e->tag) {
    case E_BINOP:                       /* binary operator [left,right] */
        l = type_of(e->binop_left);
        r = type_of(e->binop_right);
        switch (e->token.symbol) {
        case TOK_SUB:
        case TOK_ADD:
            if (l->tag == T_POINTER) {
                return l;
            } else {
                /* TODO: what if it's a foreign pointer? */
                return num_type;
            }
        case TOK_ASTERISK:
        case TOK_SLASH:
            return num_type;
        case TOK_GT:
        case TOK_GE:
        case TOK_EQ:
        case TOK_NE:
        case TOK_LE:
        case TOK_LT:
            return num_type;
        case TOK_OROR:
        case TOK_ANDAND:
            return num_type;
        case TOK_LSHIFT:
        case TOK_RSHIFT:
            return num_type;
        case TOK_VBAR:
        case TOK_CARET:
        case TOK_AMPERSAND:
        case TOK_MOD:
            return num_type;
        case TOK_EQUAL:
        case TOK_ADDEQ:
        case TOK_SUBEQ:
        case TOK_ANDEQ:
        case TOK_OREQ:
        case TOK_DIVEQ:
        case TOK_MULEQ:
        case TOK_MODEQ:
        case TOK_LSHEQ:
        case TOK_RSHEQ:
        case TOK_XOREQ:
            return r;
        case TOK_COMMA:
            return r;
        }
    case E_BUILTIN:                     /* builtin function [] */
        /* TODO: create built-in functions with their real type */
        return num_type;
    case E_CAST:                        /* type cast [type,operand] */
        return e->cast_type->extra.e_decl->type;
    case E_CONST:                       /* numeric (inkl. character) constant [] */
        return num_type;
    case E_FUNC:                        /* function call [node,args] */
        t = type_of(e->func_expr);
        if (t->tag == T_FUNCTION) {
            return t->val.function.return_type;
        } else {
            return no_type;
        }
    case E_INIT:                        /* array or struct initializer [elems] */
        assert(impossible);
        return no_type;
    case E_MEMBER:                      /* struct or union member [] */
        /* because we handle this one in E_SELECT */
        assert(impossible);
        return no_type;
    case E_PAREN:                       /* parenthesis around an expression [node] */
        return type_of(e->paren_expr);
    case E_POST:                        /* unary postfix operator [operand] */
        return type_of(e->post_operand);
    case E_PRE:                         /* unary prefix operator [operand] */
        t = type_of(e->pre_operand);
        switch (e->token.symbol) {
        case TOK_ADD:
        case TOK_SUB:
            return t;
        case TOK_ASTERISK:
            if (t->tag == T_POINTER) {
                return t->val.pointer.value_type;
            } else if (t->tag == T_ARRAY) {
                return t->val.array.elem_type;
            } else if (t->tag == T_PRIM && t->val.prim == P_STRING) {
                return num_type;
            } else {
                return no_type;
            }
        case TOK_AMPERSAND:
            return new_pointer_type(t);
        case TOK_NOT:
        case TOK_TILDE:
            return num_type;
        case TOK_INCR:
        case TOK_DECR:
            return t;
        case TOK_SIZEOF:
            return num_type;
        }
    case E_SELECT:                      /* member selection [left,right] */
        t = type_of(e->select_left);
        switch (e->token.symbol) {
        case TOK_POINTER:
#ifdef DEBUG
            report("type_of(): op='->'\n");
#endif
            if (t->tag == T_POINTER)
                t = t->val.pointer.value_type;
            else
                return no_type;
        case TOK_PERIOD:
#ifdef DEBUG
            report("type_of(): op='.', t->tag=%s\n", type_tag_names[t->tag]);
#endif
            if (t->tag == T_STRUCT)
                return member_type(t->val.structure.member_decls, e->select_right->token.str);
            else
                return no_type;
        default:
            assert(impossible);
            return no_type;
        }
    case E_STRING:                      /* string constant [] */
        return str_type;               /* just a first approximation, of course */
    case E_SUBSCR:                      /* subscripted node [operand,index] */
        t = type_of(e->subscr_operand);
        switch (t->tag) {
        case T_ARRAY:
            return t->val.array.elem_type;
        case T_POINTER:
            return t->val.pointer.value_type;
        default:
            return type_error_at(e->subscr_operand);
        }
    case E_TERNOP:                      /* ternary operator [cond,then,else] */
        return type_of(e->ternop_then);
    case E_VAR:                         /* variable [] */
        return e->extra.e_decl->type;
    default:
        assert(impossible);
        return no_type;
    }
}

int type_is_function(Node *e)
{
    Type *t = type_of(e);

    assert(t);

#ifdef DEBUG
    report("type_is_function(): t->tag==%s\n", type_tag_names[t->tag]);
#endif

    return t->tag == T_FUNCTION ||
        (t->tag == T_POINTER && t->val.pointer.value_type->tag == T_FUNCTION);
}
