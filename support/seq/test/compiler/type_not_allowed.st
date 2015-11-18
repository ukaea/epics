program type_not_allowed

#if 0
now syntax errors:
foreign *p_f;           /* error: pointer to foreign entity */
foreign const c_f;      /* error: constant foreign entity */
foreign a_f[1];         /* error: array of foreign entities */
foreign f_f();          /* error: function returning foreign entity */
#endif

evflag *p_ef;           /* error: pointer to event flag */
evflag a_ef[1];         /* error: array of event flags */
evflag const c_ef;      /* error: constant event flag */
evflag f_ef();          /* error: function returning event flag */

int (const c_a)[1];     /* error: constant array */
int (const c_f)();      /* warning: constant function */
int const const cc_i;   /* warning: repeated const */

void a_v[1];            /* error: array of void */

int f__v_i(void, int);  /* error: void must be the only parameter */
int f__v(void v);       /* error: void parameter should not have a name */

int a_i_zero[0];        /* error: invalid array size (must be >= 1) */

#include "../simple.st"
