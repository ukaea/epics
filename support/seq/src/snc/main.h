/*************************************************************************\
Copyright (c) 2010-2015 Helmholtz-Zentrum Berlin f. Materialien
                        und Energie GmbH, Germany (HZB)
This file is distributed subject to a Software License Agreement found
in the file LICENSE that is included with this distribution.
\*************************************************************************/
/*************************************************************************\
            Various reporting and printing procedures
***************************************************************************/
#ifndef INCLsncmainh
#define INCLsncmainh

#if !defined(__GNUC__)
#define __attribute__(x)
#endif

void gen_code(const char *format, ...)
__attribute__((format(printf,1,2)));

/* append '# <line_num> "<src_file>"\n' to output (if not disabled by cmd-line option) */
void gen_line_marker_prim(int line_num, const char *src_file);

/* this gets the shorter name as it is the common way to call above */
#define gen_line_marker(ep) gen_line_marker_prim((ep)->token.line, (ep)->token.file)

/* Error and warning message support */

/* just the location info */
void report_loc(const char *src_file, int line_num);

/* location plus message */
void report_at(const char *src_file, int line_num, const char *format, ...)
__attribute__((format(printf,3,4)));

/* location plus message and increase error count */
void error_at(const char *src_file, int line_num, const char *format, ...)
__attribute__((format(printf,3,4)));

/* location plus message and report a bug in snc */
void assert_at(int cond, const char *src_file, int line_num, const char *format, ...)
__attribute__((format(printf,4,5)));

/* with location from this node */
struct syntax_node;
void report_at_node(struct syntax_node *ep, const char *format, ...)
__attribute__((format(printf,2,3)));

/* with location from this node but only if warnings are enabled */
void warning_at_node(struct syntax_node *ep, const char *format, ...)
__attribute__((format(printf,2,3)));

/* with location from this node but only if extra warnings are enabled */
void extra_warning_at_node(struct syntax_node *ep, const char *format, ...)
__attribute__((format(printf,2,3)));

/* with location from this node and increase error count */
void error_at_node(struct syntax_node *ep, const char *format, ...)
__attribute__((format(printf,2,3)));

/* with location from this node and report a bug in snc */
void assert_at_node(int cond, struct syntax_node *ep, const char *format, ...)
__attribute__((format(printf,3,4)));

/* message only */
void report(const char *format, ...)
__attribute__((format(printf,1,2)));

#endif	/*INCLsncmainh*/
