/*************************************************************************\
Copyright (c) 1990      The Regents of the University of California
                        and the University of Chicago.
                        Los Alamos National Laboratory
Copyright (c) 2010-2015 Helmholtz-Zentrum Berlin f. Materialien
                        und Energie GmbH, Germany (HZB)
This file is distributed subject to a Software License Agreement found
in the file LICENSE that is included with this distribution.
\*************************************************************************/
/*************************************************************************\
                Main program, reporting and printing procedures
\*************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "types.h"
#include "parser.h"
#include "analysis.h"
#include "gen_code.h"
#include "main.h"

#include "seq_release.h"

static Options options = DEFAULT_OPTIONS;

static char *input_name;	/* input file name */
static char *output_name;	/* c output file name */

static FILE *out = NULL;	/* output file handle */

static int err_cnt;

static void parse_args(int argc, char *argv[]);
static void parse_option(char *s);
static void print_usage(void);
static char *replace_extension(const char *in, const char *ext);

/* The streams stdin and stdout are redirected to files named in the
   command parameters. */
int main(int argc, char *argv[])
{
	FILE	*in;
	Program	*prg;
        Node    *exp;

	/* Get command arguments */
	parse_args(argc, argv);

	in = fopen(input_name, "r");
	if (in == NULL)
	{
		perror(input_name);
		return EXIT_FAILURE;
	}
	out = fopen(output_name, "w");
	if (out == NULL)
	{
		perror(output_name);
		return EXIT_FAILURE;
	}

	/* the input file should be unbuffered,
           since the lexer maintains its own buffer */
	setvbuf(in, NULL, _IONBF, 0);

	exp = parse_program(in, input_name);

        prg = analyse_program(exp, options);

	if (err_cnt == 0)
		generate_code(prg);

	return err_cnt ? EXIT_FAILURE : EXIT_SUCCESS;
}

/* Initialize options, input_name, and output_name from arguments. */
static void parse_args(int argc, char *argv[])
{
	int i;

	if (argc < 2)
	{
		print_usage();
		exit(EXIT_FAILURE);
	}

	for (i=1; i<argc; i++)
	{
		char *s = argv[i];

		if (strcmp(s,"-o") == 0)
		{
			if (i+1 == argc)
			{
				report("missing filename after option -o\n");
				print_usage();
				exit(EXIT_FAILURE);
			}
			else
			{
				i++;
				output_name = argv[i];
				continue;
			}
		}
		else if (s[0] != '+' && s[0] != '-')
		{
			input_name = s;
			continue;
		}
		else
		{
			parse_option(s);
		}
	}
	if (options.safe && !options.reent) {
		options.reent = TRUE;
	}

	if (!input_name)
	{
		report("no input file argument given\n");
		print_usage();
		exit(EXIT_FAILURE);
	}

	if (!output_name)	/* no -o option given */
	{
		output_name = replace_extension(input_name, ".c");
	}
}

static char *replace_extension(const char *in, const char *ext)
{
	char *in_ext = strrchr(in, '.');
	size_t ext_len = strlen(ext);
	size_t in_len = in_ext ? (in_ext - in) : strlen(in);
	char *out = (char*)malloc(in_len+ext_len+1);

	strncpy(out, in, in_len);
	strcpy(out+in_len, ext);
	return out;
}

static void parse_option(char *s)
{
	uint		opt_val;

	opt_val = (*s == '+');

	switch (s[1])
	{
	case 'a':
		options.async = opt_val;
		break;
	case 'c':
		options.conn = opt_val;
		break;
	case 'd':
		options.debug = opt_val;
		break;
	case 'e':
		options.newef = opt_val;
		break;
	case 'r':
		options.reent = opt_val;
		break;
	case 'l':
		options.line = opt_val;
		break;
	case 'm':
		options.main = opt_val;
		break;
	case 's':
		options.safe = opt_val;
		break;
	case 'w':
		options.warn = opt_val;
		break;
	case 'W':
		options.xwarn = opt_val;
		break;
	default:
		report("unknown option ignored: '%s'\n", s);
		break;
	}
}

static void print_usage(void)
{
	report("%s\n", SEQ_RELEASE);
	report("usage: snc <options> <infile>\n");
	report("options:\n");
	report("  -o <outfile> - override name of output file\n");
	report("  +a           - do asynchronous pvGet\n");
	report("  -c           - don't wait for all connects\n");
	report("  +d           - turn on debug run-time option\n");
	report("  -e           - don't use new event flag mode\n");
	report("  -l           - suppress line numbering\n");
	report("  +m           - generate main program\n");
	report("  -i           - don't register commands/programs\n");
	report("  +r           - make reentrant at run-time\n");
	report("  +s           - safe mode (implies +r, overrides -r)\n");
	report("  -w           - suppress compiler warnings\n");
	report("  +W           - enable extra compiler warnings\n");
	report("example:\n snc +a -c vacuum.st\n");
}

/* Code generation support */

void gen_line_marker_prim(int line_num, const char *src_file)
{
	if (options.line)
		fprintf(out, "# line %d \"%s\"\n", line_num, src_file);
}

void gen_code(const char *format, ...)
{
	va_list args;

	va_start(args, format);
	vfprintf(out, format, args);
	va_end(args);
}

/* Errors and warnings */

void report_loc(const char *src_file, int line_num)
{
	fprintf(stderr, "%s:%d: ", src_file, line_num);
}

void report_at(const char *src_file, int line_num, const char *format, ...)
{
	va_list args;

	report_loc(src_file, line_num);

	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
}

void error_at(const char *src_file, int line_num, const char *format, ...)
{
	va_list args;

	err_cnt++;
	report_loc(src_file, line_num);

	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
}

/* location plus message and report a bug in snc */
void assert_at(int cond, const char *src_file, int line_num, const char *format, ...)
{
	if (!cond)
	{
		va_list args;

		report_loc(src_file, line_num);
		fprintf(stderr, "snc bug (assertion failed): ");
		va_start(args, format);
		vfprintf(stderr, format, args);
		va_end(args);
		exit(EXIT_FAILURE);
	}
}

void report_at_node(Node *ep, const char *format, ...)
{
	va_list args;

	report_loc(ep->token.file, ep->token.line);

	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
}

void warning_at_node(Node *ep, const char *format, ...)
{
	va_list args;

	if (!options.warn) return;
	report_loc(ep->token.file, ep->token.line);
	fprintf(stderr, "warning: ");

	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
}

void extra_warning_at_node(Node *ep, const char *format, ...)
{
	va_list args;

	if (!options.xwarn) return;
	report_loc(ep->token.file, ep->token.line);
	fprintf(stderr, "warning: ");

	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
}

void error_at_node(Node *ep, const char *format, ...)
{
	va_list args;

	err_cnt++;
	report_loc(ep->token.file, ep->token.line);
	fprintf(stderr, "error: ");

	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
}

/* with location from this node and report a bug in snc */
void assert_at_node(int cond, Node *ep, const char *format, ...)
{
	if (!cond)
	{
		va_list args;

		report_loc(ep->token.file, ep->token.line);
		fprintf(stderr, "snc bug (assertion failed): ");
		va_start(args, format);
		vfprintf(stderr, format, args);
		va_end(args);
		exit(EXIT_FAILURE);
	}
}

void report(const char *format, ...)
{
	va_list args;

	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
}
