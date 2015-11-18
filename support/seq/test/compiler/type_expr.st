/*************************************************************************\
Copyright (c) 2013-2015 Helmholtz-Zentrum Berlin f. Materialien
                        und Energie GmbH, Germany (HZB)
This file is distributed subject to a Software License Agreement found
in the file LICENSE that is included with this distribution.
\*************************************************************************/
program type_exprTest

option +r;

entry {
    long n = 0;
    void *p = (void *)n;
    void **pp = (void**)n;
    void (*pf1)(void) = (void (*)(void))n;
    void (*pf2)(int x,int) = (void (*)(int,int))n;
    void (*pf3)(int*) = (void (*)(int*))n;
    void (*pf4)(int**) = (void (*)(int**))n;
    void (*pf5)(double**,int) = (void (*)(double**,int))n;
    void (*pf6)(double**,int(**)[2]) = (void (*)(double**,int(**)[2]))n;
    int const c = (int const)n;
    int const *pc = (int const *)n;
    unsigned long *const cp = (unsigned long *const)n;
    double **pd = (double**)(int*)n;
    double **ppd = (double**)(int(*)[3])n;
    void (*lf)(
        void *p,
        void **pp,
        void (*pf1)(void),
        void (*pf2)(int x,int),
        void (*pf3)(int*),
        void (*pf4)(int**),
        void (*pf5)(double**,int),
        void (*pf6)(double**,int(**)[2]),
        int const c,
        int const *pc,
        unsigned long *const cp,
        double **pd,
        double **ppd
    ) = (void (*)(
        void *p,
        void **pp,
        void (*pf1)(void),
        void (*pf2)(int x,int),
        void (*pf3)(int*),
        void (*pf4)(int**),
        void (*pf5)(double**,int),
        void (*pf6)(double**,int(**)[2]),
        int const c,
        int const *pc,
        unsigned long *const cp,
        double **pd,
        double **ppd
    ))n;
    lf(p,pp,pf1,pf2,pf3,pf4,pf5,pf6,c,pc,cp,pd,ppd);
}

#include "../simple.st"
