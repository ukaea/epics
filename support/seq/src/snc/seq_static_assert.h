/*************************************************************************\
Copyright (c) 2010-2015 Helmholtz-Zentrum Berlin f. Materialien
                        und Energie GmbH, Germany (HZB)
This file is distributed subject to a Software License Agreement found
in the file LICENSE that is included with this distribution.
\*************************************************************************/
/*************************************************************************\
		static (compile time) assertions
\*************************************************************************/
#ifndef INCLstatic_asserth
#define INCLstatic_asserth

#ifndef STATIC_ASSERT

#define STATIC_JOIN(x, y) STATIC_JOIN2(x, y)
#define STATIC_JOIN2(x, y) x ## y
#define STATIC_ASSERT(expr) \
    typedef int STATIC_JOIN(static_assert_failed_at_line_, __LINE__) \
    [ (expr) ? 1 : -1 ]

/*
 * usage:
 *     STATIC_ASSERT(condition_to_assert, identifier_that_explains_the_assertion);
 */
#endif

#endif	/*INCLstatic_asserth*/
