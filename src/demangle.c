/****************************************************************************\
 *                              mangledeggslib                              *
 *                                                                          *
 *                    Copyright © 2020 Alexander Nicholi                    *
 *                       Released under BSD-2-Clause.                       *
\****************************************************************************/

#include <mangledeggs.h>

#include <stdlib.h>
#include <string.h>

#define RET_IF( x ) \
	do \
	{ \
		if( ( x ) ) \
		{ \
			return; \
		} \
	} while( 0 )
#define MAX_ONAME 256

void eg_demangle(
	const char * in, const char *** outname, const char ** outtype )
{
	char *  buf;
	char *  oname[MAX_ONAME];
	char ** onametmp;
	char *  otype;
	size_t  i, oname_i, in_sz, buf_sz, buf_cap;
	int     namedone;

	RET_IF( in == NULL || outname == NULL || outtype == NULL ||
		in[0] != 'e' || in[0] == '\0' || in[1] != 'g' ||
		in[1] == '\0' );

	in_sz    = strlen( in );
	buf_sz   = 0;
	buf_cap  = 4;
	oname_i  = 0;
	namedone = 0;

	buf = malloc( sizeof( char ) * 4 );
	RET_IF( buf == NULL );

	for( i = 2; i < in_sz; ++i )
	{
		/* ensure the buffer is always big enough, firstly */
		while( buf_sz >= buf_cap )
		{
			buf = realloc( buf, buf_cap * 2 );
			RET_IF( buf == NULL );
			buf_cap *= 2;
		}

		if( in[i] >= 0x41 && in[i] <= 0x5A )
		{
			if( !namedone )
			{
				/* we hit a capital. flush the buffer */
				RET_IF( oname_i >= MAX_ONAME - 1 );
				oname[oname_i] = malloc(
					sizeof( char ) * ( buf_sz + 1 ) );
				RET_IF( oname[oname_i] == NULL );
				memcpy( oname[oname_i],
					(const char *)buf,
					buf_sz );
				oname[oname_i][buf_sz] = '\0';
				oname_i++;
				buf_sz = 0;
			}

			buf[buf_sz] = in[i] + 0x20;
			buf_sz++;
		}
		else if( in[i] == '_' )
		{
			RET_IF( namedone );
			/* we hit an underscore, name is done. flush the buffer
			 */
			RET_IF( oname_i >= MAX_ONAME - 1 );
			oname[oname_i] =
				malloc( sizeof( char ) * ( buf_sz + 1 ) );
			RET_IF( oname[oname_i] == NULL );
			memcpy( oname[oname_i], (const char *)buf, buf_sz );
			oname[oname_i][buf_sz] = '\0';
			oname_i++;
			buf_sz = 0;
		}
		else
		{
			buf[buf_sz] = in[i];
			buf_sz++;
		}
	}

	/* buffer contains the otype stuff. store it */
	otype = malloc( sizeof( char ) * ( buf_sz + 1 ) );
	RET_IF( otype == NULL );

	memcpy( otype, buf, buf_sz );
	otype[buf_sz] = '\0';

	/* new NULL-terminated array of strings */
	onametmp = malloc( sizeof( char * ) * ( oname_i + 1 ) );
	RET_IF( onametmp == NULL );

	for( i = 0; i < oname_i; ++i )
	{
		onametmp[i] = oname[i];
	}

	onametmp[oname_i] = NULL;

	/* copy over output into parameters */
	*outname = onametmp;
	*outtype = otype;
}
