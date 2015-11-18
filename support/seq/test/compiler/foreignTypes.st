/*************************************************************************\
Copyright (c) 2010-2015 Helmholtz-Zentrum Berlin f. Materialien
                        und Energie GmbH, Germany (HZB)
This file is distributed subject to a Software License Agreement found
in the file LICENSE that is included with this distribution.
\*************************************************************************/
program foreignTypesTest

option +r;

%%#include "foreignTypes.h"

foreign E0, E1;

int i;

enum enum_test e0 = E0, e1 = E1;

struct struct_test s0 = {
    {{0}}, 0, 1.1, E0
}, s1 = {
    {{1}}, 1, 2.2, E1
};

union union_test u0, u1, u2, u3;

typename typedef_test t0 = {{{0}},0,0,0};

/* arrays and pointers */

struct struct_test *(*apaps[2])[3] = {0,0};

enum enum_test (*pe)[4];

struct struct_test *ps;
struct struct_test s;
typename typedef_test t;

ss test {
    int i;

    enum enum_test e0 = E0, e1 = E1;

    struct struct_test s0 = {
        {{0}}, 0, 1.1, E0
    }, s1 = {
        {{1}}, 1, 2.2, E1
    };

    union union_test u0, u1, u2, u3;

    typename typedef_test t0 = {{{0}},0,0,0};

    /* arrays and pointers */

    struct struct_test *(*apaps[2])[3] = {0,0};

    enum enum_test (*pe)[4];

    struct struct_test *ps;
    struct struct_test s;
    typename typedef_test t;
    state test {
        int i;

        enum enum_test e0 = E0, e1 = E1;

        struct struct_test s0 = {
            {{0}}, 0, 1.1, E0
        }, s1 = {
            {{0}}, 1, 2.2, E1
        };

        union union_test u0, u1, u2, u3;

        typename typedef_test t0 = {{{0}},0,0,0};

        /* arrays and pointers */

        struct struct_test *(*apaps[2])[3] = {0,0};

        enum enum_test (*pe)[4];

        struct struct_test *ps;
        struct struct_test s;
        typename typedef_test t;
        when () {
            typename FILE *f = fopen("bla", "r");
            *(*apaps[1])[2] = *(*apaps[0])[1];
            fread(0,0,0,f);
            fclose(f);
        } exit
    }
}
