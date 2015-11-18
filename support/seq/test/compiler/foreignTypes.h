/*************************************************************************\
Copyright (c) 2010-2015 Helmholtz-Zentrum Berlin f. Materialien
                        und Energie GmbH, Germany (HZB)
This file is distributed subject to a Software License Agreement found
in the file LICENSE that is included with this distribution.
\*************************************************************************/
#ifndef INCforeignTypes_h
#define INCforeignTypes_h

enum enum_test {
    E0, E1
};

struct struct_test {
    struct {
        union {
            double x;
        } y;
    } s;
    int i;
    double d;
    enum enum_test e;
};

union union_test {
    struct struct_test t;
    int i;
    double d;
    enum enum_test e;
};

typedef struct struct_test typedef_test;

#endif /* INCforeignTypes_h */
