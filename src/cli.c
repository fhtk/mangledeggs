/****************************************************************************\
 *                              mangledeggslib                              *
 *                                                                          *
 *                    Copyright © 2020 Alexander Nicholi                    *
 *                       Released under BSD-2-Clause.                       *
\****************************************************************************/

#include <glib.h>
#include <mangledeggs.h>

#define ERRMSG "Subroutine execution failed: %s\n"
#define RET_IF( x ) \
	do \
	{ \
		if( ( x ) ) \
		{ \
			return 127; \
		} \
	} while( 0 )
#define GERR_EXIT( RUN, ERR ) \
	do \
	{ \
		if( !( RUN ) ) \
		{ \
			g_print( ERRMSG, ( ERR )->message ); \
			return 127; \
		} \
	} while( 0 )

static gchar *  input     = NULL;
static gchar *  output    = NULL;
static gboolean byfile    = FALSE;
static gboolean reverse   = FALSE;
static gboolean nonewline = FALSE;

static const GOptionEntry opts[] = {
	{ "input",
	  'i',
	  0,
	  G_OPTION_ARG_STRING,
	  &input,
	  "Symbol PATH to (de)mangle",
	  "PATH" },
	{ "byfile",
	  'f',
	  0,
	  G_OPTION_ARG_NONE,
	  &byfile,
	  "Treat the input symbol path as a file whose contents are read as the symbolpath",
	  NULL },
	{ "output",
	  'o',
	  0,
	  G_OPTION_ARG_STRING,
	  &output,
	  "Write the output to FILE. If unspecified, stdout is used",
	  "FILE" },
	{ "demangle",
	  'd',
	  0,
	  G_OPTION_ARG_NONE,
	  &reverse,
	  "Demangle the input instead of mangling it by default",
	  NULL },
	{ "no-newline",
	  'n',
	  0,
	  G_OPTION_ARG_NONE,
	  &nonewline,
	  "Refrain from echoing a newline in the output. Only applies when using stdout",
	  NULL },
	{ NULL } };

int main( int ac, char * av[] )
{
	GError * e;
	gchar *  in;
	gchar *  out;
	size_t   in_sz;

	GOptionContext * opt_cntx;

	opt_cntx = g_option_context_new(
		"- (de)mangle symbols according to the mangledeggs spec" );
	g_option_context_add_main_entries( opt_cntx, opts, NULL );

	if( !g_option_context_parse( opt_cntx, &ac, &av, &e ) )
	{
		g_print( "Invalid options provided: %s\n", e->message );

		return 127;
	}

	if( input == NULL )
	{
		g_print( "No input provided. Exiting...\n" );

		return 127;
	}

	if( byfile )
	{
		GERR_EXIT(
			g_file_get_contents(
				(const gchar *)input, &in, NULL, &e ),
			e );
	}
	else
	{
		in = input;
	}

	in_sz = strlen( in );

	/* strip off all the trailing newlines */
	while( in[in_sz - 1] == '\n' )
	{
		in[in_sz - 1] = '\0';
		in_sz--;

		if( in[in_sz - 1] == '\r' )
		{
			in[in_sz - 1] = '\0';
			in_sz--;
		}
	}

	if( reverse )
	{
		gchar *  otmp;
		gchar ** oname;
		gchar *  otype;
		gchar *  tmpj[3];
		size_t   i;

		eg_demangle(
			in, (const char ***)&oname, (const char **)&otype );

		otmp    = g_strjoinv( "/", oname );
		tmpj[0] = otmp;
		tmpj[1] = otype;
		tmpj[2] = NULL;
		out     = g_strjoinv( ".", tmpj );
		g_free( otmp );
		g_strfreev( oname );
		g_free( otype );
	}
	else
	{
		gchar ** otmp;
		gchar ** otmp2;
		size_t   i;

		RET_IF( !g_strrstr( (const gchar *)in, "." ) );
		otmp  = g_strsplit( in, ".", 2 );
		otmp2 = g_strsplit( otmp[0], "/", -1 );

		eg_mangle(
			(const char **)otmp2,
			(const char *)( otmp[1] ),
			(const char **)&out );

		g_strfreev( otmp );
		g_strfreev( otmp2 );
	}

	if( output == NULL )
	{
		/* to stdout it goes */
		if( nonewline )
		{
			g_print( "%s", out );
		}
		else
		{
			g_print( "%s\n", out );
		}
	}
	else
	{
		GERR_EXIT(
			g_file_set_contents(
				output, (const gchar *)out, -1, &e ),
			e );
	}

	return 0;
}
