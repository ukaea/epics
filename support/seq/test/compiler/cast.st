/*************************************************************************\
Copyright (c) 2010-2015 Helmholtz-Zentrum Berlin f. Materialien
                        und Energie GmbH, Germany (HZB)
This file is distributed subject to a Software License Agreement found
in the file LICENSE that is included with this distribution.
\*************************************************************************/
program castTest

option +r;

%%#include "foreignTypes.h"

ss test {
    state test {
        when () {
            int i = 0;
            int ai[2] = {0,0};
            int (*pai)[2] = 0;

            unsigned int ui;
            unsigned int *pui;
            unsigned int (*paui)[2];

            enum enum_test e;
            struct struct_test s;
            union union_test u;
            typename typedef_test t;

            ui = *(unsigned int *)&i;
            pui = (unsigned int *)ai;
            paui = (unsigned int (*)[2])pai;

            e = (enum enum_test) 42;
            s = *(struct struct_test *)&i;
            u = *(union union_test *)&i;
            t = *(typename typedef_test*)&i;
        } exit
    }
}
